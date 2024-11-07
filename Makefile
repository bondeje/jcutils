PageBuffer: PageBuffer/page_buffer.o
	(cd $@ ; make clean check copy SANITIZE=$(SANITIZE) PREFIX=$(PREFIX))

clean:
	rm -f *.o *.h *.so
	(cd PageBuffer ; make clean)