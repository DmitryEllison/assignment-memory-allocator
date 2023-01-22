#include "test/testik.h"

int main() {
    if (heap_init(REGION_MIN_SIZE) == NULL)
        return -1;
    testik();
    return 0;
}
