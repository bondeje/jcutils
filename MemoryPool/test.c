#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include "mempool.c"
#include "../test/utils.h"

#define PRINT_SIZE_ALIGN(type) printf(STRINGIFY(type) " - size: %zu, align: %zu\n", sizeof(type), _Alignof(type))

#define MAX_TEST_COUNTS 16384
#define MAX_TEST_SIZE 16384
#ifdef __BIGGEST_ALIGNMENT__
#define MAX_TEST_ALIGN __BIGGEST_ALIGNMENT__
#else
#define MAX_TEST_ALIGN 16
#endif

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)

_Bool verbose = 0;

struct sa11 {int8_t data[1];};
struct sa21 {int8_t data[2];};
struct sa31 {int8_t data[3];};
struct sa41 {int8_t data[4];};

struct sa22 {int16_t data[1];};
struct sa32 {int16_t data[1]; char off;};
struct sa42 {int16_t data[2];};
struct sa62 {int16_t data[3];};
struct sa82 {int16_t data[4];};

struct sa44 {int32_t data[1];};
struct sa54 {int32_t data[1]; char off;};
struct sa84 {int32_t data[2];};
struct sa124 {int32_t data[3];};
struct sa164 {int32_t data[4];};

struct sa88 {int64_t data[1];};
struct sa98 {int64_t data[1]; char off;};
struct sa168 {int64_t data[2];};
struct sa248 {int64_t data[3];};
struct sa328 {int64_t data[4];};

#if __SIZEOF_LONG_DOUBLE__ > 8
struct sa1616 {long double data[1];};
struct sa1716 {long double data[1]; char off;};
struct sa3216 {long double data[2];};
struct sa4816 {long double data[3];};
struct sa6416 {long double data[4];};
#endif

_Bool is_aligned(void * ptr, unsigned char align) {
    return ((uintptr_t)ptr) % align == 0;
}

int test_mempoolmgr_new_del(void) {
    TEST_START(verbose);
    int nerrors = 0;

    for (size_t cts = 1; cts <= MAX_TEST_COUNTS; cts *= 2) {
        for (size_t size = 1; size <= MAX_TEST_SIZE; size *= 2) {
            for (size_t align = 1; align <= MAX_TEST_ALIGN; align *= 2) {
                MemPoolManager * mgr = MemPoolManager_new(cts, size, align);
                nerrors += CHECK(mgr != NULL, "failed to allocate MemPoolManager for (count, size, alignment)=(%zu, %zu, %zu)", cts, size, align);
                MemPoolManager_del(mgr);
            }
        }
    }

    TEST_END(verbose, nerrors);
    return nerrors;
}

int test_mempoolmgr_next(void) {
    TEST_START(verbose);
    int nerrors = 0;

#define MEMPOOLMGR_NEXT_TEST(type)\
    for (size_t cts = 1; cts <= MAX_TEST_COUNTS; cts *= 2) {\
        MemPoolManager * mgr = MemPoolManager(type, cts);\
        type * last = MemPoolManager_next(mgr);\
        nerrors += CHECK(last != NULL, "failed to allocate " STRINGIFY(type) " *\n");\
        nerrors += CHECK(is_aligned(last, _Alignof(type)), STRINGIFY(type) " * is misaligned: %p %% %zu = %zu\n", (void*)last, (size_t)_Alignof(type), (uintptr_t)(void*)last % _Alignof(type));\
        for (size_t i = 1; i < cts; i++) {\
            type * next = MemPoolManager_next(mgr);\
            nerrors += CHECK(next != NULL, "failed to allocate " STRINGIFY(type) " *\n");\
            nerrors += CHECK(is_aligned(next, _Alignof(type)), STRINGIFY(type) " * is misaligned: %p %% %zu = %zu\n", (void*)next, (size_t)_Alignof(type), (uintptr_t)(void*)next % _Alignof(type));\
            nerrors += CHECK((size_t)((char*)next - (char*)last) == sizeof(type), "subsequent " STRINGIFY(type) "s not adjacent: %p - %p = %zu\n", (void*)next, (void*)last, (size_t)((char*)next - (char*)last));\
            last = next;\
        }\
        /* this one should be in a new MemPool */\
        type * next = MemPoolManager_next(mgr);\
        nerrors += CHECK(next != NULL, "failed to allocate " STRINGIFY(type) " *\n");\
        nerrors += CHECK(is_aligned(next, _Alignof(type)), STRINGIFY(type) " * is misaligned: %p %% %zu = %zu\n", (void*)next, (size_t)_Alignof(type), (uintptr_t)(void*)next % _Alignof(type));\
        nerrors += CHECK((size_t)((char*)next - (char*)last) != sizeof(type), "subsequent " STRINGIFY(type) " past count is not in a new pool: %p - %p = %zu\n", (void*)next, (void*)last, (size_t)((char*)next - (char*)last));\
        MemPoolManager_del(mgr);\
    }

    MEMPOOLMGR_NEXT_TEST(char)
    MEMPOOLMGR_NEXT_TEST(short)
    MEMPOOLMGR_NEXT_TEST(double)
    MEMPOOLMGR_NEXT_TEST(long double)
    MEMPOOLMGR_NEXT_TEST(struct sa31)
    MEMPOOLMGR_NEXT_TEST(struct sa32)
    MEMPOOLMGR_NEXT_TEST(struct sa54)
    MEMPOOLMGR_NEXT_TEST(struct sa98)
#if __SIZEOF_LONG_DOUBLE__ > 8
    MEMPOOLMGR_NEXT_TEST(struct sa1716)
#endif

    TEST_END(verbose, nerrors);
    return nerrors;
}

