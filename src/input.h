#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

struct Game;

bool IsKeyDown(SDL_Scancode key);
bool IsKeyUp(SDL_Scancode key);
bool IsKeyPressed(SDL_Scancode key);
bool IsKeyReleased(SDL_Scancode key);
bool IsKeyHeld(SDL_Scancode key);
bool HasTextInput();

// returns null terminated string with the input
const char* GetTextInput();

// Poll window events. Returns true if the window is still open
bool Input_Poll(struct Game* game);
