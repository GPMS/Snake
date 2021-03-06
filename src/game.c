#include "game.h"
#include "Engine/window.h"
#include "input.h"
#include "render.h"
#include <stdio.h>


void InitSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL_Init: %s", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() < 0)
    {
        fprintf(stderr, "TTF_Init: %s", TTF_GetError());
        exit(1);
    }
}

void QuitSDL()
{
    TTF_Quit();
    SDL_Quit();
}

// Wrapper for TTF_OpenFont
TTF_Font* LoadFont(const char* file,
                   const int size)
{
    TTF_Font* font = TTF_OpenFont(file, size);
    if (!font)
    {
        fprintf(stderr, "Unable to open %s: %s\n", file, TTF_GetError());
        SDL_Quit();
    }
    return font;
}

// Loads all necessary resources into SDL
void LoadResources(Game* game)
{
    game->font = LoadFont("resources/font.ttf", 24);
}

void Game_Reset(Game* game)
{
    game->state = PLAY;

    game->option = -1;
    strcpy(game->text, "???");
    game->textLength = 0;
    game->isInputTooShort = SDL_FALSE;

    game->isRunning = SDL_TRUE;

    game->score = 0;
    game->initialSize = 4;
    Snake_Destroy(&game->snake);
    Snake_Reset(game);
}

void Game_Init(Game* game)
{
    InitSDL();

    game->rows = game->cols = 26;

    game->window = Window_Create("Engine",
                                 game->cols * BLOCK_SIZE,
                                 game->rows * BLOCK_SIZE,
                                 0);

    game->state = PLAY;
    game->isRunning = SDL_TRUE;

    LoadResources(game);

    Game_Reset(game);
    Highscore_Load(game);
}

void Game_Quit(Game* game)
{
    Highscore_Save(game);

    TTF_CloseFont(game->font);
    TTF_Quit();

    Window_Destroy(game->window);

    QuitSDL();
}

// Updates game struct with FPS info
// If targetFPS is >0 this will also limit it
void CalculateFPS(Game* game, const unsigned int targetFPS)
{
    static Uint32 lastTime = 0;
    Uint32 currentTime = SDL_GetTicks();

    Uint32 dt = currentTime - lastTime;
    float fps = 1.0f / (dt / 1000.0f);

    lastTime = currentTime;

    if (targetFPS > 0)
    {
        float targetTime = (1.0f / targetFPS)*1000;

        Uint32 delayTime = 0;
        if (dt < targetTime)
        {
            delayTime = targetTime - dt;
            SDL_Delay(delayTime);

            dt += delayTime;
            fps = 1.0f / (dt / 1000.0f);
            lastTime += delayTime;
        }

    }
    game->dt = dt;
    game->fps = fps;

}

void Update(Game* game)
{
    if (game->state == PLAY)
    {
        Snake_Update(game);
    }
}

void Game_Loop(Game* game)
{
    while (game->isRunning)
    {
        CalculateFPS(game, 30);

        Input_Process(game);
        Update(game);
        Render(game);

    }
}