int test_mempoolmgr_malloc(void) {
    TEST_START(verbose);
    int nerrors = 0;

#define MEMPOOLMGR_MALLOC_TEST(type)\
    for (size_t array_size = 1; array_size <= MAX_TEST_COUNTS / 2; array_size *= 2) {\
        /* allocating an array at a time, each pool has space for 2 arrays sequentially*/\
        MemPoolManager * mgr = MemPoolManager(type, array_size * 2);\
        type * last = MemPoolManager_malloc(mgr, array_size * sizeof(type));\
        nerrors += CHECK(last != NULL, "failed to allocate " STRINGIFY(type) " *\n");\
        nerrors += CHECK(is_aligned(last, _Alignof(type)), STRINGIFY(type) " * is misaligned: %p %% %zu = %zu\n", (void*)last, (size_t)_Alignof(type), (uintptr_t)(void*)last % _Alignof(type));\
        type * next = MemPoolManager_malloc(mgr, array_size * sizeof(type));\
        nerrors += CHECK(next != NULL, "failed to allocate " STRINGIFY(type) " *\n");\
        nerrors += CHECK(is_aligned(next, _Alignof(type)), STRINGIFY(type) " * is misaligned: %p %% %zu = %zu\n", (void*)next, (size_t)_Alignof(type), (uintptr_t)(void*)next % _Alignof(type));\
        nerrors += CHECK((size_t)((char*)next - (char*)last) == array_size * sizeof(type), "subsequent " STRINGIFY(type) "s not adjacent: %p - %p = %zu\n", (void*)next, (void*)last, (size_t)((char*)next - (char*)last));\
        last = next;\
        next = MemPoolManager_malloc(mgr, array_size * sizeof(type));\
        nerrors += CHECK(next != NULL, "failed to allocate " STRINGIFY(type) " *\n");\
        nerrors += CHECK(is_aligned(next, _Alignof(type)), STRINGIFY(type) " * is misaligned: %p %% %zu = %zu\n", (void*)next, (size_t)_Alignof(type), (uintptr_t)(void*)next % _Alignof(type));\
        nerrors += CHECK((size_t)((char*)next - (char*)last) != array_size * sizeof(type), "subsequent " STRINGIFY(type) "s not adjacent: %p - %p = %zu\n", (void*)next, (void*)last, (size_t)((char*)next - (char*)last));\
        last = next;\
        next = MemPoolManager_malloc(mgr, (1+array_size) * sizeof(type));\
        nerrors += CHECK(next != NULL, "failed to allocate " STRINGIFY(type) " *\n");\
        nerrors += CHECK(is_aligned(next, _Alignof(type)), STRINGIFY(type) " * is misaligned: %p %% %zu = %zu\n", (void*)next, (size_t)_Alignof(type), (uintptr_t)(void*)next % _Alignof(type));\
        nerrors += CHECK((size_t)((char*)next - (char*)last) != array_size * sizeof(type), "subsequent " STRINGIFY(type) "s not adjacent: %p - %p = %zu\n", (void*)next, (void*)last, (size_t)((char*)next - (char*)last));\
        MemPoolManager_del(mgr);\
    }

    MEMPOOLMGR_MALLOC_TEST(char)
    MEMPOOLMGR_MALLOC_TEST(short)
    MEMPOOLMGR_MALLOC_TEST(double)
    MEMPOOLMGR_MALLOC_TEST(struct sa31)
    MEMPOOLMGR_MALLOC_TEST(struct sa32)
    MEMPOOLMGR_MALLOC_TEST(struct sa54)
    MEMPOOLMGR_MALLOC_TEST(struct sa98)
#if __SIZEOF_LONG_DOUBLE__ > 8
    MEMPOOLMGR_MALLOC_TEST(long double)
    MEMPOOLMGR_MALLOC_TEST(struct sa1716)
#endif

    TEST_END(verbose, nerrors);
    return nerrors;
}

