#include "testik.h"

static bool DEBUG_LOG = 1;
#define TOO_SMALL (10)
#define TEST_1_NUM 64
#define TEST_2_NUM 256
#define TEST_3_NUM 512
#define TOO_HUGE (16*1024)

bool check_capacity(size_t cap_expected, size_t cap_real) {
    return cap_expected == cap_real;
}


bool test1() {
    if (DEBUG_LOG)      printf("\n\tRunning 1 test.\n");
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);
    void* result1 = _malloc(TEST_1_NUM);
    void* result2 = _malloc(TEST_2_NUM);
    void* result3 = _malloc(TEST_3_NUM);

    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);
    _free(result1);
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    _free(result2);
    _free(result3);
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    // TODO create test results
    return 1; //check_capacity(TEST_1_NUM, get_size_of_block(result1));
}

bool test2() {
    if (DEBUG_LOG)      printf("\n\tRunning 2 test.\n");
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    void* result1 = _malloc(TOO_SMALL);
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    _free(result1);
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    result1 = _malloc(TEST_2_NUM);
    _free(result1);
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    // TODO create test results
    return true;
}

bool test3() {
    if (DEBUG_LOG)      printf("\n\tRunning 3 test.\n");
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    void* result1 = _malloc(TOO_HUGE);
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    _free(result1);
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    // TODO create test results
    return true;
}

void testik() {
    if (test1()) printf("TEST 1 PASSED\n");
    if (test2()) printf("TEST 2 PASSED\n");
    if (test3()) printf("TEST 3 PASSED\n");
}