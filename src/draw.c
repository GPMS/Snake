#include "draw.h"
#include "window.h"
#include <SDL2/SDL_render.h>

//                                 R    G    B    A
const SDL_Color white       = {255, 255, 255, 255};
const SDL_Color black       = {0, 0, 0, 255};
const SDL_Color red         = {255, 0, 0, 255};
const SDL_Color green       = {0, 255, 0, 255};
const SDL_Color blue        = {0, 0, 255, 255};
const SDL_Color lightGrey   = {192, 192, 192, 255};
const SDL_Color grey        = {50, 50, 50, 255};
const SDL_Color lightPink   = {255, 153, 153, 255};
const SDL_Color orange      = {255, 128, 0, 255};
const SDL_Color lightOrange = {255, 178, 102, 255};
const SDL_Color lightBlue   = {51, 255, 255, 255};
const SDL_Color yellow      = {255, 255, 0, 255};

void SetDrawColor(SDL_Renderer*    renderer,
                  const SDL_Color* color)
{
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
}

void ClearScreen(SDL_Renderer*    renderer,
                 const SDL_Color* color)
{
    SetDrawColor(renderer, color);
    SDL_RenderClear(renderer);
}

void CopyTexture(SDL_Renderer* renderer,
                 SDL_Texture*  texture,
                 Vec2          position,
                 float         width,
                 float         height)
{
    SDL_Rect rect = (SDL_Rect){(int)position.x, (int)position.y, (int)width, (int)height};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void DrawText(Window*          window,
              const SDL_Color* color,
              Vec2             position,
              const char*      text,
              TTF_Font*        font)
{
    SDL_Renderer* renderer = window->SDLRenderer;

    SDL_Surface* tmpSurface = TTF_RenderText_Blended(font, text, *color);
    float        width      = (float)tmpSurface->w;
    float        height     = (float)tmpSurface->h;

    SDL_Texture* label = SDL_CreateTextureFromSurface(renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);

    if (position.x == CENTERED)
        position.x = (window->width - width) / 2.0f;

    if (position.y == CENTERED)
        position.y = (window->height - height) / 2.0f;

    CopyTexture(renderer, label, position, width, height);
    SDL_DestroyTexture(label);
}

void DrawFillRect(SDL_Renderer*    renderer,
                  const SDL_Color* color,
                  Vec2             position,
                  int              width,
                  int              height)
{
    SetDrawColor(renderer, color);

    SDL_Rect rect;
    rect.x = position.x;
    rect.y = position.y;
    rect.w = width;
    rect.h = height;

    SDL_RenderFillRect(renderer, &rect);
}

void DrawFillSquare(SDL_Renderer*    renderer,
                    const SDL_Color* color,
                    Vec2             position,
                    int              size)
{
    DrawFillRect(renderer, color, position, size, size);
}
