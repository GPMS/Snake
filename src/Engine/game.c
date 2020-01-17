#include "game.h"
#include "../render.h"

/**
 * @brief Inits SDL and TTF
 */
void SetUpSDL(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init: %s", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() < 0)
    {
        printf("TTF_Init: %s", TTF_GetError());
        exit(1);
    }
}

/**
 * @brief Aditional error handling for TTF_OpenFont
 *
 * @param file  file to load font from
 * @param size  size of the font
 *
 * @return font pointer
 */
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

/**
 * @brief Loads all necessary resources into SDL
 *
 * @param game     Game struct
 *
 * @see LoadFont
 */
void LoadResources(Game* game)
{
    game->numFonts = 1;
    game->fonts[0] = LoadFont("resources/font.ttf", 24);
}

Game* Game_Create(const char* title,
                  const unsigned int width,
                  const unsigned int height)
{
    SetUpSDL();

    Game* game = (Game*) calloc(1, sizeof(Game));

    game->blockSize = 25;

    game->window = SDL_CreateWindow(title,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    width * game->blockSize,
                                    height * game->blockSize,
                                    0);


    game->width = width;
    game->height = height;

    if (!game->window)
    {
        printf("SDL_CreateWindow: %s", SDL_GetError());
        exit(1);
    }

    game->renderer = SDL_CreateRenderer(game->window, 0,
                                        SDL_RENDERER_ACCELERATED);
    if (!game->renderer)
    {
        printf("SDL_CreateRenderer: %s", SDL_GetError());
        exit(1);
    }

    game->isRunning = SDL_TRUE;

    LoadResources(game);

    Game_OnStart(game);

    return game;
}

void Game_Destroy(Game* game)
{
    Game_OnFinish(game);

    for (int i = 0; i < game->numFonts; i++)
        TTF_CloseFont(game->fonts[i]);
    TTF_Quit();

    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    free(game);

    SDL_Quit();
}

/**
 * @brief Keeps the FPS within bounds
 *
 * @param game      Game struct
 * @param targetFPS Maximum FPS value
 */
void LimitFPS(Game* game,
              const unsigned int targetFPS)
{
    static Uint32 lastTime = 0;
    Uint32 currentTime = SDL_GetTicks();

    Uint32 dt = currentTime - lastTime;
    Uint32 targetTime = (1.0f / targetFPS)*1000;

    if (dt < targetTime)
    {
        Uint32 delayTime = targetTime - dt;
        SDL_Delay(delayTime);
    }

    Uint32 timeAfterDelay = SDL_GetTicks();

    game->dt = timeAfterDelay - lastTime;
    game->fps = 1.0f / (game->dt / 1000.0f);

    lastTime = currentTime;
}


/**
 * @brief Deals with SDL events
 *
 * @param game  Game struct
 */
void HandleWindowEvents(Game* game)
{
    Input_EmptyList(&game->input);
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                game->isRunning = 0;
                break;
                break;
        }
        Input_Handle(game, &event);
    }
}

void Game_Loop(Game* game)
{
    while (game->isRunning)
    {
        LimitFPS(game, 15);

        HandleWindowEvents(game);
        Logic(game);
        Render(game);
    }
}