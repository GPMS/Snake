#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

// Local files
#include "main.h"
#include "render.h"

int done = 0;

int processEvents(GameState *game);
void moveSnake(GameState *game);
void collisionCheck(GameState *game);
Body *newBody(Body *tail);
void InitPlayer(Player *player);
void deleteSnake(Body *head);
void loadHighScore(GameState *game);
void saveHighScore(const GameState *game);


int main(int argc, char **argv)
{
    GameState game;
    game.state = GAME;
    game.gameOver = 0;
    game.asw = 3;
    game.ok = 0;

    strcpy(game.text, "???");
    game.l = 0;

    loadHighScore(&game);


    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    game.window = SDL_CreateWindow(
        GNAME,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH * BLOCK_SIZE, HEIGHT * BLOCK_SIZE,
        0
    );
    game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);

    game.running = SDL_TRUE;

    InitPlayer(&game.player);

    game.apple.xGrid = 4;
    game.apple.yGrid = 10;

    srand((int)time(NULL));

    game.font = NULL;
    game.font = TTF_OpenFont("font/emulogic.ttf", 24);
    if (game.font == NULL)
    {
        printf("COULD NOT FIND FONT FILE!\n");
        SDL_Quit();
        return 1;
    }

    /* Event loop */
    while (game.running)
    {
        processEvents(&game);

        if (game.state == GAME)
        {
            while (game.player.partsDrawn < game.player.parts)
            {
                game.player.tail = newBody(game.player.tail);
                game.player.partsDrawn++;
            }

            moveSnake(&game);
            collisionCheck(&game);
        }

        doRender(&game);
        SDL_Delay(100);

        done = 0;
    }

    deleteSnake(game.player.head);

    TTF_CloseFont(game.font);

    SDL_DestroyWindow(game.window);
    SDL_DestroyRenderer(game.renderer);

    saveHighScore(&game);

    SDL_Quit();

    return 0;
}


void loadHighScore(GameState *game)
{
    FILE *fp = fopen("saveData.bin", "rb");
    int i;

    if (fp == NULL)
    {
        printf("NO SAVE FILE\n");
        for (i=0; i < 5; i++)
        {
            strcpy(game->highScores[i].name, "???");
            game->highScores[i].value = 0;
        }
    }
    else
    {
        for (i=0; i < 5; i++)
        {
            fseek(fp, (sizeof(Score) * i), SEEK_SET);
            fread(&game->highScores[i], sizeof(Score), 1, fp);
        }
        printf("SAVE DATA LOADED\n");
        fclose(fp);
    }
}


void saveHighScore(const GameState *game)
{
    FILE *fp = fopen("saveData.bin", "wb");
    int i;

    if (fp != NULL)
    {
        for(i=0; i < 5; i++)
        {
            fseek(fp, 0, SEEK_END);
            fwrite(&game->highScores[i], sizeof(Score), 5, fp);
        }
        printf("SAVED\n");
        fclose(fp);
    }
    else
    {
        printf("FILE OPEN ERROR\n");
    }
}

void InitPlayer(Player *player)
{
    player->head = NULL;
    player->tail = NULL;

    player->head = malloc(sizeof(Body));
    player->tail = player->head;

    player->pos = 0;
    player->direction = EAST;
    player->parts = INITIAL_SIZE;
    player->partsDrawn = 1;
    player->score = 0;

    player->head->xGrid = 5;
    player->head->yGrid = 10;
    player->head->next = NULL;
}

void ResetGame(GameState *game)
{
    game->gameOver = 0;
    game->asw = 3;
    game->ok = 0;

    strcpy(game->text, "???");
    game->l = 0;

    deleteSnake(game->player.head);

    InitPlayer(&game->player);

    game->apple.xGrid = 4;
    game->apple.yGrid = 10;
}


int processEvents(GameState *game)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_WINDOWEVENT_CLOSE:
                if (game->window)
                {
                    SDL_DestroyWindow(game->window);
                    game->window = NULL;
                    game->running = SDL_FALSE;
                }
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
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
                        if ((game->player.parts == 1 || game->player.direction != SOUTH) && done == 0)
                        {
                            game->player.direction = NORTH;
                            done = 1;
                        }
                        break;
                    case SDLK_DOWN:
                        if ((game->player.parts == 1 || game->player.direction != NORTH) && done == 0)
                        {
                            game->player.direction = SOUTH;
                            done = 1;
                        }
                        break;
                    case SDLK_RIGHT:
                        if ((game->player.parts == 1 || game->player.direction != WEST) && done ==  0)
                        {
                            game->player.direction = EAST;
                            done = 1;
                        }
                        break;
                    case SDLK_LEFT:
                        if ((game->player.parts == 1 || game->player.direction != EAST) && done == 0)
                        {
                            game->player.direction = WEST;
                            done = 1;
                        }
                        break;
                    case SDLK_y:
                        if (game->gameOver)
                            game->asw = 1;
                        break;
                    case SDLK_n:
                        if (game->gameOver)
                            game->asw = 0;
                        break;
                    case SDLK_BACKSPACE:
                        if (game->l > 0 && game->state == BOARD)
                            game->text[--game->l] = '?';
                        break;
                    case SDLK_RETURN:
                        if (game->state == BOARD)
                        {
                            if (game->l < 3)
                                game->ok = 3;
                            else
                                game->ok = 1;
                        }
                }
                break;
            case SDL_TEXTINPUT:
                if (game->state == BOARD)
                {
                    // Add new text onto the end of our text
                    if (game->l < 3)
                        game->text[game->l++] = event.edit.text[0];
                }
                break;
            case SDL_TEXTEDITING:
                if (game->state == BOARD)
                {
                    // Update the composition text.
                    // Update the cursor position.
                    // Update the selection length (if any).
                    game->composition = event.edit.text;
                    game->cursor = event.edit.start;
                    game->selection_len = event.edit.length;
                }
                break;
            case SDL_QUIT:
                game->running = SDL_FALSE;
                break;
        }
    }
    return done;
}


