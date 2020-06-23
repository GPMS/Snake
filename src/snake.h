#pragma once

#include "Engine/vector.h"

struct Game;

enum dir { NORTH, EAST, WEST, SOUTH };

typedef struct Body {
    Vec2 pos;
    Vec2 pastPos;
    struct Body* next;
} Body;

typedef struct Snake {
    Body* head;
    Body* tail;

    int direction;
    int parts;
    int partsDrawn;
} Snake;

void Snake_Reset(struct Game* game);
Body* Snake_AddBody(Body *tail);
void Snake_Destroy(Snake *snake);
void Snake_Update(struct Game* game);
void Snake_Draw(const struct Game* game);