// TODO: this test needs work
int test_mempoolmgr_aligned_alloc(void) {
    TEST_START(verbose);
    int nerrors = 0;

// test 2, 4, 8, 16 (if available)
#define MEMPOOLMGR_ALIGNED_ALLOC_TEST(type) {\
    MemPoolManager * mgr = MemPoolManager_new(2, sizeof(type), _Alignof(type));\
    /* mis-aligning object */\
    struct sa11 * last = MemPoolManager_malloc(mgr, sizeof(struct sa11));\
    nerrors += CHECK(last != NULL, "failed to allocate " STRINGIFY(type) " *\n");\
    nerrors += CHECK(is_aligned(last, _Alignof(type)), STRINGIFY(type) " * is misaligned: %p %% %zu = %zu\n", (void*)last, (size_t)_Alignof(type), (uintptr_t)(void*)last % _Alignof(type));\
    /* aligned object */\
    void * next = MemPoolManager_aligned_alloc(mgr, sizeof(type), _Alignof(type));\
    nerrors += CHECK(next != NULL, "failed to allocate " STRINGIFY(type) " *\n");\
    nerrors += CHECK(is_aligned(next, _Alignof(type)), STRINGIFY(type) " * is misaligned: %p %% %zu = %zu\n", (void*)next, (size_t)_Alignof(type), (uintptr_t)(void*)next % _Alignof(type));\
    /* setup is intentially so that 'next' is not aligned to next higher alignment */\
    nerrors += CHECK((size_t)((char*)next - (char*)last) == _Alignof(type), "subsequent " STRINGIFY(type) "s not adjacent: %p - %p = %zu\n", (void*)next, (void*)last, (size_t)((char*)next - (char*)last));\
    MemPoolManager_del(mgr); }

    MEMPOOLMGR_ALIGNED_ALLOC_TEST(struct CAT3(sa, 2, 2))
    MEMPOOLMGR_ALIGNED_ALLOC_TEST(struct CAT3(sa, 4, 4))
    MEMPOOLMGR_ALIGNED_ALLOC_TEST(struct CAT3(sa, 8, 8))
#if __BIGGEST_ALIGNMENT__ > 8
    MEMPOOLMGR_ALIGNED_ALLOC_TEST(struct CAT3(sa, 16, 16))
#endif

    TEST_END(verbose, nerrors);
    return nerrors;
}

int test_mempoolmgr_malloc_large(void) {
    TEST_START(verbose);
    int nerrors = 0;

    int size_mempool = 4;
    MemPoolManager * mgr = MemPoolManager_new(size_mempool, sizeof(double), _Alignof(double));
    double * test = MemPoolManager_malloc(mgr, (size_mempool + 1) * sizeof(double));
    nerrors += CHECK(test, "failed to allocate memory for %d doubles a mempool of size %d\n", size_mempool, size_mempool + 1);

    MemPoolManager_del(mgr);

    TEST_END(verbose, nerrors);
    return nerrors;
}

int main(int narg, char ** args) {
    for (int i = 1; i < narg; i++) {
        if (!strcmp(args[i], "--showtypes")) {
            PRINT_SIZE_ALIGN(struct sa11);
            PRINT_SIZE_ALIGN(struct sa21);
            PRINT_SIZE_ALIGN(struct sa31);
            PRINT_SIZE_ALIGN(struct sa41);

            PRINT_SIZE_ALIGN(struct sa22);
            PRINT_SIZE_ALIGN(struct sa32);
            PRINT_SIZE_ALIGN(struct sa42);
            PRINT_SIZE_ALIGN(struct sa62);
            PRINT_SIZE_ALIGN(struct sa82);

            PRINT_SIZE_ALIGN(struct sa44);
            PRINT_SIZE_ALIGN(struct sa54);
            PRINT_SIZE_ALIGN(struct sa84);
            PRINT_SIZE_ALIGN(struct sa124);
            PRINT_SIZE_ALIGN(struct sa164);

            PRINT_SIZE_ALIGN(struct sa88);
            PRINT_SIZE_ALIGN(struct sa98);
            PRINT_SIZE_ALIGN(struct sa168);
            PRINT_SIZE_ALIGN(struct sa248);
            PRINT_SIZE_ALIGN(struct sa328);
    #if __SIZEOF_LONG_DOUBLE__ > 8
            PRINT_SIZE_ALIGN(struct sa1616);
            PRINT_SIZE_ALIGN(struct sa1716);
            PRINT_SIZE_ALIGN(struct sa3216);
            PRINT_SIZE_ALIGN(struct sa4816);
            PRINT_SIZE_ALIGN(struct sa6416);
    #endif
        } else if (!strcmp(args[i], "--verbose")) {
            verbose = 1;
        }
    }
    int nerrors = test_mempoolmgr_new_del();
    nerrors += test_mempoolmgr_next();
    nerrors += test_mempoolmgr_malloc();
    nerrors += test_mempoolmgr_aligned_alloc();
    nerrors += test_mempoolmgr_malloc_large();

    return nerrors;
}

