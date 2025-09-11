.POSIX:
PREFIX = $$PWD

all: paths page_buffer_ strings_ memory_pool_ fft_ allocators_

.MAIN: all

paths:
	mkdir -p include lib bin

fft_: paths
	make -C fft clean check install PREFIX=$(PREFIX)

page_buffer_: paths
	make -C page_buffer clean check install PREFIX=$(PREFIX)

strings: paths
	make -C Strings clean check install PREFIX=$(PREFIX)

memory_pool_: paths
	make -C memory_pool clean check install speed PREFIX=$(PREFIX)

allocators_: paths
	make -C allocators clean check install PREFIX=$(PREFIX)

clean:
	rm -f *.o *.h *.so
	rm -rf include lib bin
	make -C page_buffer clean
	make -C Strings clean
	make -C memory_pool clean
	make -C fft clean
