#include <string.h>

#include "draw.h"
#include "game.h"
#include "vector.h"
#include "window.h"
#include "highscore.h"

static void DrawBackground(const Game* game)
{
    SDL_Renderer* renderer = game->window->SDLRenderer;

    // Draw score
    char str[128] = "";

    sprintf(str, "score:%d", game->score);
    DrawText(game->window, &white, Vector2(2 * BLOCK_SIZE, BLOCK_SIZE), str, game->font);

    sprintf(str, "highest:%d", Highscore_GetHighestScore());
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
            game->isRunning = false;
            break;
        case 1:
            Game_Reset(game);
            break;
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
            Highscore_Draw(game);
            break;
    }

    SDL_RenderPresent(game->window->SDLRenderer);
}
