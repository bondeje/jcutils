.POSIX:
CC = gcc

FLAGS = CC=$(CC) SANITIZE=$(SANITIZE) PREFIX=$(PREFIX)

all: page_buffer_ strings_ memory_pool_ fft_

.MAIN: all

fft_:
	(cd fft ; make clean check copy $(FLAGS))

page_buffer_:
	(cd page_buffer ; make clean check copy $(FLAGS))

strings_:
	(cd strings ; make clean check copy $(FLAGS))

memory_pool_:
	(cd memory_pool ; make clean check copy speed $(FLAGS))

clean:
	rm -f *.o *.h *.so
	(cd page_buffer ; make clean)
	(cd strings ; make clean)
	(cd memory_pool ; make clean)
	(cd fft ; make clean)