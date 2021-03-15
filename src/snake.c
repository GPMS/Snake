#include "snake.h"

#include "game.h"
#include "input.h"
#include "highscore.h"

#include "Engine/draw.h"
#include "Engine/window.h"

#include <stdlib.h>

unsigned int RangedRandom(unsigned int min, unsigned int max)
{
    double scaled = (double)rand() / RAND_MAX;

    return (max - min + 1) * scaled + min;
}

void GetRandomApplePos(Game* game)
{
    Snake* snake = &game->snake;
    Vec2*  apple = &game->applePos;

    int maxRow = game->rows - 2;
    int minRow = 3;
    int maxCol = game->cols - 3;
    int minCol = 2;

    while (1) {
        // Change apple location
        *apple = Vector2(RangedRandom(minCol, maxCol),
                         RangedRandom(minRow, maxRow));

        // Check if location doesn't overlap with the snake
        SDL_bool isOverlapping = SDL_FALSE;
        Body*    current       = snake->head;

        while (current != NULL) {
            if ((apple->x == current->pos.x) && (apple->y == current->pos.y)) {
                isOverlapping = SDL_TRUE;
                break;
            }

            current = current->next;
        }

        if (!isOverlapping)
            return;
    }
}

void Snake_Reset(Game* game)
{
    Snake* snake = &game->snake;

    snake->head = calloc(1, sizeof(Body));
    snake->tail = snake->head;

    snake->head->pos  = Vector2(5, 10);
    snake->head->next = NULL;

    snake->direction  = EAST;
    snake->parts      = game->initialSize;
    snake->partsDrawn = 1;

    GetRandomApplePos(game);
}

Body* Snake_AddBody(Body* tail)
{
    Body* newBody = malloc(sizeof(Body));

    tail->next = newBody;

    newBody->next = NULL;
    newBody->pos  = tail->pastPos;

    return newBody;
}

void Snake_Destroy(Snake* snake)
{
    Body* freeMe = snake->head;
    Body* holdMe = NULL;

    while (freeMe != NULL) {
        holdMe = freeMe->next;
        free(freeMe);
        freeMe = holdMe;
    }
}

void Snake_HandleInput(Game* game)
{
    Snake* snake = &game->snake;

    if (KeyRelease(SDLK_UP)) {
        if (snake->parts == 1 || snake->direction != SOUTH)
            snake->direction = NORTH;
    } else if (KeyRelease(SDLK_DOWN)) {
        if (snake->parts == 1 || snake->direction != NORTH)
            snake->direction = SOUTH;
    } else if (KeyRelease(SDLK_LEFT)) {
        if (snake->parts == 1 || snake->direction != EAST)
            snake->direction = WEST;
    } else if (KeyRelease(SDLK_RIGHT)) {
        if (snake->parts == 1 || snake->direction != WEST)
            snake->direction = EAST;
    }
}

void AppleCollision(Game* game)
{
    Snake* snake = &game->snake;
    Vec2   apple = game->applePos;

    if ((snake->head->pos.x == apple.x) && (snake->head->pos.y == apple.y)) {
        int gamePlayArea     = game->rows * game->cols;
        int scoreGain        = 10;
        int maxPossibleScore = (gamePlayArea - game->initialSize)
                               * scoreGain;

        if (game->score < maxPossibleScore)
            game->score += scoreGain;

        GetRandomApplePos(game);

        // Add body parts
        snake->parts++;
    }
}

void BodyCollision(Game* game)
{
    Snake* snake = &game->snake;

    Body* current = snake->head->next;
    while (current != NULL) {
        if ((snake->head->pos.x == current->pos.x) && (snake->head->pos.y == current->pos.y)) {
            if (game->score >= game->highScores[4].value) {
                Highscore_New(game);
            } else {
                game->state = GAMEOVER;
                break;
            }
        }
        current = current->next;
    }
}

void WallCollision(Game* game)
{
    Body* head = game->snake.head;

    int maxRow = game->rows - 2;
    int minRow = 3;
    int maxCol = game->cols - 3;
    int minCol = 2;

    // up
    if (head->pos.y < minRow) {
        head->pastPos.y = head->pos.y;
        head->pos.y     = maxRow;
    }
    // down
    else if (head->pos.y > maxRow) {
        head->pastPos.y = head->pos.y;
        head->pos.y     = minRow;
    }
    // right
    if (head->pos.x > maxCol) {
        head->pastPos.x = head->pos.x;
        head->pos.x     = minCol;
    }
    // left
    else if (head->pos.x < minCol) {
        head->pastPos.x = head->pos.x;
        head->pos.x     = maxCol;
    }
}

void CollisionCheck(Game* game)
{
    AppleCollision(game);
    BodyCollision(game);
    WallCollision(game);
}

void Move(Game* game)
{
    Snake* snake = &game->snake;

    // MOVE HEAD
    snake->head->pastPos = snake->head->pos;
    switch (snake->direction) {
        case NORTH:
            snake->head->pos.y--;
            break;
        case SOUTH:
            snake->head->pos.y++;
            break;
        case EAST:
            snake->head->pos.x++;
            break;
        case WEST:
            snake->head->pos.x--;
            break;
    }

    // MOVE BODY
    Body* current  = snake->head->next;
    Body* previous = snake->head;

    // Loop through the body of the snake, update the past
    // position of the current part and move it to the
    // past position of the previous part
    while (current != NULL) {
        current->pastPos = current->pos;
        current->pos     = previous->pastPos;

        previous = current;
        current  = current->next;
    }
}

void Snake_Update(Game* game)
{
    Snake* snake = &game->snake;

    while (snake->partsDrawn < snake->parts) {
        // Dont spawn a body part outside of the playing area
        if (snake->tail->pos.x == 0 && snake->tail->pos.y == 0) {
            break;
        }

        snake->tail = Snake_AddBody(snake->tail);
        snake->partsDrawn++;
    }

    // Move every moveSpeed miliseconds
    static int time = 0;
    time += game->dt;
    int moveSpeed = 80;
    if (time >= moveSpeed) {
        Move(game);
        time = 0;
    }

    CollisionCheck(game);
}

void Snake_Draw(const Game* game)
{
    SDL_Renderer* renderer = game->window->SDLRenderer;

    int size = BLOCK_SIZE - 5;

    Body* current = game->snake.head;

    while (current != NULL) {
        DrawFillSquare(renderer, &green, Vector2_Mul(current->pos, BLOCK_SIZE), size);

        current = current->next;
    }
}