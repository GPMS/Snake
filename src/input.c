#include "input.h"

#include <stdio.h>

#include "game.h"

#define NUM_KEYS 285  // Taken from SDL Scancode Lookup Table
static bool           isKeyDown[NUM_KEYS]   = {false};
static bool           wasKeyDown[NUM_KEYS]  = {false};
static unsigned       timePressed[NUM_KEYS] = {0};  // SDL timestamp for when the key was pressed
static const unsigned DELAY                 = 300;  // time in ms needed to recognize key being held
static bool           hasTextInput          = false;
static char           textInput[32]         = "";

bool IsKeyDown(SDL_Scancode key)
{
    return isKeyDown[key];
}

bool IsKeyUp(SDL_Scancode key)
{
    return !isKeyDown[key];
}

bool IsKeyPressed(SDL_Scancode key)
{
    return isKeyDown[key] && !wasKeyDown[key];
}

bool IsKeyReleased(SDL_Scancode key)
{
    return !isKeyDown[key] && wasKeyDown[key];
}

bool IsKeyHeld(SDL_Scancode key)
{
    if (!isKeyDown[key])
        return false;

    if (SDL_GetTicks() - timePressed[key] < DELAY) {
        return false;
    }

    return true;
}

bool HasTextInput()
{
    return hasTextInput;
}

const char* GetTextInput()
{
    return textInput;
}

bool Input_Poll(Game* game)
{
    hasTextInput = false;
    for (int i = 0; i < NUM_KEYS; i++) {
        wasKeyDown[i] = isKeyDown[i];
    }

    SDL_Event event;
    unsigned  textInputLength = 0;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return false;
                break;
            case SDL_KEYDOWN:
                if (!isKeyDown[event.key.keysym.scancode]) {
                    isKeyDown[event.key.keysym.scancode]   = true;
                    timePressed[event.key.keysym.scancode] = SDL_GetTicks();
                }
                break;
            case SDL_KEYUP:
                if (isKeyDown[event.key.keysym.scancode]) {
                    isKeyDown[event.key.keysym.scancode]   = false;
                    timePressed[event.key.keysym.scancode] = 0;
                }
                break;
            case SDL_TEXTINPUT:
                hasTextInput               = true;
                textInput[textInputLength] = event.edit.text[textInputLength];
                textInputLength++;
                break;
        }
    }
    return true;
}
