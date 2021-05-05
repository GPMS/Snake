#include "game.h"

#include <stdio.h>

#include "input.h"
#include "render.h"
#include "window.h"

static void InitSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init: %s", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() < 0) {
        fprintf(stderr, "TTF_Init: %s", TTF_GetError());
        exit(1);
    }
}

static void QuitSDL()
{
    TTF_Quit();
    SDL_Quit();
}

// Wrapper for TTF_OpenFont
static TTF_Font* LoadFont(const char* file,
                          const int   size)
{
    TTF_Font* font = TTF_OpenFont(file, size);
    if (!font) {
        fprintf(stderr, "Unable to open %s: %s\n", file, TTF_GetError());
        SDL_Quit();
    }
    return font;
}

// Loads all necessary resources into SDL
static void LoadResources(Game* game)
{
    game->font = LoadFont("resources/font.ttf", 24);
}

void Game_Reset(Game* game)
{
    game->state = PLAY;

    game->option = -1;
    strcpy(game->text, "---");
    game->textLength = 0;

    game->isRunning = SDL_TRUE;

    game->score       = 0;
    game->place       = -1;
    game->initialSize = 4;

    Snake_Reset(game);
}

Game* Game_Create()
{
    Game* game = malloc(sizeof(Game));

    InitSDL();

    game->rows = game->cols = 26;

    game->window = Window_Create("Engine",
                                 game->cols * BLOCK_SIZE,
                                 game->rows * BLOCK_SIZE,
                                 0);

    LoadResources(game);
    Game_Reset(game);
    Highscore_Load(game);

    return game;
}

void Game_Destroy(Game* game)
{
    Highscore_Save(game);

    TTF_CloseFont(game->font);
    TTF_Quit();

    Window_Destroy(game->window);

    free(game);

    QuitSDL();
}

// Updates game struct with FPS info
// If targetFPS is >0 this will also limit it
static void CalculateFPS(Game* game, const unsigned int targetFPS)
{
    static Uint32 lastTime    = 0;
    Uint32        currentTime = SDL_GetTicks();

    Uint32 dt  = currentTime - lastTime;
    float  fps = 1.0f / (dt / 1000.0f);

    lastTime = currentTime;

    if (targetFPS > 0) {
        float targetTime = (1.0f / targetFPS) * 1000;

        Uint32 delayTime = 0;
        if (dt < targetTime) {
            delayTime = targetTime - dt;
            SDL_Delay(delayTime);

            dt += delayTime;
            fps = 1.0f / (dt / 1000.0f);
            lastTime += delayTime;
        }
    }
    game->dt  = dt;
    game->fps = fps;
}

static void Update(Game* game)
{
    // quit
    if (IsKeyReleased(SDL_SCANCODE_ESCAPE)) {
        game->isRunning = SDL_FALSE;
    }

    // Toggle play/pause
    if (IsKeyReleased(SDL_SCANCODE_P)) {
        if (game->state == PLAY)
            game->state = PAUSE;
        else if (game->state == PAUSE)
            game->state = PLAY;
    }

    switch (game->state) {
        case PLAY:
            Snake_Update(game);
            break;
        case GAMEOVER:
            // Continue?
            if (IsKeyReleased(SDL_SCANCODE_Y)) {
                game->option = 1;
            } else if (IsKeyReleased(SDL_SCANCODE_N)) {
                game->option = 0;
            }
            break;
        case NEW_HIGHSCORE:
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
                        strcpy(game->highScores[game->place].name, game->text);
                    }
                }
            }

            // Delete one character from name
            if (IsKeyPressed(SDL_SCANCODE_BACKSPACE) || IsKeyHeld(SDL_SCANCODE_BACKSPACE)) {
                if (game->textLength > 0) {
                    game->text[--game->textLength] = '-';
                    strcpy(game->highScores[game->place].name, game->text);
                }
            }

            // Confirm name
            if (IsKeyReleased(SDL_SCANCODE_RETURN)) {
                game->option = 1;
            }
            break;
        default:;
    }
}

void Game_Loop(Game* game)
{
    while (game->isRunning) {
        CalculateFPS(game, 30);

        game->isRunning = Input_Poll(game);
        Update(game);
        Render(game);
    }
}
