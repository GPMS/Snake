#include <stdlib.h>
#include <stdio.h>

#include "game.h"
#include "render.h"


/**
 * @brief Entry point for the program
 *
 * @param argc  Argument counter
 * @param argv  Argument values
 *
 * @return      0 on success, 1 on error
 */
int main(int argc, char* argv[])
{
    Game* game = CreateGame("Game", 26, 26);
    GameLoop(game);
    DestroyGame(game);

    return 0;
}
