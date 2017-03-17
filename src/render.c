#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

#include "main.h"
#include "render.h"


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


SDL_Renderer *newHighscore(GameState *game, Body *head, Body **tail, Apple *apple)
{
    SDL_Renderer *renderer = game->renderer;
    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface *new = TTF_RenderText_Blended(game->font, "New Highscore!", white);
    game->label = SDL_CreateTextureFromSurface(renderer, new);
    SDL_Rect newRect = {(WIDTH*BLOCK_SIZE - new->w)/2, 15 * BLOCK_SIZE, new->w, new->h};
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
    SDL_Rect scrRect = {(WIDTH*BLOCK_SIZE - scr->w)/2, 16 * BLOCK_SIZE, scr->w, scr->h};
    SDL_RenderCopy(renderer, game->label, NULL, &scrRect);
    SDL_FreeSurface(scr);

    /* Prompt */
    SDL_Surface *prmt = TTF_RenderText_Blended(game->font, "INPUT YOUR NAME", white);
    game->label = SDL_CreateTextureFromSurface(renderer, prmt);
    SDL_Rect prmtRect = {(WIDTH*BLOCK_SIZE - prmt->w)/2, 17 * BLOCK_SIZE, prmt->w, prmt->h};
    SDL_RenderCopy(renderer, game->label, NULL, &prmtRect);
    SDL_FreeSurface(prmt);

    SDL_Surface *txt = TTF_RenderText_Blended(game->font, game->text, white);
    game->label = SDL_CreateTextureFromSurface(renderer, txt);
    SDL_Rect txtRect = {(WIDTH*BLOCK_SIZE - txt->w)/2, 18*BLOCK_SIZE, txt->w, txt->h};
    SDL_RenderCopy(renderer, game->label, NULL, &txtRect);
    SDL_FreeSurface(txt);

    //if (game->state == BOARD)   printf("BOARD\n");

    game->pos = placeNum - 1;

    if (game->ok) {
		strcpy(game->highScores[game->pos].name, game->text);
		ResetGame(game, head, tail, apple);
		game->state = GAME;
    } else if (game->ok == 3) {
		SDL_Surface *warn = TTF_RenderText_Blended(game->font, "AT LEAST 3 CHARACTERS", white);
		game->label = SDL_CreateTextureFromSurface(renderer, warn);
		SDL_Rect warnRect = {(WIDTH*BLOCK_SIZE - warn->w)/2, 19 * BLOCK_SIZE, warn->w, warn->h};
		SDL_RenderCopy(renderer, game->label, NULL, &warnRect);
		SDL_FreeSurface(warn);
    }

    return renderer;
}


SDL_Renderer *drawPlaces(SDL_Renderer *renderer, GameState game, int curPlace)
{
	char str[128] = "";
	SDL_Color colours[5] = {
		{192, 192, 192, 255},
		{255,   0,   0, 255},
		{255, 128,   0, 255},
		{255, 153, 153, 255},
		{255, 178, 102, 255}
	};
	
	if (curPlace == 5) {
		return renderer;
	}
    sprintf(str, "  %d  \t%5d\t %s", curPlace+1, game.highScores[curPlace].value, game.highScores[curPlace].name);
	
    SDL_Surface *place = TTF_RenderText_Blended(game.font, str, colours[curPlace]);
    game.label = SDL_CreateTextureFromSurface(renderer, place);
    SDL_Rect placeRect = {(WIDTH*BLOCK_SIZE - place->w)/2, (5+curPlace) * BLOCK_SIZE, place->w, place->h};
    SDL_RenderCopy(renderer, game.label, NULL, &placeRect);
    SDL_FreeSurface(place);
	
	curPlace+=1;
	
	return drawPlaces(renderer, game, curPlace);
}


SDL_Renderer *drawBoard(GameState *game, Body *head, Body **tail, Apple *apple)
{
    SDL_Renderer *renderer = game->renderer;
    SDL_Color lblue    =   { 51, 255, 255, 255};
    SDL_Color yellow   =   {255, 255,   0, 255};

    SDL_SetRenderDrawColor(renderer,
                           0, 0, 0,
                           255
                          );
    SDL_RenderClear(renderer);

    SDL_Surface *title = TTF_RenderText_Blended(game->font, "Highscore Board", lblue);
    game->label = SDL_CreateTextureFromSurface(renderer, title);
    SDL_Rect titleRect = {(WIDTH*BLOCK_SIZE - title->w)/2, 2 * BLOCK_SIZE, title->w, title->h};
    SDL_RenderCopy(renderer, game->label, NULL, &titleRect);
    SDL_FreeSurface(title);

    SDL_Surface *text = TTF_RenderText_Blended(game->font, "Place\tScore\tName", yellow);
    game->label = SDL_CreateTextureFromSurface(renderer, text);
    SDL_Rect textRect = {(WIDTH*BLOCK_SIZE - text->w)/2, 4 * BLOCK_SIZE, text->w, text->h};
    SDL_RenderCopy(renderer, game->label, NULL, &textRect);
    SDL_FreeSurface(text);

    renderer = drawPlaces(renderer, *game, 0);

    renderer = newHighscore(game, head, tail, apple);

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

    /* Continue? */
	game->gameOver = 1;
    SDL_Surface *prmt = TTF_RenderText_Blended(game->font, "CONTINUE? (Y/N)", white);
	game->label = SDL_CreateTextureFromSurface(renderer, prmt);
	SDL_Rect prmtRect = {(WIDTH*BLOCK_SIZE - prmt->w)/2, 20 * BLOCK_SIZE, prmt->w, prmt->h};
	SDL_RenderCopy(renderer, game->label, NULL, &prmtRect);
	SDL_FreeSurface(prmt);

	switch(game->asw) {
		case 0:
			game->running = SDL_FALSE;
			break;
		case 1:
			game->gameOver = 0;
			game->state = GAME;
			break;
	}

    return renderer;
}


void doRender(GameState *game, Body *head, Body **tail, Apple *apple)
{
    SDL_Renderer *renderer = game->renderer;

    switch(game->state) {
        case GAME:
            renderer = drawGame(game, head, *apple);
            break;
        case PAUSE:
            renderer = drawPause(game);
            break;
        case GAMEOVER:
            renderer = drawGameOver(game);
            break;
        case BOARD:
        	renderer = drawBoard(game, head, tail, apple);
            break;
    }

    SDL_RenderPresent(renderer);
}
