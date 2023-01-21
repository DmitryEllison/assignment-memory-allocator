#include "test/testik.h"

int main() {
    heap_init(REGION_MIN_SIZE);
    testik();
    return 0;
}
