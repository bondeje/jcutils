.POSIX:
CC = gcc
CFLAGS_COM = -Wall -Wextra -Werror -pedantic -std=gnu99 -fcx-limited-range -Wno-unused-parameter
CFLAGS_DBG = $(CFLAGS_COM) -g3 -O0
CFLAGS = $(CFLAGS_COM) -O2
LFLAGS = -lm

OBJS = fft.o
HDRS = fft.h

all: $(OBJS)

.MAIN: all

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

grind: grind.c
	$(CC) $(CFLAGS) -g3 $< -o $@ $(LFLAGS)
	valgrind --tool=cachegrind --cachegrind-out-file=cachegrind.out ./grind
	cg_annotate cachegrind.out > cachegrind.annotate

test: test.c
	@if [ -n "$(SANITIZE)" ] ; then export DBGOPT="-fsanitize=address,undefined"; else export DBGOPT="" ; fi ;\
	$(CC) $(CFLAGS_DBG) $$DBGOPT $< -o $@ $(LFLAGS)

posix_time: posix_time.c
	$(CC) $(CFLAGS) $< -o $@ $(LFLAGS)
	./posix_time

check: all test
	./test

copy: all
	@if [ -n "$(PREFIX)" ] ; then cp $(OBJS) $(HDRS) $(PREFIX) ; fi

clean:
	rm -f test grind *.out *.annotate posix_time $(OBJS)
