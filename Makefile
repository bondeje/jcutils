.POSIX:
CC = gcc

FLAGS = CC=$(CC) SANITIZE=$(SANITIZE) PREFIX=$(PREFIX)

all: page_buffer strings_ memory_pool

.MAIN: all

page_buffer:
	(cd PageBuffer ; make clean check copy $(FLAGS))

strings_:
	(cd Strings ; make clean check copy $(FLAGS))

memory_pool:
	(cd MemoryPool ; make clean check copy speed $(FLAGS))

clean:
	rm -f *.o *.h *.so
	(cd PageBuffer ; make clean)
	(cd Strings ; make clean)
	(cd MemoryPool ; make clean)