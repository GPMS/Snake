#pragma once

struct Game;

typedef struct Score {
    char name[4];
    int value;
} Score;

void Highscore_New(struct Game* game);
void Highscore_Load(struct Game* game);
void Highscore_Save(const struct Game* game);