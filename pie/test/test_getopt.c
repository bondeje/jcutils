#include "../punistd.h"
#include "../../test/utils.h"

int test_getopt(void) {
	TEST_START(verbose);
	int nerrors = 0;
	int narg = 0;
	char const * optstring = "ab:c";

	optind = 1;
	opterr = 1;
	char * args[] = {
		"test_getopt",  // 0
		"-a",
		"-bc",
		"-b",
		"e",
		"-ab",		// 5
		"f",
		"-ac",
		"--",
		"test",
		NULL		// 10
	};


	int result = -1;
	narg = COUNTOF(args);

	result = getopt(narg, args, optstring);
	nerrors += CHECK('a' == result, "error - found %c, expected %c\n", result, 'a');
	result = getopt(narg, args, optstring);
	nerrors += CHECK('b' == result, "error - found %c, expected %c\n", result, 'b');
	nerrors += CHECK(!strcmp(optarg, "c"), "error - found optarg %s, expected %s\n", optarg, "c");
	result = getopt(narg, args, optstring);
	nerrors += CHECK('b' == result, "error - found %c, expected %c\n", result, 'b');
	nerrors += CHECK(!strcmp(optarg, "e"), "error - found optarg %s, expected %s\n", optarg, "e");
	result = getopt(narg, args, optstring);
	nerrors += CHECK('a' == result, "error - found %c, expected %c\n", result, 'a');
	result = getopt(narg, args, optstring);
	nerrors += CHECK('b' == result, "error - found %c, expected %c\n", result, 'b');
	nerrors += CHECK(!strcmp(optarg, "f"), "error - found optarg %s, expected %s\n", optarg, "f");
	result = getopt(narg, args, optstring);
	nerrors += CHECK('a' == result, "error - found %c, expected %c\n", result, 'a');
	result = getopt(narg, args, optstring);
	nerrors += CHECK('c' == result, "error - found %c, expected %c\n", result, 'c');
	result = getopt(narg, args, optstring);
	nerrors += CHECK(-1 == result, "error - expected error, found %d\n", result);
	nerrors += CHECK(!strcmp("test", args[optind]), "error - expected optind to be at \"test\", but is at %s\n", args[optind]);

	// change explicit "--" to a non-option
	args[8] = "first non-option";
	// reset 
	optind = 1;

	result = getopt(narg, args, optstring);
	nerrors += CHECK('a' == result, "error - found %c, expected %c\n", result, 'a');
	result = getopt(narg, args, optstring);
	nerrors += CHECK('b' == result, "error - found %c, expected %c\n", result, 'b');
	nerrors += CHECK(!strcmp(optarg, "c"), "error - found optarg %s, expected %s\n", optarg, "c");
	result = getopt(narg, args, optstring);
	nerrors += CHECK('b' == result, "error - found %c, expected %c\n", result, 'b');
	nerrors += CHECK(!strcmp(optarg, "e"), "error - found optarg %s, expected %s\n", optarg, "e");
	result = getopt(narg, args, optstring);
	nerrors += CHECK('a' == result, "error - found %c, expected %c\n", result, 'a');
	result = getopt(narg, args, optstring);
	nerrors += CHECK('b' == result, "error - found %c, expected %c\n", result, 'b');
	nerrors += CHECK(!strcmp(optarg, "f"), "error - found optarg %s, expected %s\n", optarg, "f");
	result = getopt(narg, args, optstring);
	nerrors += CHECK('a' == result, "error - found %c, expected %c\n", result, 'a');
	result = getopt(narg, args, optstring);
	nerrors += CHECK('c' == result, "error - found %c, expected %c\n", result, 'c');
	result = getopt(narg, args, optstring);
	nerrors += CHECK(-1 == result, "error - expected error, found %d\n", result);
	nerrors += CHECK(!strcmp(args[8], args[optind]), "error - expected optind to point to %s but instead is at %s\n", args[8], args[optind]);

	// change explicit "--" to an unknown option
	args[8] = "-d";
	// reset 
	optind = 1;

	result = getopt(narg, args, optstring);
	nerrors += CHECK('a' == result, "error - found %c, expected %c\n", result, 'a');
	result = getopt(narg, args, optstring);
	nerrors += CHECK('b' == result, "error - found %c, expected %c\n", result, 'b');
	nerrors += CHECK(!strcmp(optarg, "c"), "error - found optarg %s, expected %s\n", optarg, "c");
	result = getopt(narg, args, optstring);
	nerrors += CHECK('b' == result, "error - found %c, expected %c\n", result, 'b');
	nerrors += CHECK(!strcmp(optarg, "e"), "error - found optarg %s, expected %s\n", optarg, "e");
	result = getopt(narg, args, optstring);
	nerrors += CHECK('a' == result, "error - found %c, expected %c\n", result, 'a');
	result = getopt(narg, args, optstring);
	nerrors += CHECK('b' == result, "error - found %c, expected %c\n", result, 'b');
	nerrors += CHECK(!strcmp(optarg, "f"), "error - found optarg %s, expected %s\n", optarg, "f");
	result = getopt(narg, args, optstring);
	nerrors += CHECK('a' == result, "error - found %c, expected %c\n", result, 'a');
	result = getopt(narg, args, optstring);
	nerrors += CHECK('c' == result, "error - found %c, expected %c\n", result, 'c');
	result = getopt(narg, args, optstring);
	nerrors += CHECK('?' == result, "error - expected error, found %d\n", result);
	// in this case, the status of optind is unspecified. An error has occurred
	// nerrors += CHECK(!strcmp(args[8], args[optind]), "error - expected optind to point to %s but instead is at %s\n", args[8], args[optind]);

	// CHECKING THE EXAMPLES ON opengroup.org

	optstring = ":abf:o:";
	args[1] = "-ao";
	args[2] = "arg";
	args[3] = "path";
	args[4] = "path";
	args[5] = "path";
	// reset
	optind = 1;

	result = getopt(narg, args, optstring);
	nerrors += CHECK('a' == result, "error - found %c, expected %c\n", result, 'a');
	result = getopt(narg, args, optstring);
	nerrors += CHECK('o' == result, "error - found %c, expected %c\n", result, 'o');
	nerrors += CHECK(!strcmp(optarg, "arg"), "error - found optarg %s, expected %s\n", optarg, "arg");
	result = getopt(narg, args, optstring);
	nerrors += CHECK(-1 == result, "error - expected error, found %d\n", result);
	nerrors += CHECK(3 == optind, "error - expected optind to point to %s but instead is at %s\n", args[3], args[optind]);

	args[1] = "-a";
	args[2] = "-o";
	args[3] = "arg";
	// reset
	optind = 1;

	result = getopt(narg, args, optstring);
	nerrors += CHECK('a' == result, "error - found %c, expected %c\n", result, 'a');
	result = getopt(narg, args, optstring);
	nerrors += CHECK('o' == result, "error - found %c, expected %c\n", result, 'o');
	nerrors += CHECK(!strcmp(optarg, "arg"), "error - found optarg %s, expected %s\n", optarg, "arg");
	result = getopt(narg, args, optstring);
	nerrors += CHECK(-1 == result, "error - expected error, found %d\n", result);
	nerrors += CHECK(4 == optind, "error - expected optind to point to %s but instead is at %s\n", args[4], args[optind]);

	args[1] = "-o";
	args[2] = "arg";
	args[3] = "-a";
	// reset
	optind = 1;

	result = getopt(narg, args, optstring);
	nerrors += CHECK('o' == result, "error - found %c, expected %c\n", result, 'o');
	nerrors += CHECK(!strcmp(optarg, "arg"), "error - found optarg %s, expected %s\n", optarg, "arg");
	result = getopt(narg, args, optstring);
	nerrors += CHECK('a' == result, "error - found %c, expected %c\n", result, 'a');
	result = getopt(narg, args, optstring);
	nerrors += CHECK(-1 == result, "error - expected error, found %d\n", result);
	nerrors += CHECK(4 == optind, "error - expected optind to point to %s but instead is at %s\n", args[4], args[optind]);

	args[1] = "-a";
	args[2] = "-o";
	args[3] = "arg";
	args[4] = "--";
	// reset
	optind = 1;

	result = getopt(narg, args, optstring);
	nerrors += CHECK('a' == result, "error - found %c, expected %c\n", result, 'a');
	result = getopt(narg, args, optstring);
	nerrors += CHECK('o' == result, "error - found %c, expected %c\n", result, 'o');
	nerrors += CHECK(!strcmp(optarg, "arg"), "error - found optarg %s, expected %s\n", optarg, "arg");
	result = getopt(narg, args, optstring);
	nerrors += CHECK(-1 == result, "error - expected error, found %d\n", result);
	nerrors += CHECK(5 == optind, "error - expected optind to point to %s but instead is at %s\n", args[5], args[optind]);

	args[1] = "-a";
	args[2] = "-oarg";
	args[3] = "path";
	args[4] = "path";
	// reset
	optind = 1;

	result = getopt(narg, args, optstring);
	nerrors += CHECK('a' == result, "error - found %c, expected %c\n", result, 'a');
	result = getopt(narg, args, optstring);
	nerrors += CHECK('o' == result, "error - found %c, expected %c\n", result, 'o');
	nerrors += CHECK(!strcmp(optarg, "arg"), "error - found optarg %s, expected %s\n", optarg, "arg");
	result = getopt(narg, args, optstring);
	nerrors += CHECK(-1 == result, "error - expected error, found %d\n", result);
	nerrors += CHECK(3 == optind, "error - expected optind to point to %s but instead is at %s\n", args[3], args[optind]);

	args[1] = "-aoarg";
	args[2] = "path";
	// reset
	optind = 1;

	result = getopt(narg, args, optstring);
	nerrors += CHECK('a' == result, "error - found %c, expected %c\n", result, 'a');
	result = getopt(narg, args, optstring);
	nerrors += CHECK('o' == result, "error - found %c, expected %c\n", result, 'o');
	nerrors += CHECK(!strcmp(optarg, "arg"), "error - found optarg %s, expected %s\n", optarg, "arg");
	result = getopt(narg, args, optstring);
	nerrors += CHECK(-1 == result, "error - expected error, found %d\n", result);
	nerrors += CHECK(2 == optind, "error - expected optind to point to %s but instead is at %s\n", args[2], args[optind]);

	TEST_END(verbose, nerrors);
	return nerrors;
}



