IFLAGS_ = -I. $(IFLAGS)
CFLAGS_ = -Wall -Wextra $(CFLAGS)
LFLAGS_ = $(LFLAGS)
PREFIX=.
SO=so
OBJ=o
EXE=

PREFIX=/usr/local/jcutils/pie

OBJS=punistd.$(OBJ) pgetopt.$(OBJ)

all: libpie.$(SO)
	make -C test all

libpie.$(SO): $(OBJS)
	$(CC) $(IFLAGS_) $(CFLAGS_) -shared -o libpie.$(SO) $(LFLAGS_) $(OBJS)

.SUFFIXES: .c .$(OBJ)

.c.$(OBJ):
	$(CC) $(IFLAGS_) $(CFLAGS_) -c -o $@ $<

lflags:
	@echo $(LFLAGS_)

cflags:
	@echo $(IFLAGS_) $(CFLAGS_)

check: all
	make -C test check

# this is incomplete
install: all
	install pgetopt.o $(PREFIX)/lib
	install pgetopt.h $(PREFIX)/include

clean:
	rm -rf *.$(SO)
	rm -rf $(OBJS)
	make -C test clean
