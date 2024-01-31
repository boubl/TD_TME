C_FLAGS = -Wall -g -fPIC

INCDIRS := -I/opt/homebrew/include/SDL2 -I/opt/homebrew/include/freetype2

all : libcini.a

cini_h : cini.h
	mkdir -p ../include
	cp cini.h ../include/

cini.o : cini.c cini_h
	gcc ${C_FLAGS} -c cini.c $(INCDIRS)

libcini.a : cini.o
	ar -q libcini.a cini.o
	mkdir -p ../lib
	cp libcini.a ../lib/

clean :
	rm *.o *.a ../lib/libcini.a ../include/cini.h
