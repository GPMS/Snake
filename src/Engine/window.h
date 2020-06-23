#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


enum WindowFlags {
    FULLSCREEN = 0x1,
    INVISIBLE = 0x2,
    BORDERLESS = 0x4
};

typedef struct Window {
    SDL_Window*     SDLWindow;
    SDL_Renderer*   SDLRenderer;
    unsigned int    width;
    unsigned int    height;
} Window;

Window* Window_Create(const char* title,
                      const unsigned int width,
                      const unsigned int height,
                      enum WindowFlags currentFlags);

void Window_Destroy(Window* window);
#endif

