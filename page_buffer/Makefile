.POSIX:
CC = gcc
CFLAGS_COM = -Wall -Wextra -Werror -pedantic -std=c99
CFLAGS_DBG = $(CFLAGS_COM) -g2 -O0
CFLAGS = $(CFLAGS_COM) -O2

OBJS = page_buffer.o
HDRS = page_buffer.h

all: $(OBJS)

.MAIN: all

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

test: test.c
	@if [ -n "$(SANITIZE)" ] ; then export DBGOPT="-fsanitize=address,undefined"; else export DBGOPT="" ; fi ; \
	$(CC) $(CFLAGS_DBG) $$DBGOPT $< -o $@

check: all test
	./test --verbose

copy: all
	@if [ -n "$(PREFIX)" ] ; then cp $(OBJS) $(HDRS) $(PREFIX) ; fi

clean:
	rm -f test $(OBJS)
