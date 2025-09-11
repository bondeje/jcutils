.POSIX:
CC = gcc
CFLAGS_COM = -Wall -Wextra -Werror -pedantic -std=gnu99 -fcx-limited-range -Wno-unused-parameter $(CFLAGS)
CFLAGS_DBG = $(CFLAGS_COM) -g3 -O0
IFLAGS = -I..
CFLAGS_ = $(CFLAGS_COM) -O2
LFLAGS = -lm
PREFIX = /usr/local/jcutils/fft

OBJS = fft.o
HDRS = fft.h

all: $(OBJS)

.MAIN: all

.c.o:
	$(CC) $(IFLAGS) $(CFLAGS_) -c $< -o $@

grind: grind.c
	$(CC) $(IFLAGS) $(CFLAGS_) -g3 $< -o $@ $(LFLAGS)
	valgrind --tool=cachegrind --cachegrind-out-file=cachegrind.out ./grind
	cg_annotate cachegrind.out > cachegrind.annotate

test: test.c
	$(CC) $(IFLAGS) $(CFLAGS_DBG) $< -o $@ $(LFLAGS)

posix_time: posix_time.c
	$(CC) $(IFLAGS) $(CFLAGS_) $< -o $@ $(LFLAGS)
	./posix_time

check: test
	./test --verbose

install: all
	if [ -n "$(PREFIX)" ] ; then \
		install $(HDRS) $(PREFIX)/include ; \
		install $(OBJS) $(PREFIX)/lib ; \
	fi

clean:
	rm -f test grind *.out *.annotate posix_time $(OBJS)
