#include <stdarg.h>
#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "mem_internals.h"
#include "mem.h"
#include "util.h"

#include <sys/mman.h>

void debug_block(struct block_header* b, const char* fmt, ... );
void debug(const char* fmt, ... );

extern inline block_size size_from_capacity( block_capacity cap );
extern inline block_capacity capacity_from_size( block_size sz );

static bool            block_is_big_enough( size_t query, struct block_header* block ) { return block->capacity.bytes >= query; }
static size_t          pages_count   ( size_t mem )                      { return mem / getpagesize() + ((mem % getpagesize()) > 0); }
static size_t          round_pages   ( size_t mem )                      { return getpagesize() * pages_count( mem ) ; }

static void block_init( void* restrict addr, block_size block_sz, void* restrict next ) {
  *((struct block_header*)addr) = (struct block_header) {
    .next = next,
    .capacity = capacity_from_size(block_sz),
    .is_free = true
  };
}

static size_t region_actual_size( size_t query ) {
    return size_max( round_pages( query ), REGION_MIN_SIZE );
}

extern inline bool region_is_invalid( const struct region* r );



static void* map_pages(void const* addr, size_t length, int additional_flags) {
  return mmap( (void*) addr, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | additional_flags , 0, 0 );
}

/*  аллоцировать регион памяти и инициализировать его блоком */
static struct region alloc_region  ( void const* addr, size_t query ) {
    if (addr == NULL)
        return REGION_INVALID;

    query = size_from_capacity((block_capacity) {region_actual_size(query)}).bytes;
    block_size size = size_from_capacity((block_capacity) {query});

    void* new_addr = map_pages(addr, query, MAP_FIXED);
    if (new_addr != addr) {
        new_addr = map_pages(addr, query, 0);
    }

    block_init(new_addr, size, NULL);

    return (struct region) {(void*) new_addr, query, false};
}

static void* block_after( struct block_header const* block );

void* heap_init( size_t initial ) {
    const struct region region = alloc_region( HEAP_START, initial );
    if ( region_is_invalid(&region) ) return NULL;
    return region.addr;
}

#define BLOCK_MIN_CAPACITY 24

/*  --- Разделение блоков (если найденный свободный блок слишком большой )--- */

static bool block_splittable( struct block_header* restrict block, size_t query) {
  return block-> is_free && query + offsetof( struct block_header, contents ) + BLOCK_MIN_CAPACITY <= block->capacity.bytes;
}

static bool split_if_too_big( struct block_header* block, size_t query ) {
    if (!block_splittable(block, query))
        return false;

    size_t size_of_second_block = size_from_capacity(block->capacity).bytes - size_from_capacity((block_capacity) {query}).bytes;

    block_init(block,
               size_from_capacity((block_capacity) {query}),
               NULL);


    void* next_of_second_block = block->next;
    block->next = block_after(block);

    block_init(block->next,
               (block_size) {size_of_second_block},
               next_of_second_block);
    return true;
}


/*  --- Слияние соседних свободных блоков --- */

static void* block_after( struct block_header const* block )              {
  return  (void*) (block->contents + block->capacity.bytes);
}

static bool blocks_continuous (
                               struct block_header const* fst,
                               struct block_header const* snd ) {
  return (void*)snd == block_after(fst);
}

static bool mergeable(struct block_header const* restrict fst, struct block_header const* restrict snd) {
  return fst && snd && fst->is_free && snd->is_free && blocks_continuous( fst, snd ) ;
}

static bool try_merge_with_next( struct block_header* block ) {
    if (mergeable(block, block->next)) {
        block->capacity.bytes += size_from_capacity(block->next->capacity).bytes;
        block->next = block->next->next;
        return true;
    }
    return false;
}

void merge_free_blocks(struct block_header*  header){
    while (header->next) {
        // if merged then try it with next one
        if(!try_merge_with_next(header))
            header = header->next;
    }
}


/*  --- ... ecли размера кучи хватает --- */

struct block_search_result {
  enum {BSR_FOUND_GOOD_BLOCK, BSR_REACHED_END_NOT_FOUND, BSR_CORRUPTED} type;
  struct block_header* block;
};


static struct block_search_result find_good_or_last( struct block_header* restrict block, size_t sz )    {
    struct block_search_result result;
    if (block == NULL) {
        result.type = BSR_CORRUPTED;
        result.block = NULL;
    }

    while (block->next) {
        if (block->is_free && block_is_big_enough(sz, block)) {
            result.type = BSR_FOUND_GOOD_BLOCK;
            result.block = block;
            break;
        }
        block= block->next;
    }

    result.type = block_is_big_enough(sz, block) && block->is_free ? BSR_FOUND_GOOD_BLOCK : BSR_REACHED_END_NOT_FOUND;
    return result;
}

static struct block_search_result try_memalloc_existing ( size_t query, struct block_header* block ) {
    struct block_search_result result = find_good_or_last(block, query);
    return result;
}


static struct block_header* grow_heap( struct block_header* restrict last, size_t query ) {
    struct region new_region = alloc_region(block_after(last),
                                        size_max(query, REGION_MIN_SIZE));

    if (!region_is_invalid(&new_region) && !last) {
        last->next = new_region.addr;
        return new_region.addr;
    }
    merge_free_blocks(HEAP_START);
    return NULL;
}

/*  Реализует основную логику malloc и возвращает заголовок выделенного блока */
static struct block_header* memalloc( size_t query, struct block_header* heap_start) {
    struct block_search_result result = try_memalloc_existing(query, heap_start);

    switch (result.type) {
        case BSR_FOUND_GOOD_BLOCK:
            split_if_too_big(result.block, query);
            break;

        case BSR_REACHED_END_NOT_FOUND:
            grow_heap(result.block, query);
            result = try_memalloc_existing(query, heap_start);
            break;
        default:
            return NULL;
    }
    return result.block;
}

void* _malloc( size_t query ) {
    struct block_header* const addr = memalloc(
          size_max(query, BLOCK_MIN_CAPACITY),
          (struct block_header*) HEAP_START);

    if (addr) {
        addr->is_free = false;
        return addr->contents;
    }
    return NULL;
}

static struct block_header* block_get_header(void* contents) {
  return (struct block_header*) (((uint8_t*)contents)-offsetof(struct block_header, contents));
}

void _free( void* mem ) {
    if (!mem) return ;

    struct block_header* header = block_get_header( mem );
    header->is_free = true;
    merge_free_blocks(HEAP_START);
}
