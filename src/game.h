#ifndef GAME_H
#define GAME_H

#include "Engine/vector.h"

#include "snake.h"
#include "highscore.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct Window;
struct Input;

enum GameState {
    PLAY,
    PAUSE,
    GAMEOVER,
    NEW_HIGHSCORE
};

#define BLOCK_SIZE 26

// Contains data necessary to run the game
typedef struct Game {
    struct Window*  window;

    // Game internal information
    SDL_bool        isRunning;
    float           fps;
    Uint32          dt;
    enum GameState  state;

    // Subsystems
    struct Input*   input;

    // Font files
    TTF_Font*       font;

    // Game Logic
    int rows, cols;

    int option;
    SDL_bool isInputTooShort;

    Vec2 applePos;

    Snake snake;
    int initialSize;

    int score;
    int pos;
    Score highScores[5];
} Game;

void Game_Reset(Game* game);

// Sets up SDL and creates a window and a renderer, filling up
void Game_Init(Game* game);

// Deallocates memory and quits SDL
void Game_Quit(Game* game);

// Game loop...
void Game_Loop(Game* game);

#endif
