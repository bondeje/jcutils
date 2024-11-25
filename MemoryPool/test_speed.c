#define _XOPEN_SOURCE 700

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include <time.h>
#include <sys/types.h>

#include "mempool.h"

double timespec_to_double(struct timespec * first, struct timespec * second) {
    return (second->tv_nsec*1.0 - first->tv_nsec) * 1e-9 + second->tv_sec - first->tv_sec;
}

int run(size_t size, size_t N_elements, size_t max_pool_ct) {
    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (size_t i = 0; i < N_elements; i++) {
        void * throwaway = malloc(size);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    printf("malloc time for %zu allocations: %f\n", N_elements, timespec_to_double(&start, &stop));

    for (size_t ct = 1; ct <= max_pool_ct; ct *= 2) {
        MemPoolManager * mgr = MemPoolManager_new(ct, size, size);    
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (size_t i = 0; i < N_elements; i++) {
            void * throwaway = MemPoolManager_next(mgr);
        }
        clock_gettime(CLOCK_MONOTONIC, &stop);
        MemPoolManager_del(mgr);

        printf("pool count size: %zu, time for %zu allocations: %f\n", ct, N_elements, timespec_to_double(&start, &stop));
    }
    return 0;
}

int main(int narg, char ** args) {
    size_t N = 1000000;
    size_t max_pool_ct = 1024*64;
    size_t size = sizeof(double);
    if (narg > 1) {
        N = (size_t)atoll(args[1]);
        if (!N) {
            return 1;
        }
    }
    if (narg > 2) {
        size = (size_t)atoll(args[2]);
        if (!N) {
            return 2;
        }
    }

    return run(size, N, max_pool_ct);
}

