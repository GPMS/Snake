#ifndef RENDER_H
#define RENDER_H


SDL_Renderer *drawGame(SDL_Renderer *renderer, Body *head, Apple apple, GameState *game);
SDL_Renderer *drawPause(SDL_Renderer *renderer, GameState *game);
SDL_Renderer *drawGameOver(SDL_Renderer *renderer, GameState *game);
void doRender(SDL_Renderer *renderer, Body *head, Apple apple, GameState *game);


#endif
