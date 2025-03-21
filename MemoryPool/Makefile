.POSIX:
CC = gcc
CFLAGS_COM = -Wall -Wextra -Werror -pedantic
CFLAGS_DBG = $(CFLAGS_COM) -std=c11 -g2 -O0
CFLAGS = $(CFLAGS_COM) -std=c99 -O2

OBJS = mempool.o
HDRS = mempool.h

all: $(OBJS)

.MAIN: all

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

test: test.c
	@if [ -n "$(SANITIZE)" ] ; then export DBGOPT="-fsanitize=address,undefined"; else export DBGOPT="" ; fi ; \
	$(CC) $(CFLAGS_DBG) $$DBGOPT $< -o $@

speed: $(OBJS) test_speed.c
	$(CC) $(CFLAGS_DBG) -Wno-unused-variable $(OBJS) test_speed.c -o test_speed && \
	./test_speed

check: all test
	./test --verbose

copy: all
	@if [ -n "$(PREFIX)" ] ; then cp $(OBJS) $(HDRS) $(PREFIX) ; fi

clean:
	rm -f test $(OBJS)
