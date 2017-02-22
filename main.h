#ifndef MAIN_H
#define MAIN_H

#define WIDTH           26  // BLOCK_SIZEs
#define HEIGHT          26  // BLOCK_SIZEs
#define NAME            "Snake"
#define BLOCK_SIZE      25
#define INITIAL_SIZE    4   // How many parts the snake starts with


enum dir { NORTH, EAST, WEST, SOUTH };
enum state { GAME, PAUSE, GAMEOVER };

typedef struct Body{
    int pastXGrid, pastYGrid;
    int xGrid, yGrid;
    struct Body *next;
} Body;

typedef struct{
    int xGrid, yGrid;
} Apple;

typedef struct{
    SDL_bool running;
    int state;
    
    /* Player */
    int direction;
    int parts;
    int partsDrawn;
    int score;
    
    SDL_Texture *label;
    TTF_Font *font;

} GameState;

#endif
