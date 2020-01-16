#include <stdlib.h>
#include <stdio.h>

#include "Engine/game.h"
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
    Game* game = Game_Create("Game", 26, 26);
    Game_Loop(game);
    Game_Destroy(game);

    return 0;
}
