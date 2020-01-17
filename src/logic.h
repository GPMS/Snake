#pragma once

typedef struct Apple {
    int xGrid, yGrid;
} Apple;

#include "player.h"
#include "Engine/game.h"

/**
 * @brief Gets called at the beginning of the game
 *
 * @param game  Game struct
 */
void Game_OnStart(Game* game);

/**
 * @brief Gets called once the game finishes
 *
 * @param game  Game struct
 */
void Game_OnFinish(Game* game);

/**
 * @brief Updates game logic
 *
 * @param game  Game struct
 */
void Logic(Game* game);

/**
 * @brief Resets all variables enabling a new game to start
 *
 * @param game Game struct
 */
void Game_Reset(Game* game);