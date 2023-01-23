#include "testik.h"

static bool DEBUG_LOG = 1;
#define TOO_SMALL (10)
#define TEST_1_NUM 64
#define TEST_2_NUM 256
#define TEST_3_NUM 512
#define TOO_HUGE (10*1024)

bool is_equal(size_t cap_expected, size_t cap_real) {
    return cap_expected == cap_real;
}

size_t size_from_capacity_int(size_t capacity) {
    return size_from_capacity((block_capacity){capacity}).bytes;
}


bool test1() {
    bool bool_result = true;

    if (DEBUG_LOG)      printf("\n\tRunning 1 test.\n");
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);
    void* result1 = _malloc(TEST_1_NUM);
    void* result2 = _malloc(TEST_2_NUM);
    void* result3 = _malloc(TEST_3_NUM);

    bool_result = is_equal(TEST_1_NUM, get_capacity_of_block(result1)) &&
                                is_equal(TEST_2_NUM, get_capacity_of_block(result2)) &&
                                is_equal(TEST_3_NUM, get_capacity_of_block(result3));

    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);
    _free(result1);
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    _free(result2);
    _free(result3);
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    return bool_result;
}

bool test2() {
    bool bool_result = true;

    if (DEBUG_LOG)      printf("\n\tRunning 2 test.\n");
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    void* result1 = _malloc(TOO_SMALL);
    size_t capacity1 = get_capacity_of_block(result1);

    bool_result = is_equal(size_max(TOO_SMALL, 24), capacity1);
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    _free(result1);
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    result1 = _malloc(TEST_2_NUM);
    size_t capacity2 = get_capacity_of_block(result1);
    bool_result = bool_result && is_equal(TEST_2_NUM, capacity2);
    _free(result1);

    if(!bool_result) {
        fprintf(stderr, "Expected %d, but capacity is equal: %zu\n", TOO_SMALL, capacity1);
        fprintf(stderr, "Expected %d, but capacity is equal: %zu\n", TEST_2_NUM, capacity2);
    }
    return bool_result;
}

bool test3() {
    bool bool_result = true;

    if (DEBUG_LOG)      printf("\n\tRunning 3 test.\n");
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    void* result1 = _malloc(TOO_HUGE);
    bool_result = is_equal(TOO_HUGE, get_capacity_of_block(result1));
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    _free(result1);
    if (DEBUG_LOG)      debug_heap(stdout, HEAP_START);

    if(!bool_result) {
        fprintf(stderr, "Expected %d, but capacity is equal: %zu\n", TOO_HUGE, get_capacity_of_block(result1));
    }
    return bool_result;
}

void testik() {
    //if (test1()) printf("TEST 1 PASSED\n");
    //if (test2()) printf("TEST 2 PASSED\n");
    if (test3()) printf("TEST 3 PASSED\n");
}