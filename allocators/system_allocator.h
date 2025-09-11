#ifndef SYSTEM_ALLOCATOR_H
#define SYSTEM_ALLOCATOR_H

#include "allocator.h"

struct SystemAllocator {
    IMPLEMENTS(Allocator);
};

DECLARE_IMPLEMENTATION(Allocator, SystemAllocator);

#endif // SYSTEM_ALLOCATOR_H
