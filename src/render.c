#include <string.h>

#include "draw.h"
#include "game.h"
#include "vector.h"
#include "window.h"

static void DrawBackground(const Game* game)
{
    SDL_Renderer* renderer = game->window->SDLRenderer;

    // Draw score
    char str[128] = "";

    sprintf(str, "score:%d", game->score);
    DrawText(game->window, &white, Vector2(2 * BLOCK_SIZE, BLOCK_SIZE), str, game->font);

    sprintf(str, "highest:%d", game->highScores[0].value);
    DrawText(game->window, &white, Vector2((game->cols - 13) * BLOCK_SIZE, BLOCK_SIZE), str, game->font);

    // Draw main screen
    DrawFillSquare(renderer, &grey, Vector2(2 * BLOCK_SIZE, 3 * BLOCK_SIZE), 22 * BLOCK_SIZE);
}

static void DrawGameScreen(const Game* game)
{
    DrawBackground(game);

    // Draw apple
    DrawFillSquare(game->window->SDLRenderer, &red, Vector2_Mul(game->applePos, BLOCK_SIZE), BLOCK_SIZE - 5);

    Snake_Draw(game);
}

static void DrawPauseScreen(const Game* game)
{
    DrawBackground(game);

    DrawText(game->window, &white, Vector2(CENTERED, 5 * BLOCK_SIZE), "PAUSED", game->font);

    DrawText(game->window, &white, Vector2(CENTERED, CENTERED), "Press p to continue", game->font);
}

static void DrawGameOverScreen(Game* game)
{
    DrawText(game->window, &white, Vector2(CENTERED, 5 * BLOCK_SIZE), "GAME OVER", game->font);

    char str[128];
    sprintf(str, "Score: %d", game->score);
    DrawText(game->window, &white, Vector2(CENTERED, 7 * BLOCK_SIZE), str, game->font);

    DrawText(game->window, &white, Vector2(CENTERED, CENTERED), "Continue? (Y/N)", game->font);

    switch (game->option) {
        case 0:
            game->isRunning = SDL_FALSE;
            break;
        case 1:
            Game_Reset(game);
            break;
    }
}

static void DrawPlaces(const Game* game, int curPlace)
{
    char str[128] = "";

    SDL_Color colours[5] = {lightGrey,
                            red,
                            orange,
                            lightPink,
                            lightOrange};

    if (curPlace == 5)
        return;

    sprintf(str, "  %d  \t%5d\t %s", curPlace + 1, game->highScores[curPlace].value, game->highScores[curPlace].name);

    DrawText(game->window, &colours[curPlace], Vector2(CENTERED, (5 + curPlace) * BLOCK_SIZE), str, game->font);

    DrawPlaces(game, ++curPlace);
}

static void DrawHighscoreScreen(Game* game)
{
    DrawText(game->window, &lightBlue, Vector2(CENTERED, 2 * BLOCK_SIZE), "Highscore Board", game->font);
    DrawText(game->window, &yellow, Vector2(CENTERED, 4 * BLOCK_SIZE), "Place\tScore\tName", game->font);

    DrawPlaces(game, 0);
}

static void DrawNewHighscore(Game* game)
{
    DrawText(game->window, &white, Vector2(CENTERED, 15 * BLOCK_SIZE), "New Highscore!", game->font);

    /* Show place and score */
    char place[4];
    switch (game->place + 1) {
        case 1:
            snprintf(place, 4, "1st");
            break;
        case 2:
            snprintf(place, 4, "2nd");
            break;
        case 3:
            snprintf(place, 4, "3rd");
            break;
        default:
            snprintf(place, 4, "%dth", game->place + 1);
            break;
    }
    char str[11] = "";
    sprintf(str, "%s place", place);

    DrawText(game->window, &white, Vector2(CENTERED, 16 * BLOCK_SIZE), str, game->font);

    /* Prompt */
    DrawText(game->window, &white, Vector2(CENTERED, 17 * BLOCK_SIZE), "Input your name", game->font);

    DrawText(game->window, &white, Vector2(CENTERED, 18 * BLOCK_SIZE), game->text, game->font);

    if (game->option == 1) {
        if (game->isInputTooShort) {
            DrawText(game->window, &white, Vector2(CENTERED, 19 * BLOCK_SIZE), "At least 3 characters", game->font);
            return;
        }
        strcpy(game->highScores[game->place].name, game->text);
        Game_Reset(game);
    }
}

void Render(Game* game)
{
    ClearScreen(game->window->SDLRenderer, &black);

    switch (game->state) {
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

    if (game->isInputTooShort) {
        SDL_Delay(500);
        game->isInputTooShort = SDL_FALSE;
        game->option          = -1;
    }

    SDL_RenderPresent(game->window->SDLRenderer);
}
