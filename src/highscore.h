#pragma once

typedef struct Score {
    char name[4];
    int value;
} Score;

void Highscore_New(Game* game);
void Highscore_Load(Game* game);
void Highscore_Save(const Game* game);