#pragma once

enum dir { NORTH, EAST, WEST, SOUTH };

typedef struct Body {
    int pastXGrid, pastYGrid;
    int xGrid, yGrid;
    struct Body* next;
} Body;

typedef struct Player {
    Body* head;
    Body* tail;

    int pos;
    int direction;
    int initialSize;
    int parts;
    int partsDrawn;
    int score;
} Player;

#include "Engine/game.h"

void Player_Reset(Player* player);
Body* Player_AddBody(Body *tail);
void Player_Destroy(Player *player);
void Player_Move(Game* game);