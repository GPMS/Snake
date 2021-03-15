#include "input.h"

#include <stdio.h>

#include "game.h"

static SDL_Event event;

SDL_bool KeyPress(SDL_KeyCode key)
{
    const SDL_KeyboardEvent* keyEvent = &event.key;

    if (keyEvent->keysym.sym == key && keyEvent->state == SDL_PRESSED && keyEvent->repeat == 0) {
        return SDL_TRUE;
    }

    return SDL_FALSE;
}

SDL_bool KeyHold(SDL_KeyCode key)
{
    const SDL_KeyboardEvent* keyEvent = &event.key;

    if (keyEvent->keysym.sym == key && keyEvent->state == SDL_PRESSED && keyEvent->repeat != 0) {
        return SDL_TRUE;
    }

    return SDL_FALSE;
}

SDL_bool KeyRelease(SDL_KeyCode key)
{
    const SDL_KeyboardEvent* keyEvent = &event.key;

    if (keyEvent->keysym.sym == key && keyEvent->state == SDL_RELEASED) {
        return SDL_TRUE;
    }

    return SDL_FALSE;
}

static void HandleKeyEvents(Game* game)
{
    if (KeyRelease(SDLK_ESCAPE)) {
        game->isRunning = SDL_FALSE;
    }

    if (KeyRelease(SDLK_p)) {
        if (game->state == PLAY)
            game->state = PAUSE;
        else if (game->state == PAUSE)
            game->state = PLAY;
    }

    if (game->state == PLAY) {
        Snake_HandleInput(game);
    } else if (game->state == GAMEOVER) {
        if (KeyRelease(SDLK_y)) {
            game->option = 1;
        } else if (KeyRelease(SDLK_n)) {
            game->option = 0;
        }
    } else if (game->state == NEW_HIGHSCORE) {
        if (KeyRelease(SDLK_RETURN)) {
            if (game->textLength < 3)
                game->isInputTooShort = SDL_TRUE;
            game->option = 1;
        } else if (KeyPress(SDLK_BACKSPACE) || KeyHold(SDLK_BACKSPACE)) {
            if (game->textLength > 0)
                game->text[--game->textLength] = '?';
        }
    }
}

void Input_Process(Game* game)
{
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                game->isRunning = SDL_FALSE;
                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
                HandleKeyEvents(game);
                break;

            case SDL_TEXTINPUT:
                if (game->textLength < 3)
                    game->text[game->textLength++] = event.edit.text[0];
                break;
        }
    }
}
