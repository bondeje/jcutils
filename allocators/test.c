#include <string.h>
#include <stdio.h>
#include "test/utils.h"
#include "system_allocator.c"

_Bool verbose = 0;

int hello_world_system_allocator(void) {
    TEST_START(verbose);
    int nerrors = 0;

    struct SystemAllocator * my_alloc = SystemAllocator_new();

    char * buffer = ICALL(my_alloc, Allocator, alloc, sizeof(*buffer) * 20, 1);

    memcpy(buffer, "Hello, World!\n", 14);

    nerrors += !printf("%.*s", 14, buffer);

    ICALL(my_alloc, Allocator, free, buffer, 0);

    TEST_END(verbose, nerrors);
    return nerrors;
}

int main(int narg, char ** args) {

    if (narg > 1 && !strcmp(args[1], "--verbose")) {
        verbose = 1;
    }

    hello_world_system_allocator();

    return 0;
}