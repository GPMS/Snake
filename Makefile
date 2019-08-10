CC = gcc
FLAGS = -std=c11 -g -Wall -pedantic -O3
INCLUDES = -lSDL2 -lSDL2_ttf
SRC = src/*.c
EXEC = game

make:
	$(CC) -o $(EXEC) $(FLAGS) $(INCLUDES) $(SRC)

clean:
	rm $(EXEC)
