#include <string.h>

#include "render.h"

#define CENTERED -1


//          Palette
//                          R    G    B    A
const SDL_Color white   = {255, 255, 255, 255};
const SDL_Color black   = {  0,   0,   0, 255};
const SDL_Color red     = {255,   0,   0, 255};
const SDL_Color green   = {  0, 255,   0, 255};
const SDL_Color blue    = {  0,   0, 255, 255};
const SDL_Color lightGrey     =   { 192, 192, 192, 255 };
const SDL_Color grey          =   {  50,  50,  50, 255 };
const SDL_Color lightPink     =   { 255, 153, 153, 255 };
const SDL_Color orange        =   { 255, 128,   0, 255 };
const SDL_Color lightOrange   =   { 255, 178, 102, 255 };
const SDL_Color lightBlue     =   {  51, 255, 255, 255 };
const SDL_Color yellow        =   { 255, 255,   0, 255 };

/**
 * @brief Use a color from the palette to set the draw color
 *
 * @param renderer  SDL renderer
 * @param color     Color from the palette
 */
void SetRenderDrawColor(SDL_Renderer* renderer,
                        const SDL_Color* color)
{
    SDL_SetRenderDrawColor(renderer, color->r,
                                     color->g,
                                     color->b,
                                     color->a);
}

/**
 * @brief Given a font, draw text on screen
 *
 * @param game      The game struct
 * @param x         Text x coordinate (can set to CENTERED)
 * @param y         Text y coordinate (can set to CENTERED)
 * @param text      Text string
 * @param font      Game font
 * @param color     Text color
 * @param renderer  SDL renderer
 */
void DrawText(const Game* game,
              int x, int y,
              const char* text,
              TTF_Font* font, const SDL_Color* color)
{
    SDL_Surface* tmpSurface = NULL;
    tmpSurface = TTF_RenderText_Blended(font, text, *color);

    // create texture from surface and place it on the screen
    SDL_Texture* label = NULL;
    label = SDL_CreateTextureFromSurface(game->renderer, tmpSurface);

    if (x == CENTERED) x = (game->width * game->blockSize - tmpSurface->w)/2;
    if (y == CENTERED) y = (game->height * game->blockSize - tmpSurface->h)/2;

    SDL_Rect textRect = {x, y, tmpSurface->w, tmpSurface->h};
    SDL_FreeSurface(tmpSurface);

    SDL_RenderCopy(game->renderer, label, NULL, &textRect);
}

void DrawGameScreen(const Game* game)
{
    SDL_Renderer* renderer = game->renderer;

    /* Draw black background */
    SetRenderDrawColor(renderer, &black);
    SDL_RenderClear(renderer);

    /* Draw score */
    char str[128] = "";

    sprintf(str, "score:%d", game->player.score);
    DrawText(game,
             2 * game->blockSize, game->blockSize,
             str, game->font, &white);

    sprintf(str, "highest:%d", game->highScores[0].value);
    DrawText(game,
             (game->width - 13) * game->blockSize, game->blockSize,
             str, game->font, &white);

    /* Draw main screen */
    SetRenderDrawColor(renderer, &grey);

    SDL_Rect mainScreen = {
        2*game->blockSize, 3*game->blockSize,
        22*game->blockSize, 22*game->blockSize
    };

    SDL_RenderFillRect(renderer, &mainScreen);

    /* Draw apple */
    SetRenderDrawColor(renderer, &red);

    SDL_Rect appleRect;
    appleRect.x = game->apple.xGrid * game->blockSize;
    appleRect.y = game->apple.yGrid * game->blockSize;
    appleRect.w = appleRect.h = game->blockSize - 5;

    SDL_RenderFillRect(renderer, &appleRect);

    /* Draw snake */
    SetRenderDrawColor(renderer, &green);

    SDL_Rect snakeRect;
    snakeRect.w = snakeRect.h = game->blockSize - 5;

    Body* current = game->player.head;

    while (current != NULL)
    {
        snakeRect.x = current->xGrid*game->blockSize;
        snakeRect.y = current->yGrid*game->blockSize;

        SDL_RenderFillRect(renderer, &snakeRect);

        current = current->next;
    }
}


