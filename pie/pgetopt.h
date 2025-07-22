#ifndef PGETOPT_H
#define PGETOPT_H

#ifdef _GNU_SOURCE

#include <getopt.h>

#else

struct option {
	const char * name;
	int has_arg;
	int * flag;
	int val;
};

#endif

#endif
