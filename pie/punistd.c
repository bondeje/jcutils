#include "punistd.h"

#if defined(_WIN32) && !defined(__MINGW32__)


#include <stddef.h>
#include "pgetopt.h"

char * optarg = NULL;
int opterr = 0;
int optind = 1;
int optopt = 0;

int getopt(int argc, char * const * argv,
	const char * optstring) {

	return getopt_long(argc, argv, optstring, NULL, NULL);
}


/* TODO: delete
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

char * optarg = NULL;
int opterr = 0;
int optind = 1;
int optopt = 0;

int getopt(int argc, char * const * argv,
	const char * optstring) {

	static char * nextchar = NULL;
	static bool in_chain = false;
	static char const * errmsg = "";
	// it optind is 1 and we aren't in the first argument
	if (1 == optind && !in_chain) {
		nextchar = argv[optind];
	}
	int result = -1;

	if (!nextchar) { // null pointer/end of argv
		errmsg = "end of argv";
		goto getopt_error_handling;
	}
	if (!in_chain) { // previous call was not already an option
		if ('-' != *nextchar) { // not an option
			return result;
		}
		// nextchar points to '-'
		nextchar++;
		if (!*nextchar) { // string "-"
			return -1;
		}
		if ('-' == *nextchar) { // "--" option termination
			optind++;
			return result;
		}
		in_chain = true;
	}

	char const * optchar = optstring;
	while (*optchar) {
		if (':' == *optchar) {
			optchar++;
			continue;
		}
		if (*optchar == *nextchar) { // character matches an option
			result = *(nextchar++);
			if (*(optchar + 1) == ':') { // option requires an argument
				in_chain = false; // regardless, this is the end of the option chain
				optind++;
				if ('\0' == *nextchar) {
					if (optind >= argc) { // missing option-argument
						optopt = result;
						if (':' == *optstring) {
							result = ':';
						}
						result = '?';
						errmsg = "missing option-argument; end of argv";
						goto getopt_error_handling;
					}
					optarg = argv[optind++];
				} else {
					optarg = nextchar;
				}
				nextchar = argv[optind];
			} else { // option does not require an argument
				if (!*nextchar) { // chain does not continue
					in_chain = false;
					nextchar = argv[++optind];
				} // chain does not continue, but nextchar already set
			}
			return result;
		}
		optchar++;
	}
	errmsg = "unknown option";
	in_chain = false;
	optopt = *nextchar;
	result = '?';
getopt_error_handling:
	if (opterr && ':' != *optstring) {
		if (optopt) {
			fprintf(stderr, "%s: %s -- %c\n", argv[0], errmsg, optopt);
		} else {
			fprintf(stderr, "%s: %s\n", argv[0], errmsg);
		}
	}
	return result;
}*/

#else

#endif
