#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


enum dir { NORTH, EAST, WEST, SOUTH };
enum state { GAME, PAUSE, GAMEOVER, PROMPT, SAVE, BOARD };

typedef struct Body {
    int pastXGrid, pastYGrid;
    int xGrid, yGrid;
    struct Body* next;
} Body;

typedef struct {
    Body* head;
    Body* tail;

    int pos;
    int direction;
    int parts;
    int partsDrawn;
    int score;
} Player;

typedef struct {
    int xGrid, yGrid;
} Apple;

typedef struct {
    char name[4];
    int value;
} Score;

/**
 * @brief Contains data necessary to run the game
 */
typedef struct Game {
    SDL_Window* window;
    SDL_Renderer* renderer;

    // Window resolution
    unsigned int width, height;

    SDL_bool isRunning;
    int state;
    int ok;
    float fps;
    float dt;

    SDL_Event event;
    // Keyboard state
    const Uint8* keyboard;
    Uint8 keyJustPressed;
    Uint8 keyBeingHeld;

    TTF_Font* font;

    char text[4];
    char* composition;
    Sint32 cursor;
    Sint32 selection_len;
    int inputLength;

    Player player;
    int blockSize;
    int initialSize;

    Apple apple;

    int asw;
    int gameOver;
    Score highScores[5];
} Game;

Game* CreateGame(const char* title,
                 const unsigned int width, const unsigned int height);
void DestroyGame(Game* game);
void GameLoop(Game* game);
void ResetGame(Game* game);
