#ifndef PTYPES_H
#define PTYPES_H

#ifdef _WIN32

#include <stddef.h>
typedef long long int ssize_t;

#else

#include <sys/types.h>

#endif

#endif
