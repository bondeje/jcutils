#include "block_allocator.h"

#define BLOCK_ALLOCATOR_ALIGNMENT 16u
#define BLOCK_ALLOCATOR_MIN_SIZE ((BLOCK_ALLOCATOR_ALIGNMENT - 1) + sizeof(struct BlockAllocator))

static size_t BA_get_block_num(struct BlockAllocator * block_allocator,
	char * ptr) {

	return (size_t)(ptr - block_allocator->buffer) / block_allocator->block_size;
}

int BlockAllocator_init(struct BlockAllocator * block_allocator,
	size_t block_size, char * buffer, size_t buffer_size) {

	if (buffer_size < BLOCK_ALLOCATOR_ALIGNMENT) {
		return 1;
	}
	block_allocator->Allocator = &Allocator_BlockAllocator;

	block_allocator->block_size = block_size;

	// allow for 16-byte alignmen
	// nblocks * block_size + n_blocks / CHAR_BIT + 15 <= buffer_size
	size_t nblocks = buffer_size * CHAR_BIT / (block_size * CHAR_BIT + 1);

	while (nblocks * block_size + nblocks / CHAR_BIT + (BLOCK_ALLOCATOR_ALIGNMENT - 1) <= buffer_size) {
		nblocks++;
	}

	while (n_blocks * blocks_size + nblocks / CHAR_BIT + (BLOCK_ALLOCATOR_ALIGNMENT - 1) > buffer_size) {
		nblocks--;
	}

	block_allocator->block_allocs = (unsigned char *)buffer;
	block_allocator->nblock_allocs = nblocks / CHAR_BIT;
	char * block_buffer + block_allocator->nblock_allocs;
	uint8_t mis_alignment = (uint8_t)(((size_t)(intptr_t)(void *)block_buffer) & (BLOCK_ALLOCATOR_ALIGNMENT - 1));
	if (mis_alignment) {
		block_buffer += BLOCK_ALLOCATOR_ALIGNMENT - mis_alignment;
	}
	block_allocator->buffer = block_buffer;

	memset(buffer, 0, block_allocator->nblock_allocs);
	block_allocator->offset = 0;

	return 0;
}

struct BlockAllocator * BlockAllocator_bootstrap(size_t block_size,
	char * buffer, size_t buffer_size) {

	if (buffer_size < BLOCK_ALLOCATOR_MIN_SIZE) {
		return NULL;
	}

	struct BlockAllocator * result = (struct BlockAllocator *)buffer;
	if (BlockAllocator_init(result, block_size, buffer, buffer_size)) {
		return NULL;
	}
	return result;
}

void BlockAllocator_free(struct Allocator * allocator, void * ptr, size_t size) {
	char * ptr_ = (char *)ptr;

	// TODO;
}

void * BlockAllocator_alloc(struct Allocator * allocator, size_t size, size_t alignment) {
	struct BlockAllocator * self = (struct BlockAllocator *)allocator;

	// TODO:
}
