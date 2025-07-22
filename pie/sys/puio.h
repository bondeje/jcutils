#ifndef PUIO_H
#define PUIO_H

#ifdef _WIN32

#include <sys/types.h>
struct iovec {
	void * iov_base;
	size_t iov_len;
};

#else

#include <sys/uio.h>

#endif

#endif
