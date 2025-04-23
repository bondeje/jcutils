CC = gcc
STDC = -std=c99
CFLAGS_ = -Wall -Wextra -pedantic -Werror $(CFLAGS) $(STDC)
IFLAGS += -I..

OBJS = system_allocator.o

all: $(OBJS)

.MAIN: all

.c.o:
	$(CC) $(IFLAGS) $(CFLAGS_) -c $< -o $@

test: test.c
	$(CC) $(IFLAGS) $(CFLAGS_) $< -o $@

check: test
	./test --verbose

install: all
	if [ -n "$(PREFIX)" ] ; then cp $(OBJS) $(HDRS) $(PREFIX) ; fi

clean:
	rm -f test *.o