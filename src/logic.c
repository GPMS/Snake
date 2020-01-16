#include "logic.h"

void HandleInput(Game* game)
{
    Input* input = &game->input;

    if (Input_KeyWasReleased(input, SDLK_ESCAPE))
        game->isRunning = 0;

    switch (game->state)
    {
        case GAME:
            if (Input_KeyWasPressed(input, SDLK_UP))
            {
                if (game->player.parts == 1 || game->player.direction != SOUTH)
                    game->player.direction = NORTH;
            }
            else if (Input_KeyWasPressed(input, SDLK_DOWN))
            {
                if (game->player.parts == 1 || game->player.direction != NORTH)
                    game->player.direction = SOUTH;
            }
            else if (Input_KeyWasPressed(input, SDLK_LEFT))
            {
                if (game->player.parts == 1 || game->player.direction != EAST)
                    game->player.direction = WEST;
            }
            else if (Input_KeyWasPressed(input, SDLK_RIGHT))
            {
                if (game->player.parts == 1 || game->player.direction != WEST)
                    game->player.direction = EAST;
            }
            else if (Input_KeyWasPressed(input, SDLK_p))
            {
                game->state = PAUSE;
            }
            break;
        case PAUSE:
            if (Input_KeyWasPressed(input, SDLK_p))
                game->state = GAME;
            break;
        case GAMEOVER:
            if (Input_KeyWasPressed(input, SDLK_y))
                input->asw = 1;
            else if (Input_KeyWasPressed(input, SDLK_n))
                input->asw = 0;
            break;
        case HIGHSCORE:
            if (Input_KeyWasPressed(input, SDLK_BACKSPACE) ||
                Input_KeyIsBeingHeld(input, SDLK_BACKSPACE))
            {
                if (input->textLength > 0)
                    input->text[--input->textLength] = '?';
            }
            if (Input_KeyWasPressed(input, SDLK_RETURN))
            {
                if (input->textLength < 3)
                    input->isTooShort = SDL_TRUE;

                input->asw = 1;
            }
            break;
        default:
            break;
    }
}

unsigned int RangedRandom(unsigned int min, unsigned int max)
{
    double scaled = (double)rand()/RAND_MAX;

    return (max - min +1)*scaled + min;
}


void GetRandomApplePos(Game* game)
{
    Player* player = &game->player;
    Apple* apple = &game->apple;

    while (1)
    {
        // Change apple location
        apple->xGrid = RangedRandom(2, game->width-3);
        apple->yGrid = RangedRandom(3, game->height-2);

        // Check if location doesn't overlap with the snake
        SDL_bool isOverlapping = SDL_FALSE;
        Body* current = player->head;

        while(current != NULL)
        {
            if ((apple->xGrid == current->xGrid) &&
                (apple->yGrid == current->yGrid))
            {
                isOverlapping = SDL_TRUE;
                break;
            }

            current = current->next;
        }

        if (!isOverlapping) return;
    }
}

void PlayerAppleCollision(Game* game)
{
    Player* player = &game->player;
    Apple* apple = &game->apple;

    if ((player->head->xGrid == apple->xGrid) &&
        (player->head->yGrid == apple->yGrid))
    {
        int gamePlayArea = 20 * 20;
        int scoreGain = 10; // Amount of points earned for eating an apple
        int maxPossibleScore = (gamePlayArea - game->player.initialSize)
                               * scoreGain;

        if (player->score < maxPossibleScore)
                player->score += scoreGain;

        GetRandomApplePos(game);

        // Add body parts
        player->parts++;
    }
}

void PlayerPlayerCollision(Game* game)
{
    Player* player = &game->player;

    Body* current = player->head->next;
    while (current != NULL)
    {
        if ((player->head->xGrid == current->xGrid) &&
            (player->head->yGrid == current->yGrid))
        {
            if (player->score >= game->highScores[4].value)
            {
                Highscore_New(game);
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

void PlayerWallCollision(Game* game)
{
    Body* head = game->player.head;
    // up
    if (head->yGrid < 3)
    {
        head->pastYGrid = head->yGrid;
        head->yGrid = game->height-2;
    }
    // down
    else if (head->yGrid > game->height-2)
    {
        head->pastYGrid = head->yGrid;
        head->yGrid = 3;
    }
    // right
    if (head->xGrid > game->width-3)
    {
        head->pastXGrid = head->xGrid;
        head->xGrid = 2;
    }
    // left
    else if (head->xGrid < 2)
    {
        head->pastXGrid = head->xGrid;
        head->xGrid = game->width-3;
    }
}

void CollisionCheck(Game* game)
{
    PlayerAppleCollision(game);
    PlayerPlayerCollision(game);
    PlayerWallCollision(game);
}

void Game_Reset(Game* game)
{
    game->state = GAME;

    game->input.asw = -1;
    strcpy(game->input.text, "---");
    game->input.textLength = 0;
    game->input.isTooShort = SDL_FALSE;

    game->isRunning = SDL_TRUE;

    game->player.initialSize = 4;
    Player_Destroy(&game->player);
    Player_Reset(&game->player);

    GetRandomApplePos(game);
}

void Logic(Game* game)
{
    HandleInput(game);

    if (game->state == GAME)
    {
        while (game->player.partsDrawn < game->player.parts)
        {
            game->player.tail = Player_AddBody(game->player.tail);
            game->player.partsDrawn++;
        }

        // Calls MoveSnake every moveSpeed seconds
        static int calls = 0;
        calls++;
        float moveSpeed = 0.25;
        if (calls >= game->fps * moveSpeed)
        {
            calls = 0;
            Player_Move(game);
        }
        CollisionCheck(game);
    }
}
