#ifndef MAIN_H
#define MAIN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WIDTH           26  // BLOCK_SIZEs
#define HEIGHT          26  // BLOCK_SIZEs
#define GNAME            "Snake"
#define BLOCK_SIZE      25
#define INITIAL_SIZE    4   // How many parts the snake starts with


enum dir { NORTH, EAST, WEST, SOUTH };
enum state { GAME, PAUSE, GAMEOVER, PROMPT, SAVE, BOARD };

typedef struct Body {
    int pastXGrid, pastYGrid;
    int xGrid, yGrid;
    struct Body *next;
} Body;

typedef struct {
    int xGrid, yGrid;
} Apple;

typedef struct {
    char name[4];
    int value;
} Score;

typedef struct {
    SDL_bool running;
    int state;
    int ok;

    SDL_Window *window;
    SDL_Renderer *renderer;

    /* Player */
    int pos;
    int direction;
    int parts;
    int partsDrawn;
    int score;
    int asw;
    int gameOver;
    Score highScores[5];

    SDL_Texture *label;
    TTF_Font *font;

    char text[4];
    char *composition;
    Sint32 cursor;
    Sint32 selection_len;
    int l;
} GameState;

int processEvents(GameState *game);
void ResetGame(GameState *game, Body *head, Body **tail, Apple *apple);

#endif
