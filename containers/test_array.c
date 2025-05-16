#include <stdio.h>
#include "array.h"

DECLARE_Container(int)
DECLARE_Iterable(int)
DECLARE_Iterator(int)
DECLARE_Sequence(int)
DECLARE_Array(int)
DEFINE_Array(int)

int main(void) {
    struct Array(int) int_array = {&(int[]){0, 1, 2, 3}[0], 4, Array_initer(int)};

    printf("Array size is %zu\n", ICALL(&int_array, Container(int), len));
    return 0;
}