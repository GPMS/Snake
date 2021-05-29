#include <stdio.h>

#include "window.h"
#include "input.h"
#include "draw.h"
#include "game.h"

static Score highscores[5];

int Highscore_GetHighestScore()
{
    return highscores[0].value;
}

int Highscore_GetLowestScore()
{
    return highscores[4].value;
}

// Shift all places down starting from "place"
static void ShiftPlaces(Game* game, int place)
{
    char holdName[2][4] = {"", ""};
    int  holdValue[2]   = {0, 0};

    int side = 0;

    holdValue[side] = highscores[place].value;
    strcpy(holdName[side], highscores[place].name);

    side ^= 1;

    for (int i = place + 1; i < 5; i++) {
        holdValue[side] = highscores[i].value;
        strcpy(holdName[side], highscores[i].name);

        highscores[i].value = holdValue[!side];
        strcpy(highscores[i].name, holdName[!side]);

        side ^= 1;
    }
}

// Find out player's place in the highscore
// and secure a new position for him in that place
void Highscore_New(Game* game)
{
    int place;
    for (int i = 0; i < 5; i++) {
        if (i == 4) {
            place = 4;
        } else if (game->score > highscores[i].value) {
            ShiftPlaces(game, i);
            place = i;
            break;
        }
    }
    game->place             = place;
    highscores[place].value = game->score;
    strcpy(highscores[place].name, "---");

    game->state = NEW_HIGHSCORE;
}

void Highscore_Load(Game* game)
{
    FILE* fp = fopen("saveData.bin", "rb");
    int   i;

    if (fp == NULL) {
        printf("NO SAVE FILE\n");
        for (i = 0; i < 5; i++) {
            strcpy(highscores[i].name, "???");
            highscores[i].value = 0;
        }
    } else {
        for (i = 0; i < 5; i++) {
            fseek(fp, (sizeof(Score) * i), SEEK_SET);
            fread(&highscores[i], sizeof(Score), 1, fp);
        }
        printf("SAVE DATA LOADED\n");
        fclose(fp);
    }
}

void Highscore_Save(const Game* game)
{
    FILE* fp = fopen("saveData.bin", "wb");
    if (!fp) {
        fprintf(stderr, "HIGHSCORE: FILE TO CREATE SAVE FILE!\n");
        return;
    }

    fwrite(highscores, sizeof(Score), 5, fp);
    fclose(fp);

    fprintf(stderr, "SAVED\n");
}

void Highscore_Update(Game* game)
{
    // Get name for new highscore
    if (HasTextInput()) {
        const char* textInput = GetTextInput();
        int         len       = strlen(textInput);

        for (int i = 0; i < len; i++) {
            char c = textInput[i];

            if (c == '\0') {
                break;
            }

            if (game->textLength >= 3)
                break;

            if (c != '?' && c != '-') {
                game->text[game->textLength++] = c;
                strcpy(highscores[game->place].name, game->text);
            }
        }
    }

    // Delete one character from name
    if (IsKeyPressed(SDL_SCANCODE_BACKSPACE) || IsKeyHeld(SDL_SCANCODE_BACKSPACE)) {
        if (game->textLength > 0) {
            game->text[--game->textLength] = '-';
            strcpy(highscores[game->place].name, game->text);
        }
    }

    // Confirm name
    if (IsKeyReleased(SDL_SCANCODE_RETURN)) {
        for (int i = 0; i < 3; i++) {
            if (game->text[i] == '-')
                game->text[i] = ' ';
        }
        strcpy(highscores[game->place].name, game->text);
        Game_Reset(game);
    }
}

static void DrawPlaces(const Game* game, int curPlace)
{
    char str[128] = "";

    const SDL_Color colors[5] = {lightGrey, red, orange, lightPink, lightOrange};

    if (curPlace == 5)
        return;

    const int posY      = (5 + curPlace) * BLOCK_SIZE;
    const int posXPlace = 6 * BLOCK_SIZE;
    const int posXName  = 17 * BLOCK_SIZE;

    sprintf(str, "  %d  %5d", curPlace + 1, highscores[curPlace].value);
    DrawText(game->window, &colors[curPlace], Vector2(posXPlace, posY), str, game->font);
    DrawText(game->window, &colors[curPlace], Vector2(posXName, posY), highscores[curPlace].name, game->font);
    if (curPlace == game->place) {
        const SDL_Color* color;
        double           flicker = sin(SDL_GetTicks() / 200.0);
        if (flicker <= 0.0) {
            color = &white;
        } else {
            color = &black;
        }
        SetDrawColor(game->window->SDLRenderer, color);
        DrawLine(game->window->SDLRenderer,
                 Vector2(posXPlace, (6 + curPlace) * BLOCK_SIZE),
                 Vector2(posXName + 3 * BLOCK_SIZE, (6 + curPlace) * BLOCK_SIZE));
    }

    DrawPlaces(game, ++curPlace);
}

static void DrawNewHighscoreScreen(Game* game)
{
    DrawText(game->window, &lightBlue, Vector2(CENTERED, 2 * BLOCK_SIZE), "New Highscore!", game->font);
    DrawText(game->window, &yellow, Vector2(6 * BLOCK_SIZE, 4 * BLOCK_SIZE), "Place", game->font);
    DrawText(game->window, &yellow, Vector2(11 * BLOCK_SIZE, 4 * BLOCK_SIZE), "Score", game->font);
    DrawText(game->window, &yellow, Vector2(16 * BLOCK_SIZE, 4 * BLOCK_SIZE), "Name", game->font);

    DrawPlaces(game, 0);
}

void Highscore_Draw(Game* game)
{
    DrawNewHighscoreScreen(game);
    DrawText(game->window, &white, Vector2(CENTERED, 16 * BLOCK_SIZE), "Input your name...", game->font);
}
