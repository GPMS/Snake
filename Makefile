CC= gcc
FLAGS= -g -Wall
LIBS= -lSDL2 -lSDL2_image -lSDL2_ttf
SRC= src/*.c src/Engine/*.c
OUTPUT= game

make:
	$(CC) -o $(OUTPUT) $(FLAGS) $(LIBS) $(SRC)