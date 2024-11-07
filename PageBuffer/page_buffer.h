#ifndef PAGE_BUFFER_H
#define PAGE_BUFFER_H

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#ifdef _POSIX_VERSION
#include <sys/types.h>
#endif

typedef struct PageBuffer PageBuffer;

// if buffer_size == 0, sets to DEFAULT_BUFFER_SIZE
PageBuffer * PageBuffer_new(ptrdiff_t buffer_size);
// returns 0 on success, non-zero on failure. // wraps snprintf
int pbnprintf(PageBuffer * restrict * restrict pb, char const * restrict format, ...);
int vpbnprintf(PageBuffer * restrict * restrict pb, char const * restrict format, va_list args);
// returns cumulative effect of fwrite(...)
size_t PageBuffer_fwrite(PageBuffer * restrict pb, FILE * file);
#ifdef _POSIX_VERSION
// returns cumulative effect of write(...)
ssize_t PageBuffer_write(PageBuffer * restrict pb, int fd);
#endif
// total length of PageBuffer
size_t PageBuffer_length(PageBuffer * pb);
// returns null terminated string. result must be free'd
char * PageBuffer_swrite(PageBuffer * pb, size_t * length);
void PageBuffer_del(PageBuffer * pb);


#endif

