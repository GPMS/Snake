#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>


#define WIDTH       640
#define HEIGHT      480
#define NAME        "Snake"
#define BLOCK_SIZE  25


enum dir { NORTH, EAST, WEST, SOUTH };

typedef struct Body{
    int pastX, pastY;
    int x, y;
    struct Body *next;
} Body;

typedef struct{
    int x, y;
} Apple;

typedef struct{
    SDL_bool running;
    
    /* Player */
    int direction;
    int parts;
    int partsDrawn;
    int score;

} GameState;


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


void doRender(SDL_Renderer *renderer, Body *head, Apple apple)
{
    /* Draw black background */
    SDL_SetRenderDrawColor(renderer,
                           0, 0, 0,
                           255
                          );
    SDL_RenderClear(renderer);
    
    /* Draw apple */
    SDL_SetRenderDrawColor(renderer,
                       255, 0, 0,
                       255
                      );
    SDL_Rect appleRect = { apple.x, apple.y, BLOCK_SIZE-5, BLOCK_SIZE-5 };
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
        SDL_Rect snakeRect = { current->x, current->y, BLOCK_SIZE-5, BLOCK_SIZE-5 };
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
            head->pastY = head->y;
            head->pastX = head->x;
            head->y -= BLOCK_SIZE;
            break;
        case SOUTH:
            head->pastY = head->y;
            head->pastX = head->x;
            head->y += BLOCK_SIZE;
            break;
        case EAST:
            head->pastY = head->y;
            head->pastX = head->x;
            head->x += BLOCK_SIZE;
            break;
        case WEST:
            head->pastY = head->y;
            head->pastX = head->x;
            head->x -= BLOCK_SIZE;
            break;
    }
    
    /* Move Body */
    Body *current = head->next;
    Body *previous = head;
    
    while (current != NULL) {
        current->pastX = current->x;
        current->x = previous->pastX;
        current->pastY = current->y;
        current->y = previous->pastY;
        
        previous = current;
        current = current->next;
    }
}


int collision2D(int x1, int y1,
                int x2, int y2)
{
    if ( (x1 == x2)  && (y1 == y2) ) {
        return 1;
    }
    
    return 0;
}


void collisionCheck(GameState *game, Body *head, Apple *apple)
{
    int blockX = WIDTH / BLOCK_SIZE;
    int blockY = HEIGHT / BLOCK_SIZE;
    
    /* Apple collision */
    if (collision2D(head->x, head->y, apple->x, apple->y)) {
        while (1) {
		    // Change apple location
		    apple->x = (random() % blockX) * BLOCK_SIZE;
		    apple->y = (random() % blockY) * BLOCK_SIZE;
		    
		    // Check if location doesn't overlap with the snake
			int ok = 1;
			if (apple->x == head->x && apple->y == head->y)
				ok = 0;
			
			Body *current = head;
			Body *previous = head;
		
			while(current != NULL) {
				if (apple->x == current->x && apple->y == current->y) {
					ok = 0;
					break;
				}
				
				previous = current;
				current = current->next;
			}
			
			if (ok)
				break;
		}
        
        // Add body parts
        game->parts++;
    }
    
    /* Body collision */
    Body *current = head->next;
    Body *previous = head->next;
    
    while (current != NULL) {
        if (collision2D(head->x, head->y, current->x, current->y)) {
            printf("GAME OVER\n");
            game->running = SDL_FALSE;
            break;
        }
        
        previous = current;
        current = current->next;
    }
    
    /* Outside boundary */
    int outside = 0;
    
    //up or down
    if (head->y < 0 || (head->y + BLOCK_SIZE) > HEIGHT)
        outside = 1;
    //right or left
    if ((head->x + BLOCK_SIZE) > WIDTH || head->x < 0)
        outside = 1;
    
    if (outside) {
        printf("GAME OVER\n");
        game->running = SDL_FALSE;
    }
}


Body *newBody(Body *tail)
{
    Body *newBody = malloc(sizeof(Body));
    
    tail->next = newBody;
    
    newBody->next = NULL;
    newBody->x = tail-> pastX;
    newBody->y = tail-> pastY;
    
    return newBody;
}


int main(int argc, char **argv)
{
    GameState game;

    SDL_Window *window;
    SDL_Renderer *renderer;
    
    SDL_Init(SDL_INIT_VIDEO);
    
    window = SDL_CreateWindow("Game Window",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              640, 480,
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
    game.parts = 1;
    game.partsDrawn = 1;
    
    head->x = 8 * BLOCK_SIZE;
    head->y = 8 * BLOCK_SIZE;
    head->next = NULL;
    
    Apple apple;
    apple.x = 2*BLOCK_SIZE;
    apple.y = 8*BLOCK_SIZE;

    /* Event loop */
    while (game.running) {
        processEvents(window, &game);
        
        if (game.parts != game.partsDrawn) {
            tail = newBody(tail);
            game.partsDrawn++;
        }
        
        doRender(renderer, head, apple);
        moveSnake(game, head);
        collisionCheck(&game, head, &apple);
        
        SDL_Delay(120);
    }
    
    free(head);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    SDL_Quit();
    
    return 0;
}
