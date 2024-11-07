#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "page_buffer.h"

#ifndef DEFAULT_BUFFER_SIZE
#if defined(_POSIX_VERSION) || (defined(_XOPEN_SOURCE) && _XOPEN_SOURXE > 500)
	#include <limits.h>
	#define DEFAULT_BUFFER_SIZE PAGE_SIZE
#else
	#define DEFAULT_BUFFER_SIZE 4096
#endif
#endif

#define PAGE_BUFFER(pb) ((pb)->end - (pb)->buffer_size)
#define PAGE_BUFFER_SIZE(pb) ((size_t)((pb)->end - (pb)->buffer))
#define PAGE_BUFFER_SIZE_MAX(pb) ((size_t)((pb)->buffer_size))
#define PAGE_BUFFER_WRITE_SIZE(pb) ((size_t)((pb)->buffer - ((pb)->end - (pb)->buffer_size)))

struct PageBuffer {
	struct PageBuffer * last;
	char * end;
	char * buffer;
	ptrdiff_t buffer_size;
};

PageBuffer * PageBuffer_new_(PageBuffer * last, ptrdiff_t buffer_size) {
	if (0 >= buffer_size) {
		buffer_size = DEFAULT_BUFFER_SIZE - sizeof(PageBuffer);
	}
	PageBuffer * new = malloc(buffer_size + sizeof(PageBuffer));
	if (!new) {
		return new;
	}
	new->buffer = ((char *)new) + sizeof(PageBuffer);
	new->buffer_size = buffer_size;
	new->end = new->buffer + buffer_size;
	new->last = last;
	return new;
}

PageBuffer * PageBuffer_new(ptrdiff_t buffer_size) {
	return PageBuffer_new_(NULL, buffer_size);
}

void pb_backfill(PageBuffer * top) {
	PageBuffer * dest = top->last;

	if (dest->end == dest->buffer) {
		return;
	}
	size_t n = (dest->end - dest->buffer);
	if (!n) {
		return;
	}
	char * src = top->end - top->buffer_size;
	memcpy(dest->buffer, src, n);
	dest->buffer += n; 			// move dest->buffer n bytes later
	memmove(src, src + n, PAGE_BUFFER_WRITE_SIZE(top) - n); 	// move string already written to top back n bytes
	top->buffer -= n;			// mark buffer n bytes earlier
}

// returns -1 on irrecoverable error, else the number of bytes written or would have been written
// excluding the null-terminator
int vpbnprintf_(PageBuffer * restrict pb, char const * restrict format, va_list args) {
	ptrdiff_t n = PAGE_BUFFER_SIZE(pb);
	int nwritten = vsnprintf(pb->buffer, n, format, args);
	if (nwritten >= 0 && nwritten < n) {
		pb->buffer += nwritten;
	} else if ((unsigned int)nwritten >= PAGE_BUFFER_SIZE_MAX(pb)) { // so that pbnprintf doesn't try again
		return -1;
	}
	return nwritten;
}

int vpbnprintf(PageBuffer * restrict * restrict pb, char const * restrict format, va_list args) {
	va_list argsc;
	va_copy(argsc, args);
	size_t buf_size = PAGE_BUFFER_SIZE(*pb);
	int nwritten = vpbnprintf_(*pb, format, args);
	int status = 0;
	if (nwritten < 0) {
		status = nwritten;
	} else if ((unsigned int)nwritten >= buf_size) {
		PageBuffer * new = PageBuffer_new_(*pb, PAGE_BUFFER_SIZE_MAX(*pb));
		if (!new) {
			status = -1;
		} else {
			*pb = new;
			status = vpbnprintf_(new, format, argsc);
			if (0 <= status) {
				pb_backfill(new);
			}
		}
	}
	va_end(argsc);
	return status; 
}

int pbnprintf(PageBuffer * restrict * restrict pb, char const * restrict format, ...) {
	va_list args;
	va_start(args, format);
	int status = vpbnprintf(pb, format, args);
	va_end(args);
	return status;
}

size_t PageBuffer_fwrite(PageBuffer * restrict pb, FILE * stream) {
	size_t out = 0;
	while (pb) {
		out += fwrite(PAGE_BUFFER(pb), 1, PAGE_BUFFER_WRITE_SIZE(pb), stream);
		pb = pb->last;
	}
	return out;
}

#ifdef _POSIX_VERSION
ssize_t PageBuffer_write(PageBuffer * restrict pb, int fd) {
	ssize_t out = 0;
	while (pb) {
		ssize_t status = write(fd, PAGE_BUFFER(pb), PAGE_BUFFER_WRITE_SIZE(pb));
		if (0 > status) {
			return -1;
		}
		pb = pb->last;
		out += status;
	}
	return out;
}
#endif

size_t PageBuffer_length(PageBuffer * pb) {
	size_t length = 0;
	while (pb) {
		length += PAGE_BUFFER_WRITE_SIZE(pb);
		pb = pb->last;
	}
	return length;
}

char * PageBuffer_swrite_(PageBuffer * pb, char * buffer) {
	if (!pb) {
		return buffer;
	}
	buffer = PageBuffer_swrite_(pb->last, buffer);
	memcpy(buffer, PAGE_BUFFER(pb), PAGE_BUFFER_WRITE_SIZE(pb));
	return buffer + PAGE_BUFFER_WRITE_SIZE(pb);
}

// result must be free'd
char * PageBuffer_swrite(PageBuffer * pb, size_t * length) {
	*length = PageBuffer_length(pb);
	char * buffer = calloc(*length + 1, sizeof(char));
	if (!buffer) {
		return buffer;
	}
	PageBuffer_swrite_(pb, buffer);
	buffer[*length] = '\0';
	return buffer;
}

void PageBuffer_del(PageBuffer * pb) {
	PageBuffer * next = pb->last;
	free(pb);
	if (next) {
		PageBuffer_del(next);
	}
}

/* for testing purposes */
size_t PageBuffer_size(PageBuffer * pb) {
	return PAGE_BUFFER_SIZE(pb);
}

size_t PageBuffer_write_size(PageBuffer * pb) {
	return PAGE_BUFFER_WRITE_SIZE(pb);
}

size_t PageBuffer_size_max(PageBuffer * pb) {
	return PAGE_BUFFER_SIZE_MAX(pb);
}

size_t PageBuffer_get_default_size(void) {
	return DEFAULT_BUFFER_SIZE;
}
