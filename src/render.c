#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

#include "render.h"

#define CENTERED -1

// Colors
//                             R    G    B    A
SDL_Color white         =   { 255, 255, 255, 255 };
SDL_Color lightGrey     =   { 192, 192, 192, 255 };
SDL_Color grey          =   {  50,  50,  50, 255 };
SDL_Color black         =   {   0,   0,   0, 255 };
SDL_Color red           =   { 255,   0,   0, 255 };
SDL_Color green         =   {   0, 255,   0, 255 };
SDL_Color blue          =   {   0,   0, 255, 255 };
SDL_Color lightPink     =   { 255, 153, 153, 255 };
SDL_Color orange        =   { 255, 128,   0, 255 };
SDL_Color lightOrange   =   { 255, 178, 102, 255 };
SDL_Color lightBlue     =   {  51, 255, 255, 255 };
SDL_Color yellow        =   { 255, 255,   0, 255 };

void setRenderColor(SDL_Renderer *renderer, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void drawText(int inputX, int inputY, const char *text, TTF_Font* font, SDL_Color color, SDL_Renderer *renderer)
{
    SDL_Texture *label = NULL;
    SDL_Surface *tmpSurface = NULL;
    int x, y;
    
    tmpSurface = TTF_RenderText_Blended(font, text, color);
    label = SDL_CreateTextureFromSurface(renderer, tmpSurface);

    if (inputX == CENTERED) x = (WIDTH*BLOCK_SIZE - tmpSurface->w)/2;
    else                    x = inputX;
    
    if (inputY == CENTERED) y = (HEIGHT*BLOCK_SIZE - tmpSurface->h)/2;
    else                    y = inputY;
    
    SDL_Rect textRect = {x, y, tmpSurface->w, tmpSurface->h};
    
    SDL_RenderCopy(renderer, label, NULL, &textRect);
    SDL_FreeSurface(tmpSurface);
}

SDL_Renderer *drawGameScreen(const GameState *game)
{
    SDL_Renderer *renderer = game->renderer;

    /* Draw black background */
    setRenderColor(renderer, black);
    SDL_RenderClear(renderer);

    /* Draw score */
    char str[128] = "";

    sprintf(str, "score:%d", game->player.score);
    drawText(2 * BLOCK_SIZE, BLOCK_SIZE, str, game->font, white, renderer);

    sprintf(str, "highest:%d", game->highScores[0].value);
    drawText((WIDTH - 13) * BLOCK_SIZE, BLOCK_SIZE, str, game->font, white, renderer);

    /* Draw main screen */
    setRenderColor(renderer, grey);

    SDL_Rect mainScreen = {
        2*BLOCK_SIZE, 3*BLOCK_SIZE,
        22*BLOCK_SIZE, 22*BLOCK_SIZE
    };

    SDL_RenderFillRect(renderer, &mainScreen);

    /* Draw apple */
    setRenderColor(renderer, red);

    SDL_Rect appleRect;
    appleRect.x = game->apple.xGrid * BLOCK_SIZE;
    appleRect.y = game->apple.yGrid * BLOCK_SIZE;
    appleRect.w = appleRect.h = BLOCK_SIZE - 5;
    
    SDL_RenderFillRect(renderer, &appleRect);

    /* Draw snake */
    setRenderColor(renderer, green);

    SDL_Rect snakeRect;
    snakeRect.w = snakeRect.h = BLOCK_SIZE - 5;

    Body *current = game->player.head;

    while (current != NULL)
    {
        snakeRect.x = current->xGrid*BLOCK_SIZE;
        snakeRect.y = current->yGrid*BLOCK_SIZE;

        SDL_RenderFillRect(renderer, &snakeRect);

        current = current->next;
    }

    return renderer;
}


SDL_Renderer *drawPauseScreen(const GameState *game)
{
    SDL_Renderer *renderer = game->renderer;

    // Clear playground
    setRenderColor(renderer, black);
    SDL_RenderClear(renderer);

    // Draw score
    char str[128] = "";
    sprintf(str, "score:%d", game->player.score);

    drawText(BLOCK_SIZE, BLOCK_SIZE, str, game->font, white, renderer);

    // Draw main screen
    setRenderColor(renderer, grey);

    SDL_Rect mainScreen;
    mainScreen.x = 2*BLOCK_SIZE;
    mainScreen.y = 3*BLOCK_SIZE;
    mainScreen.w = mainScreen.h = 22*BLOCK_SIZE;

    SDL_RenderFillRect(renderer, &mainScreen);

    // Draw pause
    drawText(CENTERED, 5 * BLOCK_SIZE, "PAUSED", game->font, white, renderer);

    // Draw instructions
    drawText(CENTERED, CENTERED, "Press p to continue", game->font, white, renderer);

    return renderer;
}


SDL_Renderer *newHighscore(GameState *game)
{
    SDL_Renderer *renderer = game->renderer;

    drawText(CENTERED, 15 * BLOCK_SIZE, "New Highscore!", game->font, white, renderer);

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

    drawText(CENTERED, 16 * BLOCK_SIZE, str, game->font, white, renderer);

    /* Prompt */
    drawText(CENTERED, 17 * BLOCK_SIZE, "Input your name", game->font, white, renderer);

    drawText(CENTERED, 18 * BLOCK_SIZE, game->text, game->font, white, renderer);

    game->player.pos = placeNum - 1;

    if (game->ok == 1)
    {
        strcpy(game->highScores[game->player.pos].name, game->text);
        ResetGame(game);
        game->state = GAME;
    }
    else if (game->ok == 3)
    {
        drawText(CENTERED, 19 * BLOCK_SIZE, "At least 3 characters", game->font, white, renderer);
    }

    return renderer;
}


SDL_Renderer *drawPlaces(const GameState *game, int curPlace)
{
    SDL_Renderer *renderer = game->renderer;
    char str[128] = "";

    SDL_Color colours[5] = { lightGrey, red, orange, lightPink, lightOrange };

    if (curPlace == 5)  return renderer;
    
    sprintf(str, "  %d  \t%5d\t %s", curPlace+1, game->highScores[curPlace].value, game->highScores[curPlace].name);

    drawText(CENTERED, (5 + curPlace) * BLOCK_SIZE, str, game->font, colours[curPlace], renderer);

    return drawPlaces(game, ++curPlace);
}


SDL_Renderer *drawHighscoreScreen(GameState *game)
{
    SDL_Renderer *renderer = game->renderer;

    setRenderColor(renderer, black);
    SDL_RenderClear(renderer);

    drawText(CENTERED, 2 * BLOCK_SIZE, "Highscore Board", game->font, lightBlue, renderer);
    drawText(CENTERED, 4 * BLOCK_SIZE, "Place\tScore\tName", game->font, yellow, renderer);

    renderer = drawPlaces(game, 0);

    renderer = newHighscore(game);

    return renderer;
}


SDL_Renderer *drawGameOverScreen(GameState *game)
{
    SDL_Renderer *renderer = game->renderer;

    /* Draw black background */
    setRenderColor(renderer, black);
    SDL_RenderClear(renderer);

    /* Write gameover */
    drawText(CENTERED, 5 * BLOCK_SIZE, "GameOver", game->font, white, renderer);

    char str[128];

    sprintf(str, "Score: %d", game->player.score);

    drawText(CENTERED, 7 * BLOCK_SIZE, str, game->font, white, renderer);

    /* Continue? */
    game->gameOver = 1;
    drawText(CENTERED, CENTERED, "Continue? (Y/N)", game->font, white, renderer);

    switch(game->asw)
    {
        case 0:
            game->running = SDL_FALSE;
            break;
        case 1:
            game->gameOver = 0;
            game->state = GAME;
            break;
    }

    return renderer;
}


void doRender(GameState *game)
{
    SDL_Renderer *renderer = game->renderer;

    switch(game->state)
    {
        case GAME:
            renderer = drawGameScreen(game);
            break;
        case PAUSE:
            renderer = drawPauseScreen(game);
            break;
        case GAMEOVER:
            renderer = drawGameOverScreen(game);
            break;
        case BOARD:
            renderer = drawHighscoreScreen(game);
            break;
    }

    SDL_RenderPresent(renderer);

    if (game->ok == 3)
    {
        SDL_Delay(500);
        game->ok= 0;
    }
}
