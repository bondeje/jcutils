CC = gcc

FLAGS = CC=$(CC) SANITIZE=$(SANITIZE) PREFIX=$(PREFIX)

all: PageBuffer Strings

PageBuffer: PageBuffer/page_buffer.o
	(cd $@ ; make clean check copy $(FLAGS))

Strings: Strings/strings.o
	(cd $@ ; make clean check copy $(FLAGS))

clean:
	rm -f *.o *.h *.so
	(cd PageBuffer ; make clean)
	(cd Strings ; make clean)