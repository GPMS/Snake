#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "main.h"


SDL_Renderer *drawGame(SDL_Renderer *renderer, Body *head, Apple apple, GameState *game)
{
    /* Draw black background */
    SDL_SetRenderDrawColor(renderer,
                           0, 0, 0,
                           255
                          );
    SDL_RenderClear(renderer);
    
    /* Draw score */
    SDL_Color white = {255, 255, 255, 255};
    
    char str[128] = "";
    sprintf(str, "score:%d", game->score);
    
    SDL_Surface *tmp = TTF_RenderText_Blended(game->font, str, white);
    game->label = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_Rect textRect = {BLOCK_SIZE, BLOCK_SIZE, tmp->w, tmp->h};
    SDL_RenderCopy(renderer, game->label, NULL, &textRect);
    SDL_FreeSurface(tmp);
    
    /* Draw main screen */
    SDL_SetRenderDrawColor(renderer,
                           50, 50, 50,
                           255
                          );
    SDL_Rect mainScreen = { 2*BLOCK_SIZE, 3*BLOCK_SIZE, 22*BLOCK_SIZE, 22*BLOCK_SIZE };
    SDL_RenderFillRect(renderer, &mainScreen);
    
    /* Draw apple */
    SDL_SetRenderDrawColor(renderer,
                       255, 0, 0,
                       255
                      );
    SDL_Rect appleRect = { apple.xGrid*BLOCK_SIZE, apple.yGrid*BLOCK_SIZE, BLOCK_SIZE-5, BLOCK_SIZE-5 };
    SDL_RenderFillRect(renderer, &appleRect);

    /* Draw snake */
    // Set Colour
    SDL_SetRenderDrawColor(renderer,
                       0, 255, 0,
                       255
                      );
    
    Body *current = head;
    
    // Draw snake
    while (current != NULL){
        SDL_Rect snakeRect = { current->xGrid*BLOCK_SIZE, current->yGrid*BLOCK_SIZE, BLOCK_SIZE-5, BLOCK_SIZE-5 };
        SDL_RenderFillRect(renderer, &snakeRect);
        
        current = current->next;
    }
    
    return renderer;

}


SDL_Renderer *drawPause(SDL_Renderer *renderer, GameState *game)
{
    /* Clear playground */
    // Draw black background
    SDL_SetRenderDrawColor(renderer,
                           0, 0, 0,
                           255
                          );
    SDL_RenderClear(renderer);
    
    // Draw score
    SDL_Color white = {255, 255, 255, 255};
    
    char str[128] = "";
    sprintf(str, "score:%d", game->score);
    
    SDL_Surface *tmp1 = TTF_RenderText_Blended(game->font, str, white);
    game->label = SDL_CreateTextureFromSurface(renderer, tmp1);
    SDL_Rect scoreRect = {BLOCK_SIZE, BLOCK_SIZE, tmp1->w, tmp1->h};
    SDL_RenderCopy(renderer, game->label, NULL, &scoreRect);
    SDL_FreeSurface(tmp1);
    
    // Draw main screen
    SDL_SetRenderDrawColor(renderer,
                           50, 50, 50,
                           255
                          );
    SDL_Rect mainScreen = { 2*BLOCK_SIZE, 3*BLOCK_SIZE, 22*BLOCK_SIZE, 22*BLOCK_SIZE };
    SDL_RenderFillRect(renderer, &mainScreen);
    
    /* Write 'Pause' */
    SDL_Surface *tmp2 = TTF_RenderText_Blended(game->font, "PAUSED", white);
    game->label = SDL_CreateTextureFromSurface(renderer, tmp2);
    SDL_Rect pauseRect = {10 * BLOCK_SIZE, 5 * BLOCK_SIZE, tmp2->w, tmp2->h};
    SDL_RenderCopy(renderer, game->label, NULL, &pauseRect);
    SDL_FreeSurface(tmp2);
    /* Write instructions */
    SDL_Surface *tmp3 = TTF_RenderText_Blended(game->font, "Press p to continue", white);
    game->label = SDL_CreateTextureFromSurface(renderer, tmp3);
    SDL_Rect textRect = {4 * BLOCK_SIZE, 10 * BLOCK_SIZE, tmp3->w, tmp3->h};
    SDL_RenderCopy(renderer, game->label, NULL, &textRect);
    SDL_FreeSurface(tmp3);
    
    return renderer;
}


SDL_Renderer *drawGameOver(SDL_Renderer *renderer, GameState *game)
{
    /* Draw black background */
    SDL_SetRenderDrawColor(renderer,
                           0, 0, 0,
                           255
                          );
    SDL_RenderClear(renderer);
    
    /* Write gameover */
    SDL_Color white = {255, 255, 255, 255};
    
    SDL_Surface *tmp = TTF_RenderText_Blended(game->font, "GameOver", white);
    game->label = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_Rect textRect = {9 * BLOCK_SIZE, 5 * BLOCK_SIZE, tmp->w, tmp->h};
    SDL_RenderCopy(renderer, game->label, NULL, &textRect);
    SDL_FreeSurface(tmp);
    
    return renderer;
}


void doRender(SDL_Renderer *renderer, Body *head, Apple apple, GameState *game)
{
    switch(game->state) {
        case GAME:
            renderer = drawGame(renderer, head, apple, game);
            break;
        case PAUSE:
            renderer = drawPause(renderer, game);
            break;
        case GAMEOVER:
            renderer = drawGameOver(renderer, game);
            break;
    }
    SDL_RenderPresent(renderer);
}
