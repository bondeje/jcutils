CFLAGS_ = -Wall -Wextra -Werror -pedantic -O0 -g3 $(CFLAGS)
IFLAGS_ = -I.. $(IFLAGS)
LFLAGS_ = -Wl,-rpath=.. -L.. -lpie $(LFLAGS)

all: test

SRCS = test.c test_getopt.c

test: $(SRCS)
	$(CC) $(IFLAGS_) $(CFLAGS_) -o test $(LFLAGS_) test.c

check: all
	./test --verbose

clean:
	rm -f test
