#include "Engine/game.h"

#include <stdio.h>

void ShiftPlace(Game* game, int place)
{

    int i;
    int side = 0;

    // Hold on to name and score
    char holdN[2][4] = {"",""}; // name
    int holdV[2]     = { 0, 0}; // score

    for (i = place; i < 4; i++)
    {

        holdV[side] = game->highScores[i+1].value;
        strcpy(holdN[side], game->highScores[i+1].name);

        if (i == place)
        {
            game->highScores[i+1].value = game->highScores[i].value;
            strcpy(game->highScores[i+1].name, game->highScores[i].name);
        }
        else
        {
            game->highScores[i+1].value = holdV[!side];
            strcpy(game->highScores[i+1].name, holdN[!side]);
        }

        side ^= 1;
    }
}

void Highscore_New(Game* game)
{
    Player* player = &game->player;

    for (int i = 4; i > -1; i--)
    {
        if (player->score < game->highScores[i].value)
        {
            ShiftPlace(game, i+1);

            strcpy( game->highScores[i+1].name, "---");
            game->highScores[i+1].value = player->score;
            break;
        }
        else if (player->score == game->highScores[i].value)
        {
            ShiftPlace(game, i);

            strcpy( game->highScores[i].name, "---");
            game->highScores[i].value = player->score;
            break;
        }
        else if (i == 0)
        {
            ShiftPlace(game, 0);

            strcpy( game->highScores[0].name, "---");
            game->highScores[0].value = player->score;
        }
    }

    game->state = HIGHSCORE;
}

void Highscore_Load(Game* game)
{
    FILE* fp = fopen("saveData.bin", "rb");
    int i;

    if (fp == NULL)
    {
        printf("NO SAVE FILE\n");
        for (i=0; i < 5; i++)
        {
            strcpy(game->highScores[i].name, "???");
            game->highScores[i].value = 0;
        }
    }
    else
    {
        for (i=0; i < 5; i++)
        {
            fseek(fp, (sizeof(Score) * i), SEEK_SET);
            fread(&game->highScores[i], sizeof(Score), 1, fp);
        }
        printf("SAVE DATA LOADED\n");
        fclose(fp);
    }
}

void Highscore_Save(const Game* game)
{
    FILE* fp = fopen("saveData.bin", "wb");
    int i;

    if (fp != NULL)
    {
        for(i=0; i < 5; i++)
        {
            fseek(fp, 0, SEEK_END);
            fwrite(&game->highScores[i], sizeof(Score), 5, fp);
        }
        printf("SAVED\n");
        fclose(fp);
    }
    else
    {
        printf("FILE OPEN ERROR\n");
    }
}