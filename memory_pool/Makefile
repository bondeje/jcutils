.POSIX:
CC = gcc
CFLAGS_COM = -Wall -Wextra -Werror -pedantic $(CFLAGS)
CFLAGS_DBG = $(CFLAGS_COM) -std=c11 -g2 -O0
CFLAGS_ = $(CFLAGS_COM) -std=c99 -O2
IFLAGS = -I..

OBJS = mempool.o
HDRS = mempool.h

all: $(OBJS)

.MAIN: all

.c.o:
	$(CC) $(IFLAGS) $(CFLAGS_) -c $< -o $@

test: test.c
	$(CC) $(IFLAGS) $(CFLAGS_DBG) $< -o $@

speed: $(OBJS) test_speed.c
	$(CC) $(IFLAGS) $(CFLAGS_DBG) -Wno-unused-variable $(OBJS) test_speed.c -o test_speed && \
	./test_speed

check: test
	./test --verbose

install: all
	if [ -n "$(PREFIX)" ] ; then cp $(OBJS) $(HDRS) $(PREFIX) ; fi

clean:
	rm -f test testspeed $(OBJS)
