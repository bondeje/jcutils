#ifndef PUNISTD_H
#define PUNISTD_H

#include "pgetopt.h"

#ifdef _WIN32

#else

#include <unistd.h>

#endif

#ifndef _GNU_SOURCE

extern char * optarg;
extern int opterr;
extern int optind;
extern int optopt;

int getopt_long(int argc, char * const argv[], const char * optstring,
	const struct option * longopts, int * longindex);

#ifdef _WIN32

int getopt(int argc, char * const * argv, const char * optstring);

#endif /* _WIN32 */

#endif /* _GNU_SOURCE */

#endif /* PUNISTD_H */
