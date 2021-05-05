#pragma once

#include "vector.h"

struct Game;

enum dir {
    NORTH,
    EAST,
    WEST,
    SOUTH
};

typedef struct Body {
    Vec2 pos;
    Vec2 pastPos;
} Body;

typedef struct Snake {
    Body* body;

    int bodyLength;
    int direction;
    int parts;
    int partsDrawn;
} Snake;

void Snake_Reset(struct Game* game);
void Snake_Update(struct Game* game);
void Snake_Draw(const struct Game* game);
