#include "Engine/pallete.h"
#include "Engine/window.h"
#include "Engine/vector.h"

#include "game.h"

#include <string.h>


void SetDrawColor(SDL_Renderer* renderer,
                  const SDL_Color* color)
{
    SDL_SetRenderDrawColor(renderer, color->r,
                                     color->g,
                                     color->b,
                                     color->a);
}

void CopyTexture(SDL_Renderer* renderer,
                 SDL_Texture* texture,
                 Vec2 position, float width, float height)
{
    SDL_Rect rect = (SDL_Rect) {(int)position.x, (int)position.y,
                                (int)width, (int)height};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

#define CENTERED -1

// Draw text on screen
void DrawText(const Game* game,
              const SDL_Color* color,
              Vec2 position,
              const char* text,
              TTF_Font* font)
{
    Window* window = game->window;
    SDL_Renderer* renderer = window->SDLRenderer;

    SDL_Surface* tmpSurface = TTF_RenderText_Blended(font, text, *color);
    float width = (float)tmpSurface->w;
    float height = (float)tmpSurface->h;

    SDL_Texture* label = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);

    if (position.x == CENTERED)
        position.x = (window->width - width)/2.0f;

    if (position.y == CENTERED)
        position.y = (window->height - height)/2.0f;

    CopyTexture(renderer, label, position, width, height);
}

void DrawPoint(SDL_Renderer* renderer, const SDL_Color* color,
               Vec2 p)
{
    SetDrawColor(renderer, color);
    SDL_RenderDrawPoint(renderer, (int)p.x, (int)p.y);
}

void DrawLine(SDL_Renderer* renderer, const SDL_Color* color,
              Vec2 a, Vec2 b)
{
    SetDrawColor(renderer, color);
    SDL_RenderDrawLine(renderer, (int)a.x, (int)a.y,
                                 (int)b.x, (int)b.y);
}

void DrawFillCircle(SDL_Renderer* renderer, const SDL_Color* color,
                    Vec2 center, float radius)
{
    for(int y=-radius; y<=radius; y++)
        for(int x=-radius; x<=radius; x++)
            if(x*x+y*y <= radius*radius)
                DrawPoint(renderer, color,
                          Vector2_Add(center, Vector2((float)x, (float)y)));
}

void DrawBackground(const Game* game)
{
    SDL_Renderer* renderer = game->window->SDLRenderer;

    // Draw black background
    SetDrawColor(renderer, &black);
    SDL_RenderClear(renderer);

    // Draw score
    char str[128] = "";

    sprintf(str, "score:%d", game->score);
    DrawText(game, &white,
             Vector2(2 * BLOCK_SIZE, BLOCK_SIZE),
             str, game->font);

    sprintf(str, "highest:%d", game->highScores[0].value);
    DrawText(game, &white,
             Vector2((game->cols - 13) * BLOCK_SIZE, BLOCK_SIZE),
             str, game->font);

    // Draw main screen
    SetDrawColor(renderer, &grey);

    SDL_Rect mainScreen;
    mainScreen.x = 2*BLOCK_SIZE;
    mainScreen.y = 3*BLOCK_SIZE;
    mainScreen.w = mainScreen.h = 22*BLOCK_SIZE;

    SDL_RenderFillRect(renderer, &mainScreen);
}

void Apple_Draw(const Game* game)
{
    SDL_Renderer* renderer = game->window->SDLRenderer;

    SetDrawColor(renderer, &red);

    SDL_Rect appleRect;
    appleRect.x = game->applePos.x * BLOCK_SIZE;
    appleRect.y = game->applePos.y * BLOCK_SIZE;
    appleRect.w = appleRect.h = BLOCK_SIZE - 5;

    SDL_RenderFillRect(renderer, &appleRect);
}

void DrawGameScreen(const Game* game)
{
    DrawBackground(game);

    Apple_Draw(game);
    Snake_Draw(game);
}

void DrawPauseScreen(const Game* game)
{
    DrawBackground(game);

    DrawText(game, &white,
             Vector2(CENTERED, 5 * BLOCK_SIZE),
             "PAUSED", game->font);

    DrawText(game, &white,
             Vector2(CENTERED, CENTERED),
             "Press p to continue", game->font);
}

void DrawGameOverScreen(Game* game)
{
    DrawText(game, &white,
             Vector2(CENTERED, 5 * BLOCK_SIZE),
             "GAME OVER",
             game->font);

    char str[128];
    sprintf(str, "Score: %d", game->score);
    DrawText(game, &white,
             Vector2(CENTERED, 7 * BLOCK_SIZE),
             str,
             game->font);

    DrawText(game, &white,
             Vector2(CENTERED, CENTERED),
             "Continue? (Y/N)",
             game->font);

    switch(game->option)
    {
        case 0:
            game->isRunning = SDL_FALSE;
            break;
        case 1:
            Game_Reset(game);
            break;
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

    DrawText(game, &colours[curPlace],
             Vector2(CENTERED, (5 + curPlace) * BLOCK_SIZE),
             str, game->font);

    DrawPlaces(game, ++curPlace);
}

void DrawHighscoreScreen(Game* game)
{
    SDL_Renderer* renderer = game->window->SDLRenderer;

    SetDrawColor(renderer, &black);
    SDL_RenderClear(renderer);

    DrawText(game, &lightBlue,
             Vector2(CENTERED, 2 * BLOCK_SIZE),
             "Highscore Board", game->font);
    DrawText(game, &yellow,
             Vector2(CENTERED, 4 * BLOCK_SIZE),
             "Place\tScore\tName", game->font);

    DrawPlaces(game, 0);
}

void DrawNewHighscore(Game* game)
{
    DrawText(game, &white,
             Vector2(CENTERED, 15 * BLOCK_SIZE),
             "New Highscore!", game->font);

    /* Show place and score */
    char place[4];

    int i;
    int placeNum;
    for (i = 4; i > -1; i--)
    {
        if (game->score < game->highScores[i].value)
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

    DrawText(game, &white,
             Vector2(CENTERED, 16 * BLOCK_SIZE),
             str, game->font);

    /* Prompt */
    DrawText(game, &white,
             Vector2(CENTERED, 17 * BLOCK_SIZE),
             "Input your name", game->font);

    DrawText(game, &white,
             Vector2(CENTERED, 18 * BLOCK_SIZE),
             game->text, game->font);

    game->pos = placeNum - 1;

    if (game->option == 1)
    {
        if (game->isInputTooShort)
        {
            DrawText(game, &white,
                     Vector2(CENTERED, 19 * BLOCK_SIZE),
                     "At least 3 characters", game->font);
            return;
        }

        strcpy(game->highScores[game->pos].name,
               game->text);
        Game_Reset(game);
    }
}

void Render(Game* game)
{
    switch(game->state)
    {
        case PLAY:
            DrawGameScreen(game);
            break;
        case PAUSE:
            DrawPauseScreen(game);
            break;
        case GAMEOVER:
            DrawGameOverScreen(game);
            break;
        case NEW_HIGHSCORE:
            DrawHighscoreScreen(game);
            DrawNewHighscore(game);
            break;
    }

    if (game->isInputTooShort)
    {
        SDL_Delay(500);
        game->isInputTooShort = SDL_FALSE;
        game->option = -1;
    }

    SDL_RenderPresent(game->window->SDLRenderer);
}
