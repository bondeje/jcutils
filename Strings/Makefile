.POSIX:
CC = gcc
STDC = -std=c99
CFLAGS_COM = -Wall -Wextra -Werror -pedantic $(STDC) $(CFLAGS)
CFLAGS_DBG = $(CFLAGS_COM) -g2 -O0
CFLAGS_ = $(CFLAGS_COM) -O2

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
	if [ -n "$(PREFIX)" ] ; then cp $(OBJS) $(HDRS) $(PREFIX) ; fi

clean:
	rm -f test $(OBJS)
