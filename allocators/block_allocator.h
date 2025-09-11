#ifndef BLOCK_ALLOCATOR_H
#define BLOCK_ALLOCATOR_H

#include "allocator.h"

struct BlockAllocator {
	IMPLEMENTS(Allocator);
	unsigned char * block_allocs;
	char * buffer;
	size_t offset; // pointer into block_allocations
	size_t block_size;
	size_t nblock_allocs;
};

DECLARE_IMPLEMENTATION(Allocator, BlockAllocator);

void BlockAllocator_init(struct BlockAllocator * block_allocator,
	size_t block_size, char * buffer, size_t buffer_size);

struct BlockAllocator * BlockAllocator_bootstrap(size_t block_size,
	char * buffer, size_t buffer_size);

#endif // STACK_ALLOCATOR_H
