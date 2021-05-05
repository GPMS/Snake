#pragma once

#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "highscore.h"
#include "snake.h"
#include "vector.h"

struct Window;

enum GameState {
    PLAY,
    PAUSE,
    GAMEOVER,
    NEW_HIGHSCORE
};

#define BLOCK_SIZE 26

// Contains data necessary to run the game
typedef struct Game {
    struct Window* window;

    // Game internal information
    bool           isRunning;
    float          fps;
    Uint32         dt;
    enum GameState state;

    // Input
    char text[4];
    int  textLength;

    // Font files
    TTF_Font* font;

    // Game Logic
    int rows, cols;

    int option;

    Vec2 applePos;

    Snake snake;
    int   initialSize;

    int   score;
    int   place;
    int   pos;
    Score highScores[5];
} Game;

void Game_Reset(Game* game);

// Sets up SDL and creates a window and a renderer, filling up
Game* Game_Create();

// Deallocates memory and quits SDL
void Game_Destroy(Game* game);

// Game loop...
void Game_Loop(Game* game);
