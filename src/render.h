#pragma once

#include <SDL2/SDL.h>

struct Game;

// Draws the game
void Render(struct Game* game);

void SetDrawColor(SDL_Renderer*    renderer,
                  const SDL_Color* color);
