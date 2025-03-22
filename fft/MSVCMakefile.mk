.POSIX:
CC = cl
CFLAGS_COM = /Wall /WX /W3 /wd4146 /wd4711 /wd4710 /wd5045 /std:c11
CFLAGS_DBG = $(CFLAGS_COM) /O0
CFLAGS = $(CFLAGS_COM) /O2
IFLAGS = 
LFLAGS =

OBJS = fft.obj
HDRS = fft.h

all: $(OBJS)

.MAIN: all

.SUFFIXES: .c .obj
.c.obj:
	$(CC) $(IFLAGS) $(CFLAGS) /c $<

test.exe: test.c
	$(CC) $(CFLAGS_DBG) $< /Fe:$@ $(LFLAGS)

win32_time.exe: win32_time.c
	$(CC) $(CFLAGS) $< /Fe:$@ $(LFLAGS)
	./win32_time

check: all test.exe
	test --verbose

copy: all
	@if [ -n "$(PREFIX)" ] ; then copy $(OBJS) $(HDRS) $(PREFIX) ; fi

clean:
	if exist test.exe del test.exe test.obj
	if exist win32_time.exe del win32_time.exe win32_time.obj
	del $(OBJS)
