#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/* main Header file */
#include "main.h"
/* sub Headers */
#include "render.h"

int done = 0;

int processEvents(SDL_Window *window, GameState *game);
void moveSnake(GameState game, Body *head);
unsigned int randr(unsigned int min, unsigned int max);
void collisionCheck(GameState *game, Body *head, Apple *apple);
Body *newBody(Body *tail);
void deleteSnake(Body *head);


int main(int argc, char **argv)
{
    GameState game;
    game.state = GAME;
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    
    window = SDL_CreateWindow("Game Window",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WIDTH * BLOCK_SIZE, HEIGHT * BLOCK_SIZE,
                              0
                             );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    game.running = SDL_TRUE;
    
    Body *head = NULL;
    Body *tail = NULL;
    
    head = malloc(sizeof(Body));
    tail = head;
    
    srandom((int)time(NULL));
    
    game.direction = EAST;
    game.parts = INITIAL_SIZE;
    game.partsDrawn = 1;
    game.score = 0;
    
    game.font = NULL;
    game.font = TTF_OpenFont("emulogic.ttf", 24);
    if (game.font == NULL) {
        printf("COULD NOT FIND FONT FILE!\n");
        SDL_Quit();
        return 1;
    }
    
    head->xGrid = 5;
    head->yGrid = 10;
    head->next = NULL;
    
    Apple apple;    
    apple.xGrid = 2;
    apple.yGrid = 10;

    /* Event loop */
    while (game.running) {
        processEvents(window, &game);
        
        if (game.state == GAME) {
            while (game.partsDrawn < game.parts) {
                tail = newBody(tail);
                game.partsDrawn++;
            }
            moveSnake(game, head);
            collisionCheck(&game, head, &apple);
        }
        
        doRender(renderer, head, apple, &game);
        
        SDL_Delay(100);

        done = 0;
    }
    
    deleteSnake(head);
    
    SDL_DestroyTexture(game.label);
    TTF_CloseFont(game.font);
    
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    SDL_Quit();
    
    return 0;
}



int processEvents(SDL_Window *window, GameState *game)
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_WINDOWEVENT_CLOSE:
                if (window) {
                    SDL_DestroyWindow(window);
                    window = NULL;
                    game->running = SDL_FALSE;
                }
                break;
        
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        game->running = SDL_FALSE;
                        break;
                    case SDLK_p:
                        if (game->state == GAME)
                            game->state = PAUSE;
                        else if (game->state == PAUSE)
                            game->state = GAME;
                        
                        break;
                    case SDLK_UP:
                        if ((game->parts == 1 || game->direction != SOUTH) && done == 0) {
                            game->direction = NORTH;
                            done = 1;
                        }
                        break;
                    case SDLK_DOWN:
                        if ((game->parts == 1 || game->direction != NORTH) && done == 0) {
                            game->direction = SOUTH;
                            done = 1;
                        }
                        break;
                    case SDLK_RIGHT:
                        if ((game->parts == 1 || game->direction != WEST) && done ==  0){
                            game->direction = EAST;
                            done = 1;
                        }
                        break;
                    case SDLK_LEFT:
                        if ((game->parts == 1 || game->direction != EAST) && done == 0) {
                            game->direction = WEST;
                            done = 1;
                        }
                        break;
                }
                break;
        
            case SDL_QUIT:
                game->running = SDL_FALSE;
                break;
        }
    }
    
    return done;
}


void moveSnake(GameState game, Body *head)
{
    /* Move Head */
    switch(game.direction) {
        case NORTH:
            head->pastYGrid = head->yGrid;
            head->pastXGrid = head->xGrid;
            head->yGrid--;
            break;
        case SOUTH:
            head->pastYGrid = head->yGrid;
            head->pastXGrid = head->xGrid;
            head->yGrid++;
            break;
        case EAST:
            head->pastYGrid = head->yGrid;
            head->pastXGrid = head->xGrid;
            head->xGrid++;
            break;
        case WEST:
            head->pastYGrid = head->yGrid;
            head->pastXGrid = head->xGrid;
            head->xGrid--;
            break;
    }
    
    /* Move Body */
    Body *current = head->next;
    Body *previous = head;
    
    while (current != NULL) {
        current->pastXGrid = current->xGrid;
        current->xGrid = previous->pastXGrid;
        current->pastYGrid = current->yGrid;
        current->yGrid = previous->pastYGrid;
        
        previous = current;
        current = current->next;
    }
}

unsigned int randr(unsigned int min, unsigned int max)
{
       double scaled = (double)rand()/RAND_MAX;

       return (max - min +1)*scaled + min;
}

void collisionCheck(GameState *game, Body *head, Apple *apple)
{   
    /* Apple collision */
    if ( (head->xGrid == apple->xGrid) && (head->yGrid == apple->yGrid) ) {
        
        if (game->score < WIDTH*HEIGHT*10-INITIAL_SIZE+1*10)
            game->score += 10;
        
        while (1) {
            // Change apple location
            apple->xGrid = randr(2, WIDTH-3);
            apple->yGrid = randr(3, HEIGHT-2);
            
            // Check if location doesn't overlap with the snake
            int ok = 1;
            if ( (head->xGrid == apple->xGrid) && (head->yGrid == apple->yGrid) )
                ok = 0;
            
            Body *current = head;
            Body *previous = head;
        
            while(current != NULL) {
                if ( (apple->xGrid == current->xGrid) && (apple->yGrid == current->yGrid) ) {
                    ok = 0;
                    break;
                }
                
                previous = current;
                current = current->next;
            }
            
            if (ok) break;
        }
        
        game->parts++;  // Add body parts
    }
    
    /* Body collision */
    Body *current = head->next;
    Body *previous = head->next;
    
    while (current != NULL) {
        if ( (head->xGrid == current->xGrid) && (head->yGrid == current->yGrid) ) {
            game->state = GAMEOVER;
            break;
        }
        
        previous = current;
        current = current->next;
    }
    
    /* Outside boundary */
    
    // up
    if (head->yGrid < 3) {
        head->pastYGrid = head->yGrid;
        head->yGrid = HEIGHT-2;
    // down
    } else if (head->yGrid > HEIGHT-2) {
        head->pastYGrid = head->yGrid;
        head->yGrid = 3;
    }
    // right
    if (head->xGrid > WIDTH-3) {
        head->pastXGrid = head->xGrid;
        head->xGrid = 2;
    // left
    } else if (head->xGrid < 2) {
        head->pastXGrid = head->xGrid;
        head->xGrid = WIDTH-3;
    }
}


Body *newBody(Body *tail)
{
    Body *newBody = malloc(sizeof(Body));
    
    tail->next = newBody;
    
    newBody->next = NULL;
    newBody->xGrid = tail-> pastXGrid;
    newBody->yGrid = tail-> pastYGrid;
    
    return newBody;
}


void deleteSnake(Body *head)
{
    Body *freeMe = head;
    Body *holdMe = NULL;
    
    while(freeMe != NULL) {
        holdMe = freeMe->next;
        free(freeMe);
        freeMe = holdMe;
    }
}
