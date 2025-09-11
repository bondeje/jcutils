.POSIX:

all: page_buffer_ strings_ memory_pool_ fft_ allocators_

.MAIN: all

fft_:
	make -C fft clean check install

page_buffer_:
	make -C page_buffer clean check install

strings:
	make -C Strings clean check install

memory_pool_:
	make -C memory_pool clean check install speed

allocators_:
	make -C allocators clean check install

clean:
	rm -f *.o *.h *.so
	make -C page_buffer clean
	make -C Strings clean
	make -C memory_pool clean
	make -C fft clean
