#ifndef MEMPOOL_H
#define MEMPOOL_H

#include <stddef.h>

typedef struct MemPool MemPool;
typedef struct MemPoolManager MemPoolManager;

#define MemPoolManager(type, count) MemPoolManager_new(count, sizeof(type), _Alignof(type))

MemPoolManager * MemPoolManager_new(size_t count, size_t size, size_t alignment);
void MemPoolManager_del(MemPoolManager * self);
void * MemPoolManager_next(MemPoolManager * self);
void * MemPoolManager_malloc(MemPoolManager * self, size_t size);
void * MemPoolManager_aligned_alloc(MemPoolManager * self, size_t size, size_t alignment);

#endif


