#include "game.h"

#include <stdlib.h>
#include <stdio.h>



// Entry point for the program
int main(int argc, char* argv[])
{
    struct Game game;

    Game_Init(&game);
    Game_Loop(&game);
    Game_Quit(&game);

    return 0;
}
