#ifndef _MEM_H_
#define _MEM_H_


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// linux fans...
#include <sys/mman.h>

#define HEAP_START ((void*)0x04040000)

void* _malloc( size_t query );
void  _free( void* mem );
void* heap_init( size_t initial_size );

#define DEBUG_FIRST_BYTES 4

void debug_struct_info( FILE* f, void const* address );
void debug_heap( FILE* f,  void const* ptr );
struct block_header* head_of_block( void const* contents );
size_t get_size_of_block(void const* contents);
size_t get_capacity_of_block(void const* contents);

#endif
