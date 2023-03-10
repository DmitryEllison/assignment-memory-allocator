#include <stdio.h>
#include <stdarg.h>
#include "mem_internals.h"
#include "mem.h"


struct block_header* head_of_block( void const* contents ) {
    return  (void*) ((uint8_t*)contents - offsetof(struct block_header, contents));
}

size_t get_size_of_block(void const* contents) {
    debug_struct_info(stdout, head_of_block(contents));
    return size_from_capacity(head_of_block(contents)-> capacity).bytes;
}

size_t get_capacity_of_block(void const* contents) {
    return head_of_block(contents)-> capacity.bytes;
}

void debug_struct_info( FILE* f, void const* addr ) {
    struct block_header const* header =  addr;
        fprintf( f,
        "%12p %12p %12zu %12zu %8s   ",
        addr,
        (void*)(header->next),
        header-> capacity.bytes,
        size_from_capacity(header-> capacity).bytes,
        header-> is_free? "free" : "taken"
    );
    for ( size_t i = 0; i < DEBUG_FIRST_BYTES && i < header -> capacity.bytes; ++i )
        fprintf( f, "%hhX", header-> contents[i] );
    fprintf( f, "\n" );
}

void debug_heap( FILE* f,  void const* ptr ) {
  fprintf( f, "\n --- Heap ---\n");
  fprintf( f, "%12s %12s %12s %12s %8s %10s\n", "start", "next", "capacity", "block_size", "status", "contents" );
  for(struct block_header const* header =  ptr; header; header = header ->next )
    debug_struct_info( f, header );
}

void debug_block(struct block_header* b, const char* fmt, ... ) {
  #ifdef DEBUG

  va_list args;
  va_start (args, fmt);
  vfprintf(stderr, fmt, args);
  debug_struct_info( stderr, b );
  va_end (args);

  #else
  (void) b; (void) fmt;
  #endif
}

void debug(const char* fmt, ... ) {
#ifdef DEBUG

  va_list args;
  va_start (args, fmt);
  vfprintf(stderr, fmt, args);
  va_end (args);

#else
  (void) fmt;
#endif
}
