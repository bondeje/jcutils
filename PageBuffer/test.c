#include <stddef.h>
#include <string.h>
#include "page_buffer.c"

#include "../test/utils.h"

int verbosity = 0;

void test_PageBuffer_new(void) {
    TEST_START(verbosity);
    int nerrors = 0;

    PageBuffer * pb = PageBuffer_new(0);
    nerrors += CHECK(pb, "failed default new Buffer");
    if (pb) {
        nerrors += CHECK(PageBuffer_size(pb) == PageBuffer_get_default_size() - sizeof(struct PageBuffer), "did not initialize to default size. found: %zu, expected: %zu", PageBuffer_size(pb), PageBuffer_get_default_size() - sizeof(struct PageBuffer));
    }
    PageBuffer_del(pb);

    ptrdiff_t size = 400;
    pb = PageBuffer_new(size);
    nerrors += CHECK(pb, "failed default new Buffer of size %lld", (long long)size);
    if (pb) {
        nerrors += CHECK(PageBuffer_size(pb) == (size_t)size, "did not initialize to default size. found: %zu, expected: %zu", PageBuffer_size(pb), (size_t)size);
    }
    PageBuffer_del(pb);

    TEST_END(verbosity, nerrors);
}

void test_pbnprintf(void) {
    TEST_START(verbosity);
    int nerrors = 0;

    char const * hw = "Hello, World";
    size_t strlength = strlen(hw);

    ptrdiff_t size = 40;
    PageBuffer * pb = PageBuffer_new(size);
    nerrors += CHECK(pb, "failed default new Buffer of size %lld", (long long)size);
    if (pb) {
        nerrors += CHECK(PageBuffer_size(pb) == (size_t)size, "did not initialize to default size. found: %zu, expected: %zu", PageBuffer_size(pb), (size_t)size);

        size_t length = 0;
        for (int i = 0; i < (int)(size / strlength * 8); i++) {
            pbnprintf(&pb, "%s", hw);
            length += strlength;
            nerrors += CHECK(PageBuffer_length(pb) == length, 
                        "total length does not match expected value. "
                        "found: %zu, expected: %zu", 
                        PageBuffer_length(pb), length);
        }
    }
    PageBuffer_del(pb);

    TEST_END(verbosity, nerrors);
}

void test_PageBuffer_swrite(void) {
    TEST_START(verbosity);
    int nerrors = 0;

    char const * hw = "Hello, World";
    size_t strlength = strlen(hw);

    ptrdiff_t size = 40;
    PageBuffer * pb = PageBuffer_new(size);
    nerrors += CHECK(pb, "failed default new Buffer of size %lld", (long long)size);
    if (pb) {
        nerrors += CHECK(PageBuffer_size(pb) == (size_t)size, "did not initialize to default size. found: %zu, expected: %zu", PageBuffer_size(pb), (size_t)size);

        size_t length = 0;
        int nwrites = size / strlength * 8;
        for (int i = 0; i < nwrites; i++) {
            pbnprintf(&pb, "%s", hw);
            length += strlength;
            nerrors += CHECK(PageBuffer_length(pb) == length, 
                        "total length does not match expected value. "
                        "found: %zu, expected: %zu", 
                        PageBuffer_length(pb), length);
        }

        size_t string_size = 0;
        char * string = PageBuffer_swrite(pb, &string_size);
        nerrors += CHECK(string_size == strlen(string), 
                    "resulting string not the correct size. found: %zu, "
                    "expected: %zu", string_size, strlen(string));

        //printf("final string: %s", string);
        
        char * s = string;
        for (int i = 0; i < nwrites; i++) {
            nerrors += CHECK(!strncmp(s, hw, strlength),
                        "resulting string not matching at write %d. "
                        "found: %.*s, expected: %s", i + 1,
                        (int)strlength, s, hw);
            s += strlength;
        }
        free(string);
    }
    PageBuffer_del(pb);

    TEST_END(verbosity, nerrors);
}

int main(int narg, char ** args) {
    if (narg > 1 && !strcmp("--verbose", args[1])) {
        verbosity = 1;
    }

    test_PageBuffer_new();
    test_pbnprintf();
    test_PageBuffer_swrite();
    return 0;
}