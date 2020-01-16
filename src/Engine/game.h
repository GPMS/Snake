#pragma once

typedef struct Game Game;

#include "input.h"
#include "../highscore.h"
#include "../logic.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct Player Player;
typedef struct Apple Apple;
typedef struct Score Score;

typedef enum GameState {
    GAME,
    PAUSE,
    GAMEOVER,
    HIGHSCORE
} GameState;

/**
 * @struct Game
 * @brief Contains data necessary to run the game
 */
typedef struct Game {
    SDL_Window*     window;
    SDL_Renderer*   renderer;

    // Window resolution
    unsigned int    width, height;

    // Game internal information
    SDL_bool        isRunning;
    float           fps;
    Uint32          dt;
    GameState       state;

    // Subsytems
    Input           input;

    // Font files
    TTF_Font*       fonts[1];
    int             numFonts;

    // GAME LOGIC
    // Player
    Player player;
    int blockSize;
    // Apple
    Apple apple;
    // MISC
    Score highScores[5];
} Game;

/**
 * @brief Sets up SDL and creates a window and a renderer, filling up
 * default values for the Game struct
 *
 * @param title     Window title
 * @param width     Window width
 * @param height    Window height
 *
 * @return          The game struct
 *
 * @see SetUpSDL
 * @see InitInput
 * @see LoadResources
 */
Game* Game_Create(const char* title,
                  const unsigned int width, const unsigned int height);

/**
 * @brief Deallocates memory and quits SDL
 *
 * @param game  Game struct
 */
void Game_Destroy(Game* game);

/**
 * @brief Game loop...
 *
 * @param game Game struct
 *
 * @see LimitFPS
 * @see HandleWindowEvents
 * @see Update
 * @see Render
 */
void Game_Loop(Game* game);
