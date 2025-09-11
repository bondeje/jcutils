#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdlib.h>
#include "Utils/macros.h"
#include "Utils/interface.h"

struct Allocator;

typedef void AllocatorFree_f(INTERFACE_PARAM(Allocator), void *, size_t);
typedef void * AllocatorAlloc_f(INTERFACE_PARAM(Allocator), size_t, size_t);
typedef void * AllocatorRealloc_f(INTERFACE_PARAM(Allocator), void *, size_t, size_t);

static inline void Allocator_free(struct Allocator * allocator, void * ptr, size_t size) {
	return allocator->free(allocator, ptr, size);
}

static inline void * Allocator_alloc(struct Allocator * allocator, size_t size, size_t alignment) {
	return allocator->alloc(allocator, size, alignment);
}

static inline void * Allocator_realloc(struct Allocator * allocator, void * ptr, size_t size, size_t alignment) {
	return allocator->realloc(allocator, ptr, size, alignment);
}

DECLARE_INTERFACE_START(Allocator)
    AllocatorAlloc_f * alloc;
    AllocatorRealloc_f * realloc;
    AllocatorFree_f * free;
DECLARE_INTERFACE_END

#endif // ALLOCATOR_H
