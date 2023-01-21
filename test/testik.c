#include "testik.h"

void testik() {
    printf("\tRunning 1 test.\n");
    void* result1 = _malloc(64);
    void* result2 = _malloc(256);
    void* result3 = _malloc(12);
    debug_struct_info(stdout, result1);
    debug_struct_info(stdout, result2);
    debug_struct_info(stdout, result3);
    _free(result1);
    debug_struct_info(stdout, result1);
    debug_struct_info(stdout, result2);
    debug_struct_info(stdout, result3);
}