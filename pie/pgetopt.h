#ifndef PGETOPT_H
#define PGETOPT_H

#include "punistd.h"

#ifdef _WIN32

struct option {
	const char * name;
	int has_arg;
	int * flag;
	int val;
};

int getopt_long(int narg, char * const * args, const char * optstring,
	const struct option * longopts, int * longindex);

#else

#include <getopt.h>

#endif

#endif
