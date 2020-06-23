#include "window.h"


Window* Window_Create(const char* title,
                      const unsigned int width,
                      const unsigned int height,
                      enum WindowFlags currentFlags)
{
    Window* window = (Window*) calloc(1, sizeof(Window));

    Uint32 flags = 0;

    if (currentFlags & FULLSCREEN)
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

    if (currentFlags & INVISIBLE)
        flags |= SDL_WINDOW_HIDDEN;

    if (currentFlags & BORDERLESS)
        flags |= SDL_WINDOW_BORDERLESS;

    window->SDLWindow = SDL_CreateWindow(title,
                                         SDL_WINDOWPOS_UNDEFINED,
                                         SDL_WINDOWPOS_UNDEFINED,
                                         width, height,
                                         flags);

    window->width = width;
    window->height = height;

    if (!window->SDLWindow)
    {
        fprintf(stderr, "SDL_CreateWindow: %s", SDL_GetError());
        exit(1);
    }

    window->SDLRenderer = SDL_CreateRenderer(window->SDLWindow, 0,
                                             SDL_RENDERER_ACCELERATED);
    if (!window->SDLRenderer)
    {
        fprintf(stderr, "SDL_CreateRenderer: %s", SDL_GetError());
        exit(1);
    }

    return window;
}

void Window_Destroy(Window* window)
{

    SDL_DestroyRenderer(window->SDLRenderer);
    SDL_DestroyWindow(window->SDLWindow);
    free(window);
}

