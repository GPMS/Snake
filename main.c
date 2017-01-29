#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


#define WIDTH       26 // BLOCK_SIZEs
#define HEIGHT      26 // BLOCK_SIZEs   
#define NAME        "Snake"
#define BLOCK_SIZE  25


enum dir { NORTH, EAST, WEST, SOUTH };

typedef struct Body{
    int pastXGrid, pastYGrid;
    int xGrid, yGrid;
    struct Body *next;
} Body;

typedef struct{
    int xGrid, yGrid;
} Apple;

typedef struct{
    SDL_bool running;
    
    /* Player */
    int direction;
    int parts;
    int partsDrawn;
    int score;
    
    SDL_Texture *label;
    TTF_Font *font;

} GameState;


int processEvents(SDL_Window *window, GameState *game);
void doRender(SDL_Renderer *renderer, Body *head, Apple apple, GameState *game);
void moveSnake(GameState game, Body *head);
unsigned int randr(unsigned int min, unsigned int max);
void collisionCheck(GameState *game, Body *head, Apple *apple);
Body *newBody(Body *tail);
void deleteSnake(Body *head);


int main(int argc, char **argv)
{
    GameState game;
    
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
    game.parts = 3;
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
        
        while (game.partsDrawn < game.parts) {
            tail = newBody(tail);
            game.partsDrawn++;
        }
        
        doRender(renderer, head, apple, &game);
        moveSnake(game, head);
        collisionCheck(&game, head, &apple);
        
        SDL_Delay(120);
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
    int done = 0;
    
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
                    case SDLK_UP:
                        if (game->parts == 1 || game->direction != SOUTH)
                            game->direction = NORTH;
                        break;
                    case SDLK_DOWN:
                        if (game->parts == 1 || game->direction != NORTH)
                            game->direction = SOUTH;
                        break;
                    case SDLK_RIGHT:
                        if (game->parts == 1 || game->direction != WEST)
                            game->direction = EAST;
                        break;
                    case SDLK_LEFT:
                        if (game->parts == 1 || game->direction != EAST)
                            game->direction = WEST;
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


void doRender(SDL_Renderer *renderer, Body *head, Apple apple, GameState *game)
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
    
    SDL_RenderPresent(renderer);
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
        
        if (game->score < 1000000)
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
            printf("GAME OVER\n");
            game->running = SDL_FALSE;
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
