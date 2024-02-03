UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux) #LINUX
OS := Linux
INCDIRS := -I/usr/include/SDL2 -I/usr/include/freetype2
endif

ifeq ($(UNAME_S), Darwin) #APPLE
OS := macOS
INCDIRS := -I/opt/homebrew/include/SDL2 -I/opt/homebrew/include/freetype2
endif

ifeq ($(OS), Windows_NT) #WINDOWS
OS := Windows
INCDIRS := -I../include/SDL2 -I../include/freetype2
endif

C_FLAGS = -Wall -g -fPIC

all : libcini.a

cini_h : cini.h
	mkdir -p ../include
	cp cini.h ../include/

cini.o : cini.c cini_h
	${CC} -c cini.c $(INCDIRS) ${C_FLAGS}

libcini.a : cini.o
	ar -q libcini.a cini.o
	mkdir -p ../lib
	cp libcini.a ../lib/

clean :
	rm *.o *.a ../lib/libcini.a ../include/cini.h
