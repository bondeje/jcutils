#include "getopt.h"

#ifdef _WIN32

#include <stdbool.h>
#include <string.h>

int getopt_long(int argc, char * const * argv,
	const char * optstring,
	const struct option * longopts, int * longindex) {
	static bool in_chain = false;
	static char * nextchar = NULL;
	static char const * errmsg = "";
	static char invalid_char = '\0';
	if (1 == optind) { // a reset is detected
		nextchar = argv[optind];
		in_chain = false;
	}
	int result = -1;

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

		if ('-' == *(nextchar++)) { 
			if (!*nextchar) { // "--" option termination or longopts
				optind++;
				errmsg = "\"--\" string detected. end of options";
				goto getopt_error_handling;
			} else if (!longopts) {
				errmsg = "long option found but no longopts provided";
				goto getopt_error_handling;
			} else { // handling longopts
				int index = 0;
				while (longopts[index]->name) {
					size_t namelen = strlen(longopts[index]->name);
					if (!strncmp(nextchar, longopts[index]->name, namelen)) { // longopt matches
						switch (*(nextchar + namelen)) {
							case '\0': // matches without "=option-argument"
							case '=': // --longopt=option-argument
							default: // not actually a match
						}
					}
				}
			}
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
	errmsg = "no matching option found";
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

#else

#endif