void DrawPauseScreen(const Game* game)
{
    SDL_Renderer* renderer = game->renderer;

    /* Clear playground */
    SetRenderDrawColor(renderer, &black);
    SDL_RenderClear(renderer);

    /* Draw score */
    char str[128] = "";
    sprintf(str, "score:%d", game->player.score);

    DrawText(game,
             game->blockSize, game->blockSize,
             str, game->font, &white);

    /* Draw main screen */
    SetRenderDrawColor(renderer, &grey);

    SDL_Rect mainScreen;
    mainScreen.x = 2*game->blockSize;
    mainScreen.y = 3*game->blockSize;
    mainScreen.w = mainScreen.h = 22*game->blockSize;

    SDL_RenderFillRect(renderer, &mainScreen);

    /* Draw pause */
    DrawText(game,
             CENTERED, 5 * game->blockSize,
             "PAUSED", game->font, &white);

    // Draw instructions
    DrawText(game,
             CENTERED, CENTERED,
             "Press p to continue", game->font, &white);
}


void DrawNewHighscore(Game* game)
{
    DrawText(game,
             CENTERED, 15 * game->blockSize,
             "New Highscore!", game->font, &white);

    /* Show place and score */
    char place[4];

    int i;
    int placeNum;
    for (i = 4; i > -1; i--)
    {
        if (game->player.score < game->highScores[i].value)
        {
            placeNum = i+2;
            break;
        }
        else
        {
            placeNum = 1;
        }
    }

    switch(placeNum)
    {
        case 1:
            place[1] = 's';
            place[2] = 't';
            break;
        case 2:
            place[1] = 'n';
            place[2] = 'd';
            break;
        case 3:
            place[1] = 'r';
            place[2] = 'd';
            break;
        default:
            place[1] = 't';
            place[2] = 'h';
            break;
    }
    place[0] = placeNum + '0';
    place[3] = '\0';

    char str[128] = "";
    sprintf(str, "%s place", place);

    DrawText(game,
             CENTERED, 16 * game->blockSize,
             str, game->font, &white);

    /* Prompt */
    DrawText(game,
             CENTERED, 17 * game->blockSize,
             "Input your name", game->font, &white);

    DrawText(game,
             CENTERED, 18 * game->blockSize,
             game->text, game->font, &white);

    game->player.pos = placeNum - 1;

    if (game->ok == 1)
    {
        strcpy(game->highScores[game->player.pos].name, game->text);
        ResetGame(game);
    }
    else if (game->ok == 3)
    {
        DrawText(game,
                 CENTERED, 19 * game->blockSize,
                 "At least 3 characters", game->font, &white);
    }
}


void DrawPlaces(const Game* game, int curPlace)
{
    char str[128] = "";

    SDL_Color colours[5] = {lightGrey,
                            red,
                            orange,
                            lightPink,
                            lightOrange};

    if (curPlace == 5)  return;

    sprintf(str, "  %d  \t%5d\t %s",
            curPlace+1,
            game->highScores[curPlace].value,
            game->highScores[curPlace].name);

    DrawText(game,
             CENTERED, (5 + curPlace) * game->blockSize,
             str, game->font, &colours[curPlace]);

    DrawPlaces(game, ++curPlace);
}


void DrawHighscoreScreen(Game* game)
{
    SDL_Renderer* renderer = game->renderer;

    SetRenderDrawColor(renderer, &black);
    SDL_RenderClear(renderer);

    DrawText(game,
             CENTERED, 2 * game->blockSize,
             "Highscore Board", game->font, &lightBlue);
    DrawText(game,
             CENTERED, 4 * game->blockSize,
             "Place\tScore\tName", game->font, &yellow);

    DrawPlaces(game, 0);

    DrawNewHighscore(game);
}


void DrawGameOverScreen(Game* game)
{
    SDL_Renderer* renderer = game->renderer;

    /* Draw black background */
    SetRenderDrawColor(renderer, &black);
    SDL_RenderClear(renderer);

    /* Write gameover */
    DrawText(game,
             CENTERED, 5 * game->blockSize,
             "GameOver", game->font, &white);

    char str[128];

    sprintf(str, "Score: %d", game->player.score);

    DrawText(game,
             CENTERED, 7 * game->blockSize,
             str, game->font, &white);

    /* Continue? */
    game->gameOver = 1;
    DrawText(game,
             CENTERED, CENTERED,
             "Continue? (Y/N)", game->font, &white);

    switch(game->asw)
    {
        case 0:
            game->isRunning = SDL_FALSE;
            break;
        case 1:
            game->gameOver = 0;
            ResetGame(game);
            break;
    }
}


void Render(Game* game)
{
    switch(game->state)
    {
        case GAME:
            DrawGameScreen(game);
            break;
        case PAUSE:
            DrawPauseScreen(game);
            break;
        case GAMEOVER:
            DrawGameOverScreen(game);
            break;
        case BOARD:
            DrawHighscoreScreen(game);
            break;
    }

    SDL_RenderPresent(game->renderer);

    if (game->ok == 3)
    {
        SDL_Delay(500);
        game->ok= 0;
    }
}