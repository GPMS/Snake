#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "main.h"


SDL_Renderer *drawGame(GameState *game, Body *head, Apple apple)
{
    SDL_Renderer *renderer = game->renderer;
    SDL_Color white = {255, 255, 255, 255};
    
    /* Draw black background */
    SDL_SetRenderDrawColor(renderer,
                           0, 0, 0,
                           255
                          );
    SDL_RenderClear(renderer);
    
    /* Draw score */
    char str[128] = "";
    sprintf(str, "score:%d", game->score);
    
    SDL_Surface *score = TTF_RenderText_Blended(game->font, str, white);
    game->label = SDL_CreateTextureFromSurface(renderer, score);
    SDL_Rect textRect = {2*BLOCK_SIZE, BLOCK_SIZE, score->w, score->h};
    SDL_RenderCopy(renderer, game->label, NULL, &textRect);
    SDL_FreeSurface(score);
    
    sprintf(str, "highest:%d", game->highScores[0].value);
    
    SDL_Surface *highest = TTF_RenderText_Blended(game->font, str, white);
    game->label = SDL_CreateTextureFromSurface(renderer, highest);
    SDL_Rect highestRect = {(WIDTH-13)*BLOCK_SIZE, BLOCK_SIZE, highest->w, highest->h};
    SDL_RenderCopy(renderer, game->label, NULL, &highestRect);
    SDL_FreeSurface(highest);
    
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


SDL_Renderer *drawPause(GameState *game)
{
    SDL_Renderer *renderer = game->renderer;
    SDL_Color white = {255, 255, 255, 255};
    
    /* Clear playground */
    // Draw black background
    SDL_SetRenderDrawColor(renderer,
                           0, 0, 0,
                           255
                          );
    SDL_RenderClear(renderer);
    
    // Draw score
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
    SDL_RenderCopy(game->renderer, game->label, NULL, &textRect);
    SDL_FreeSurface(tmp3);
    
    return renderer;
}


SDL_Renderer *drawBoard(GameState *game)
{
    SDL_Renderer *renderer = game->renderer;
    SDL_Color white = {255, 255, 255, 255};
    
    SDL_RenderClear(renderer);
    
    SDL_Surface *title = TTF_RenderText_Blended(game->font, "Highscore Board", white);
    game->label = SDL_CreateTextureFromSurface(renderer, title);
    SDL_Rect titleRect = {(WIDTH*BLOCK_SIZE - title->w)/2, 2 * BLOCK_SIZE, title->w, title->h};
    SDL_RenderCopy(renderer, game->label, NULL, &titleRect);
    SDL_FreeSurface(title);
    
    SDL_Surface *text = TTF_RenderText_Blended(game->font, "Place\tScore\tName", white);
    game->label = SDL_CreateTextureFromSurface(renderer, text);
    SDL_Rect textRect = {2, 4 * BLOCK_SIZE, text->w, text->h};
    SDL_RenderCopy(renderer, game->label, NULL, &textRect);
    SDL_FreeSurface(text);
    
    char str[128] = "";
    sprintf(str, "  %d  \t%5d\t %s", 1, game->highScores[0].value, game->highScores[0].name);
    
    SDL_Surface *place1 = TTF_RenderText_Blended(game->font, str, white);
    game->label = SDL_CreateTextureFromSurface(renderer, place1);
    SDL_Rect place1Rect = {2, 5 * BLOCK_SIZE, place1->w, place1->h};
    SDL_RenderCopy(renderer, game->label, NULL, &place1Rect);
    SDL_FreeSurface(place1);
    
    sprintf(str, "  %d  \t%5d\t %s", 2, game->highScores[1].value, game->highScores[1].name);
    
    SDL_Surface *place2 = TTF_RenderText_Blended(game->font, str, white);
    game->label = SDL_CreateTextureFromSurface(renderer, place2);
    SDL_Rect place2Rect = {2, 6 * BLOCK_SIZE, place2->w, place2->h};
    SDL_RenderCopy(renderer, game->label, NULL, &place2Rect);
    SDL_FreeSurface(place2);
    
    sprintf(str, "  %d  \t%5d\t %s", 3, game->highScores[2].value, game->highScores[2].name);
    
    SDL_Surface *place3 = TTF_RenderText_Blended(game->font, str, white);
    game->label = SDL_CreateTextureFromSurface(renderer, place3);
    SDL_Rect place3Rect = {2, 7 * BLOCK_SIZE, place3->w, place3->h};
    SDL_RenderCopy(renderer, game->label, NULL, &place3Rect);
    SDL_FreeSurface(place3);
    
    sprintf(str, "  %d  \t%5d\t %s", 4, game->highScores[3].value, game->highScores[3].name);
    
    SDL_Surface *place4 = TTF_RenderText_Blended(game->font, str, white);
    game->label = SDL_CreateTextureFromSurface(renderer, place4);
    SDL_Rect place4Rect = {2, 8 * BLOCK_SIZE, place4->w, place4->h};
    SDL_RenderCopy(renderer, game->label, NULL, &place4Rect);
    SDL_FreeSurface(place4);
    
    sprintf(str, "  %d  \t%5d\t %s", 5, game->highScores[4].value, game->highScores[4].name);
    
    SDL_Surface *place5 = TTF_RenderText_Blended(game->font, str, white);
    game->label = SDL_CreateTextureFromSurface(renderer, place5);
    SDL_Rect place5Rect = {2, 9 * BLOCK_SIZE, place5->w, place5->h};
    SDL_RenderCopy(renderer, game->label, NULL, &place5Rect);
    SDL_FreeSurface(place5);
    
    return renderer;
}


SDL_Renderer *newHighscore(GameState *game)
{	
    SDL_Renderer *renderer = game->renderer;
    SDL_Color white = {255, 255, 255, 255};
    
    SDL_Surface *new = TTF_RenderText_Blended(game->font, "New Highscore!", white);
    game->label = SDL_CreateTextureFromSurface(renderer, new);
    SDL_Rect newRect = {(WIDTH*BLOCK_SIZE - new->w)/2, 10 * BLOCK_SIZE, new->w, new->h};
    SDL_RenderCopy(renderer, game->label, NULL, &newRect);
    SDL_FreeSurface(new);
    
    
    /* Show place and score */
    char place[4];
    
    int i;
    int placeNum;
    for (i = 4; i > -1; i--) {
        if (game->score < game->highScores[i].value) {
            placeNum = i+2;
            break;
        } else {
            placeNum = 1;
        }
    }
    
    switch(placeNum) {
        case 1:
            place[1] = 's';
            place[2] = 't';
            break;
        case 2:
            place[1] = 'n';
            place[2] = 'd';
            break;
        case 3:
            place[1] = 'r';
            place[2] = 'd';
            break;
        default:
            place[1] = 't';
            place[2] = 'h';
            break;
    }
    place[0] = placeNum + '0';
    
    char str[128] = "";
    sprintf(str, "%s place", place);
    
    SDL_Surface *scr = TTF_RenderText_Blended(game->font, str, white);
    game->label = SDL_CreateTextureFromSurface(renderer, scr);
    SDL_Rect scrRect = {(WIDTH*BLOCK_SIZE - scr->w)/2, 11 * BLOCK_SIZE, scr->w, scr->h};
    SDL_RenderCopy(renderer, game->label, NULL, &scrRect);
    SDL_FreeSurface(scr);
    
    /* Prompt */
    SDL_Surface *prmt = TTF_RenderText_Blended(game->font, "Do you want to save? (Y/N)", white);
    game->label = SDL_CreateTextureFromSurface(renderer, prmt);
    SDL_Rect prmtRect = {(WIDTH*BLOCK_SIZE - prmt->w)/2, 20 * BLOCK_SIZE, prmt->w, prmt->h};
    SDL_RenderCopy(renderer, game->label, NULL, &prmtRect);
    SDL_FreeSurface(prmt);
    
    switch(game->asw) {
    	case 0:
    		game->state = BOARD;
    		break;
    	case 1:
    		game->state = SAVE;
    		break;
    }
    
    return renderer;
}

SDL_Renderer *drawGameOver(GameState *game)
{
    SDL_Renderer *renderer = game->renderer;
    SDL_Color white = {255, 255, 255, 255};
    
    /* Draw black background */
    SDL_SetRenderDrawColor(renderer,
                           0, 0, 0,
                           255
                          );
    SDL_RenderClear(renderer);
    
    /* Write gameover */
    SDL_Surface *over = TTF_RenderText_Blended(game->font, "GameOver", white);
    game->label = SDL_CreateTextureFromSurface(renderer, over);
    SDL_Rect overRect = {(WIDTH*BLOCK_SIZE - over->w)/2, 5 * BLOCK_SIZE, over->w, over->h};
    SDL_RenderCopy(renderer, game->label, NULL, &overRect);
    SDL_FreeSurface(over);
    
    char str[128];
    
    sprintf(str, "Score: %d", game->score);
    
    SDL_Surface *pts = TTF_RenderText_Blended(game->font, str, white);
    game->label = SDL_CreateTextureFromSurface(renderer, pts);
    SDL_Rect ptsRect = {(WIDTH*BLOCK_SIZE - pts->w)/2, 7 * BLOCK_SIZE, pts->w, pts->h};
    SDL_RenderCopy(renderer, game->label, NULL, &ptsRect);
    SDL_FreeSurface(pts);
    
    if (game->score > game->highScores[4].value) {
        game->state = PROMPT;
    }
    
    return renderer;
}


void doRender(GameState *game, Body *head, Apple apple)
{
    SDL_Renderer *renderer = game->renderer;
    
    switch(game->state) {
        case GAME:
            renderer = drawGame(game, head, apple);
            break;
        case PAUSE:
            renderer = drawPause(game);
            break;
        case GAMEOVER:
            renderer = drawGameOver(game);
            break;
        case PROMPT:
        	renderer = newHighscore(game);
        	break;
        case SAVE:
            break;
        case BOARD:
        	renderer = drawBoard(game);
            break;
    }
    SDL_RenderPresent(renderer);
}
