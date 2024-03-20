UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux) #LINUX
OS := Linux
INCDIRS := -I/usr/include/SDL2 -I/usr/include/freetype2
LIBDIRS := -L/usr/lib/x86_64-linux-gnu
CHECK_SDL
endif

ifeq ($(UNAME_S), Darwin) #APPLE
OS := macOS
INCDIRS := -I/opt/homebrew/include/SDL2 -I/opt/homebrew/include/freetype2
LIBDIRS := -L/opt/homebrew/lib
endif

C_FLAGS = -Wall -g -fPIC

all: check libcini.so

check:
ifneq ("$(wildcard /usr/include/cini.h)","")
	@echo 
endif

cini.o: cini.c
	$(CC) -c -o cini.o cini.c $(INCDIRS) $(C_FLAGS)

libcini.so: cini.o
	$(CC) -shared -o $@ $< -L /opt/homebrew/lib/ -lSDL2 -lSDL2_ttf
	mkdir -p include/ lib/
	cp $@ lib/
	cp cini.h include/

clean:
	rm -r *.o *.so lib/ include/
