CC=gcc
CFLAGS=-Wall

FILES=./src/*.c 

LIBS=-lSDL2 -lSDL2_ttf

all:
	gcc -o game $(FILES) $(CFLAGS) $(LIBS)

clean:
	rm game
