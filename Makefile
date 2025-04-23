.POSIX:
CC = gcc

FLAGS = CC=$(CC) SANITIZE=$(SANITIZE) PREFIX=$(PREFIX)

all: page_buffer_ strings_ memory_pool_ fft_ allocators_

.MAIN: all

fft_:
	(cd fft ; make clean check install $(FLAGS))

page_buffer_:
	(cd page_buffer ; make clean check install $(FLAGS))

strings_:
	(cd strings ; make clean check install $(FLAGS))

memory_pool_:
	(cd memory_pool ; make clean check install speed $(FLAGS))

allocators_:
	(cd allocators ; make clean check install $(FLAGS))

clean:
	rm -f *.o *.h *.so
	(cd page_buffer ; make clean)
	(cd strings ; make clean)
	(cd memory_pool ; make clean)
	(cd fft ; make clean)