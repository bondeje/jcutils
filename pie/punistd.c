#include <stdbool.h>
#include <string.h>
#include "punistd.h"

#ifndef _GNU_SOURCE

char * optarg = NULL;
int opterr = 0;
int optind = 1;
int optopt = 0;

int getopt_long(int argc, char * const * argv,
	const char * optstring,
	const struct option * longopts, int * longindex) {

	int getopt_result = getopt(argc, argv, optstring);
	return 0;
}

#ifdef _WIN32

int getopt(int argc, char * const * argv,
	const char * optstring) {
	static char * nextchar = argv[optind];
	static bool in_chain = false;
	int result = -1;
	static char const * errmsg = "";
	static char invalid_char = '\0';

	if (!nextchar) { // null pointer/end of argv
		errmsg = "end of argv";
		goto getopt_error_handling;
	}
	invalid_char = *nextchar;
	if (!in_chain) { // previous call was not already an option
		if ('-' != *nextchar) { // not an option
			errmsg = "non-option argument detected";
			goto getopt_error_handling;
		}
		// nextchar points to '-'
		nextchar++;
		if (!*nextchar) { // string "-"
			errmsg = "\"-\" string detected";
			goto getopt_error_handling;
		}
		invalid_char = *nextchar;

		if ('-' == *nextchar) { // "--" option termination
			optind++;
			errmsg = "\"--\" string detected. end of options";
			goto getopt_error_handling;
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
				switch (*nextchar) {
					case '\0': // last character in the string
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
						break;
					case '=':
						optarg = nextchar + 1;
						break;
					default:
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
getopt_error_handling:
	if (opterr && ':' != *optstring) {
		if (invalid_char) {
			fprintf(stderr, "%s: %s - invalid charactor: %c\n", argv[0], errmsg, invalid_char);
		} else {
			fprintf(stderr, "%s: %s\n", argv[0], errmsg);
		}
	}
	return '?';
}

#endif

#endif
