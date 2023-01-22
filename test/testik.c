#include "testik.h"

void testik() {
    printf("\tRunning 1 test.\n");
    //debug_heap(stdout, HEAP_START);
    void* result1 = _malloc(64);
    void* result2 = _malloc(256);
    void* result3 = _malloc(12);

    //debug_heap(stdout, HEAP_START);
    _free(result1);
    //debug_heap(stdout, HEAP_START);
    _free(result2);
    _free(result3);
}