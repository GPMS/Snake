#pragma once

struct Game;

typedef struct Score {
    char name[4];
    int  value;
} Score;

int  Highscore_GetHighestScore();
int  Highscore_GetLowestScore();
void Highscore_New(struct Game* game);
void Highscore_Load(struct Game* game);
void Highscore_Save(const struct Game* game);
void Highscore_Update(struct Game* game);
void Highscore_Draw(struct Game* game);
