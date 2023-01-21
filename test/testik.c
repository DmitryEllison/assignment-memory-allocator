#include "testik.h"

void testik() {
    void* result = _malloc(1024);
    debug_struct_info(stdout, result);
    _free(result);
}