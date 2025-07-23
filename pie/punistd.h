#ifndef PUNISTD_H
#define PUNISTD_H

#ifdef _WIN32

extern char * optarg;
extern int opterr;
extern int optind;
extern int optopt;

int getopt(int argc, char * const * argv, const char * optstring);

#else

#include <unistd.h>

#endif

#endif /* PUNISTD_H */
