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
		"test_getopt",
		"-a",
		"-bc",
		"-b",
		"e",
		"-ab",
		"f",
		"-ac",
		"--",
		"test",
		NULL
	};


	int result = -1;
	narg = COUNTOF(args);

	result = getopt(narg, args, optstring);
	nerrors += CHECK(result == 'a', "error - found %c, expected %c\n", result, 'a');
	result = getopt(narg, args, optstring);
	nerrors += CHECK(result == 'b', "error - found %c, expected %c\n", result, 'b');
	nerrors += CHECK(!strcmp(optarg, "c"), "error - found optarg %s, expected %s\n", optarg, "c");
	result = getopt(narg, args, optstring);
	nerrors += CHECK(result == 'b', "error - found %c, expected %c\n", result, 'b');
	nerrors += CHECK(!strcmp(optarg, "e"), "error - found optarg %s, expected %s\n", optarg, "e");
	result = getopt(narg, args, optstring);
	nerrors += CHECK(result == 'a', "error - found %c, expected %c\n", result, 'a');
	result = getopt(narg, args, optstring);
	nerrors += CHECK(result == 'b', "error - found %c, expected %c\n", result, 'b');
	nerrors += CHECK(!strcmp(optarg, "f"), "error - found optarg %s, expected %s\n", optarg, "f");
	result = getopt(narg, args, optstring);
	nerrors += CHECK(result == 'a', "error - found %c, expected %c\n", result, 'a');
	result = getopt(narg, args, optstring);
	nerrors += CHECK(result == 'c', "error - found %c, expected %c\n", result, 'c');
	result = getopt(narg, args, optstring);
	nerrors += CHECK(-1 == result, "error - expected error, found %d\n", result);

	TEST_END(verbose, nerrors);
	return nerrors;
}



