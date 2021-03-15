#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "vector.h"

#define CENTERED -1

extern const SDL_Color white;
extern const SDL_Color black;
extern const SDL_Color red;
extern const SDL_Color green;
extern const SDL_Color blue;
extern const SDL_Color lightGrey;
extern const SDL_Color grey;
extern const SDL_Color lightPink;
extern const SDL_Color orange;
extern const SDL_Color lightOrange;
extern const SDL_Color lightBlue;
extern const SDL_Color yellow;

struct Window;

// Set draw color
void SetDrawColor(SDL_Renderer*    renderer,
                  const SDL_Color* color);

// Clear screen to color
void ClearScreen(SDL_Renderer*    renderer,
                 const SDL_Color* color);

void CopyTexture(SDL_Renderer* renderer,
                 SDL_Texture*  texture,
                 Vec2          position,
                 float         width,
                 float         height);

// Draw text on screen
void DrawText(struct Window*   window,
              const SDL_Color* color,
              Vec2             position,
              const char*      text,
              TTF_Font*        font);

// Draw rectangle filled with color
void DrawFillRect(SDL_Renderer*    renderer,
                  const SDL_Color* color,
                  Vec2             position,
                  int              width,
                  int              height);

// Draw square filled with color
void DrawFillSquare(SDL_Renderer*    renderer,
                    const SDL_Color* color,
                    Vec2             position,
                    int              size);