void moveSnake(GameState *game)
{
    Player *player = &game->player;

    // Move head
    switch(player->direction)
    {
        case NORTH:
            player->head->pastYGrid = player->head->yGrid;
            player->head->pastXGrid = player->head->xGrid;
            player->head->yGrid--;
            break;
        case SOUTH:
            player->head->pastYGrid = player->head->yGrid;
            player->head->pastXGrid = player->head->xGrid;
            player->head->yGrid++;
            break;
        case EAST:
            player->head->pastYGrid = player->head->yGrid;
            player->head->pastXGrid = player->head->xGrid;
            player->head->xGrid++;
            break;
        case WEST:
            player->head->pastYGrid = player->head->yGrid;
            player->head->pastXGrid = player->head->xGrid;
            player->head->xGrid--;
            break;
    }

    /* Move Body */
    Body *current = player->head->next;
    Body *previous = player->head;

    while (current != NULL)
    {
        current->pastXGrid = current->xGrid;
        current->xGrid = previous->pastXGrid;
        current->pastYGrid = current->yGrid;
        current->yGrid = previous->pastYGrid;

        previous = current;
        current = current->next;
    }
}


void shiftPlace(GameState *game, int place)
{

    int i;
    int side = 0;

    // Hold on to name and score
    char holdN[2][4] = {"",""}; // name
    int holdV[2]     = { 0, 0}; // score

    for (i = place; i < 4; i++)
    {

        holdV[side] = game->highScores[i+1].value;
        strcpy(holdN[side], game->highScores[i+1].name);

        if (i == place)
        {
            game->highScores[i+1].value = game->highScores[i].value;
            strcpy(game->highScores[i+1].name, game->highScores[i].name);
        }
        else
        {
            game->highScores[i+1].value = holdV[!side];
            strcpy(game->highScores[i+1].name, holdN[!side]);
        }

        side ^= 1;
    }
}


unsigned int randr(unsigned int min, unsigned int max)
{
    double scaled = (double)rand()/RAND_MAX;

    return (max - min +1)*scaled + min;
}


void snakeAppleCollision(Player *player, Apple *apple)
{
    if ( (player->head->xGrid == apple->xGrid) && (player->head->yGrid == apple->yGrid) )
    {
        if (player->score < (WIDTH*HEIGHT*10)-(INITIAL_SIZE+1)*10)
                player->score += 10;

        while (1)
        {
            // Change apple location
            apple->xGrid = randr(2, WIDTH-3);
            apple->yGrid = randr(3, HEIGHT-2);

            // Check if location doesn't overlap with the snake
            int ok = 1;
            if ( (player->head->xGrid == apple->xGrid) && (player->head->yGrid == apple->yGrid) )
                ok = 0;

            Body *current = player->head;

            while(current != NULL)
            {
                if ( (apple->xGrid == current->xGrid) && (apple->yGrid == current->yGrid) )
                {
                    ok = 0;
                    break;
                }

                current = current->next;
            }

            if (ok) break;
        }

        // Add body parts
        player->parts++;
    }
}

void snakeSnakeCollision(GameState *game)
{
    Player *player = &game->player;

    Body *current = player->head->next;
    while (current != NULL)
    {
        if ( (player->head->xGrid == current->xGrid) && (player->head->yGrid == current->yGrid) )
        {
            if (player->score >= game->highScores[4].value)
            {
                for (int i = 4; i > -1; i--)
                {
                    if (player->score < game->highScores[i].value)
                    {
                        shiftPlace(game, i+1);

                        strcpy( game->highScores[i+1].name, "---");
                        game->highScores[i+1].value = player->score;
                        break;
                    }
                    else if (player->score == game->highScores[i].value)
                    {
                        shiftPlace(game, i);

                        strcpy( game->highScores[i].name, "---");
                        game->highScores[i].value = player->score;
                        break;
                    }
                    else if (i == 0)
                    {
                        shiftPlace(game, 0);

                        strcpy( game->highScores[0].name, "---");
                        game->highScores[0].value = player->score;
                    }
                }
                game->state = BOARD;

            }
            else
            {
                game->state = GAMEOVER;
                break;
            }
        }
        current = current->next;
    }
}

void snakeWallCollision(Body *head)
{
    // up
    if (head->yGrid < 3)
    {
        head->pastYGrid = head->yGrid;
        head->yGrid = HEIGHT-2;
    }
    // down
    else if (head->yGrid > HEIGHT-2)
    {
        head->pastYGrid = head->yGrid;
        head->yGrid = 3;
    }
    // right
    if (head->xGrid > WIDTH-3)
    {
        head->pastXGrid = head->xGrid;
        head->xGrid = 2;
    }
    // left
    else if (head->xGrid < 2)
    {
        head->pastXGrid = head->xGrid;
        head->xGrid = WIDTH-3;
    }
}

void collisionCheck(GameState *game)
{
    snakeAppleCollision(&game->player, &game->apple);
    snakeSnakeCollision(game);
    snakeWallCollision(game->player.head);
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

    while(freeMe != NULL)
    {
        holdMe = freeMe->next;
        free(freeMe);
        freeMe = holdMe;
    }
}
