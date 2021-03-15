#include "game.h"

#include <stdlib.h>
#include <stdio.h>

// Entry point for the program
int main(int argc, char* argv[])
{
    Game* game = Game_Create(&game);
    Game_Loop(game);
    Game_Destroy(game);

    return 0;
}
