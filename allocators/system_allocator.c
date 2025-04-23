#include "system_allocator.h"

void SA_free(__attribute__((unused)) INTERFACE_PARAM(Allocator), void * ptr, __attribute__((unused)) size_t size) {
    free(ptr);
}

#if __STDC_VERSION__ >= 201112L
void * SA_alloc(__attribute__((unused)) INTERFACE_PARAM(Allocator), size_t size, size_t alignment) {
    return aligned_alloc(alignment, size);
}
#else
void * SA_alloc(__attribute__((unused)) INTERFACE_PARAM(Allocator), size_t size, __attribute__((unused)) size_t alignment) {
    return malloc(size);
}
#endif

void * SA_realloc(__attribute__((unused)) INTERFACE_PARAM(Allocator), void * ptr, size_t new_size, __attribute__((unused)) size_t old_size) {
    return realloc(ptr, new_size);
}

struct SystemAllocator sys_alloc_obj = {
    .Allocator = INTERFACE_INITIALIZER_START(Allocator)
        .alloc = SA_alloc,
        .realloc = SA_realloc,
        .free = SA_free
    INTERFACE_INITIALIZER_END
};

struct SystemAllocator * sys_alloc = &sys_alloc_obj;

struct SystemAllocator * SystemAllocator_new(void) {
    return sys_alloc;
}
