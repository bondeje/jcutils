#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdlib.h>
#include "utils/macros.h"
#include "utils/interface.h"

struct Allocator;

typedef void AllocatorFree_f(INTERFACE_PARAM(Allocator), void *, size_t);
typedef void * AllocatorAlloc_f(INTERFACE_PARAM(Allocator), size_t, size_t);
typedef void * AllocatorRealloc_f(INTERFACE_PARAM(Allocator), void *, size_t, size_t);

DECLARE_INTERFACE_START(Allocator)
    AllocatorAlloc_f * alloc;
    AllocatorRealloc_f * realloc;
    AllocatorFree_f * free;
DECLARE_INTERFACE_END

#endif // ALLOCATOR_H
