#include "pgetopt.h"

#if defined(_WIN32) && !defined(__MINGW32__)

#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define no_argument 0
#define required_argument 1
#define optional_argument 2

struct getopt_long_error {
	char * errmsg;
	union {
		char invalid_char;
		char * invalid_option;
	} bad_value;
	int result;
};

int getopt_long(int argc, char * const * argv,
	const char * optstring,
	const struct option * longopts, int * longindex) {
	static bool in_chain = false;
	static char * nextchar = NULL;
	static char const * errmsg = "";
	if (1 == optind && !in_chain) { // a reset is detected
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
			return result;
		}

		if ('-' == *nextchar) { // arg starts with "--"
			nextchar++;
			if (!*nextchar) { // "--" option termination
				optind++;
				return result;
			} else if (!longopts) { // long option but no longopts
				errmsg = "long option found but no longopts provided";
				goto getopt_error_handling;
			} else { // handling longopts
				int index = 0;
				while (longopts[index].name) {
					size_t namelen = strlen(longopts[index].name);
					if (!strncmp(nextchar, longopts[index].name, namelen)) { // possibly matches
						nextchar += namelen;
						if (*nextchar != '\0' && !('=' == *nextchar && longopts[index].has_arg != no_argument)) {
							// not actually a match
							index++;
							continue; 
						}
						optind++; // no matter what, optind is advancing at least 1
						switch (longopts[index].has_arg) {
							case no_argument: {
								if ('\0' != *nextchar) {
									errmsg = "option does not taken an argument";
									goto getopt_error_handling;
								}
								optarg = NULL;
								break;
							}
							case required_argument: {
								if ('\0' == *nextchar) {
									if (optind >= argc) {
										errmsg = "missing required argument";
										goto getopt_error_handling;
									}
									optarg = argv[optind++];
								} else if ('=' == *nextchar) {
									optarg = ++nextchar;
								} // other case is already handled
								break;
							}
							case optional_argument: {
								// if end of arg and not followed by a potential option
								if ('\0' == *nextchar && (optind >= argc || '-' != *(argv[optind]))) {
									if (optind >= argc) {
										optarg = NULL;
									} else {
										optarg = argv[optind++];
									}
								} else if ('=' == *nextchar) {
									optarg = ++nextchar;
								} else {
									optarg = NULL;
								}
								break;
							}
							default: {
								errmsg = "option has invalid has_arg parameter";
								goto getopt_error_handling;
							}
						}
						if (longopts[index].flag) {
							*(longopts[index].flag) = longopts[index].val;
							result = 0;
						} else {
							result = longopts[index].val;
						}
						nextchar = argv[optind];
						if (longindex) {
							*longindex = index;
						}
						return result;
					}
					index++;
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
}

#else

#endif
