#include "testik.h"

static bool DEBUG_LOG = 1;

bool test1() {
    if (DEBUG_LOG)      printf("\n\tRunning 1 test.\n");
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);
    void* result1 = _malloc(64);
    void* result2 = _malloc(256);
    void* result3 = _malloc(10);

    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);
    _free(result1);
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    _free(result2);
    _free(result3);
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    // TODO create test results
    return true;
}

bool test2() {
    if (DEBUG_LOG)      printf("\n\tRunning 2 test.\n");
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    void* result1 = _malloc(64);
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    _free(result1);
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    result1 = _malloc(64);
    _free(result1);
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    // TODO create test results
    return true;
}

bool test3() {
    if (DEBUG_LOG)      printf("\n\tRunning 3 test.\n");
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    void* result1 = _malloc(8*2048);
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    _free(result1);
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    // TODO create test results
    return true;
}

void testik() {
    test2();
    test1();
    test3();
}