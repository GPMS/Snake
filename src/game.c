#include "game.h"
#include "Engine/window.h"
#include "Engine/input.h"
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
    strcpy(game->input->text, "---");
    game->input->textLength = 0;
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

    game->input = Input_Create();

    game->state = PLAY;
    game->isRunning = SDL_TRUE;

    LoadResources(game);

    Game_Reset(game);
    Highscore_Load(game);
}

void Game_Quit(Game* game)
{
    Highscore_Save(game);

    Input_Destroy(game->input);

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

// Deals with SDL events
void ProcessInput(Game* game)
{
    Input* input = game->input;

    game->isRunning = Input_Get(input);
}

void Update(Game* game)
{
    Input* input = game->input;

    if (Input_KeyWasReleased(input, SDLK_ESCAPE))
        game->isRunning = SDL_FALSE;

    // Pause/Unpause the game
    if (Input_KeyWasPressed(input, SDLK_p))
    {
        if (game->state == PLAY) game->state = PAUSE;
        else if (game->state == PAUSE) game->state = PLAY;
    }

    if (game->state == PLAY)
    {
        Snake_Update(game);
    }

    if (game->state == GAMEOVER)
    {
        if (Input_KeyWasPressed(input, SDLK_y)) game->option = 1;
        if (Input_KeyWasPressed(input, SDLK_n)) game->option = 0;
    }

    if (game->state == NEW_HIGHSCORE)
    {
        if (Input_KeyWasPressed(input, SDLK_BACKSPACE))
        {
            if (input->textLength > 0)
                input->text[--input->textLength] = '?';
        }
        if (Input_KeyWasPressed(input, SDLK_RETURN))
        {
            if (input->textLength < 3)
                game->isInputTooShort = SDL_TRUE;
            game->option = 1;
        }
    }

}

void Game_Loop(Game* game)
{
    while (game->isRunning)
    {
        CalculateFPS(game, 30);

        ProcessInput(game);
        Update(game);
        Render(game);

    }
}
