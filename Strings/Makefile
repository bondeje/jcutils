.POSIX:
CC = gcc
STDC = -std=c99
CFLAGS_COM = -Wall -Wextra -Werror -pedantic $(STDC) $(CFLAGS)
CFLAGS_DBG = $(CFLAGS_COM) -g3 -O0
CFLAGS_ = $(CFLAGS_COM) -O2
PREFIX = /usr/local/jcutils/Strings

OBJS = strings.o
HDRS = strings.h

all: $(OBJS)

.MAIN: all

.c.o:
	$(CC) $(CFLAGS_) -c $< -o $@

test: test.c
	$(CC) $(CFLAGS_DBG) $< -o $@

check: test
	./test --verbose

install: all
	install $(HDRS) $(PREFIX)/include ; \
	install $(OBJS) $(PREFIX)/lib ; \

clean:
	rm -f test $(OBJS)
