IFLAGS_ = -I. $(IFLAGS)
CFLAGS_ = -Wall -Wextra $(CFLAGS)
LFLAGS_ = $(LFLAGS)
PREFIX=.

OBJS=punistd.o

all: libpie.so
	make -C test all

libpie.so: $(OBJS)
	$(CC) $(IFLAGS_) $(CFLAGS_) -shared -o libpie.so $(LFLAGS_) $(OBJS)

.c.o:
	$(CC) $(IFLAGS_) $(CFLAGS_) -c -o $@ $<

lflags:
	@echo $(LFLAGS_)

cflags:
	@echo $(IFLAGS_) $(CFLAGS_)

check: all
	make -C test check

clean:
	rm -rf $(OBJS)
	make -C test clean
