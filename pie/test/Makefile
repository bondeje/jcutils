# for now with MSVC, compile: cl /Zc:preprocessor punistd.c pgetopt.c test/test.c /Fetest.exe

CFLAGS_ = -Wall -Wextra -Werror -pedantic -O0 -g3 $(CFLAGS)
IFLAGS_ = -I.. $(IFLAGS)
LFLAGS_ = -Wl,-rpath=.. -L.. -lpie $(LFLAGS)

all: test$(EXE)

SRCS = test.c test_getopt.c

test$(EXE): $(SRCS)
	$(CC) $(IFLAGS_) $(CFLAGS_) -o test$(EXE) $(LFLAGS_) test.c

check: all
	./test$(EXE) --verbose

clean:
	rm -f test$(EXE)
