#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

struct Game;

SDL_bool KeyPress(SDL_KeyCode key);
SDL_bool KeyHold(SDL_KeyCode key);
SDL_bool KeyRelease(SDL_KeyCode key);

// Adds Input event to a list so that it can be processed later
void Input_Process(struct Game* game);

#endif
