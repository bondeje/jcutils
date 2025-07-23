#include <string.h>
#include <stdio.h>

int verbose = 0;

#include "test_getopt.c"

int main(int narg, char ** args) {
	if (narg > 1 && !strcmp(args[1], "--verbose")) {
		verbose = 1;
	}

	int nerrors = 0;
	nerrors += test_getopt();
	nerrors += test_getopt_long();

	return nerrors;
}
