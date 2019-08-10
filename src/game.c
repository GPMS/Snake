#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "render.h"

/**
 * @brief Inits SDL and TTF
 */
void SetUpSDL(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init: %s", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() < 0)
    {
        printf("TTF_Init: %s", TTF_GetError());
        exit(1);
    }
}

void InitSnake(Game* game)
{
    Player* player = &game->player;

    player->head = NULL;
    player->tail = NULL;

    player->head = malloc(sizeof(Body));
    player->tail = player->head;

    player->pos = 0;
    player->direction = EAST;
    player->parts = game->initialSize;
    player->partsDrawn = 1;
    player->score = 0;

    player->head->xGrid = 5;
    player->head->yGrid = 10;
    player->head->next = NULL;
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
        /* Change apple location */
        apple->xGrid = RangedRandom(2, game->width-3);
        apple->yGrid = RangedRandom(3, game->height-2);

        /* Check if location doesn't overlap with the snake */
        int ok = 1;
        Body* current = player->head;

        while(current != NULL)
        {
            if ((apple->xGrid == current->xGrid) &&
                (apple->yGrid == current->yGrid))
            {
                ok = 0;
                break;
            }

            current = current->next;
        }

        if (ok) return;
    }
}

/**
 * @brief Sets up SDL and creates a window and a renderer, filling up
 * default values for the Game struct
 *
 * @param title     Window title
 * @param width     Window width
 * @param height    Window height
 *
 * @return          The game struct
 *
 * @see SetUpSDL
 */
Game* CreateGame(const char* title,
                 const unsigned int width, const unsigned int height)
{
    SetUpSDL();

    Game* game = (Game*) malloc(sizeof(Game));

    game->blockSize = 25;

    game->window = SDL_CreateWindow(title,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    width * game->blockSize, height * game->blockSize,
                                    0);

    game->width = width;
    game->height = height;

    if (!game->window)
    {
        printf("SDL_CreateWindow: %s", SDL_GetError());
        exit(1);
    }

    game->renderer = SDL_CreateRenderer(game->window, -1,
                                        SDL_RENDERER_ACCELERATED);
    if (!game->renderer)
    {
        printf("SDL_CreateRenderer: %s", SDL_GetError());
        exit(1);
    }

    srand((int)time(NULL));

    game->keyboard = SDL_GetKeyboardState(NULL);
    game->isRunning = SDL_TRUE;

    game->asw = 3;
    game->ok = 0;
    game->state = GAME;

    strcpy(game->text, "---");
    game->inputLength = 0;

    game->font = TTF_OpenFont("resources/font.ttf", 24);

    game->initialSize = 4;

    InitSnake(game);

    GetRandomApplePos(game);

    return game;
}

void ShiftPlace(Game* game, int place)
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

void SnakeAppleCollision(Game* game)
{
    Player* player = &game->player;
    Apple* apple = &game->apple;

    if ((player->head->xGrid == apple->xGrid) &&
        (player->head->yGrid == apple->yGrid))
    {
        if (player->score < (game->width*game->height*10)-(game->initialSize+1)*10)
                player->score += 10;

        GetRandomApplePos(game);

        // Add body parts
        player->parts++;
    }
}

void NewHighscore(Game* game)
{
    Player* player = &game->player;

    for (int i = 4; i > -1; i--)
    {
        if (player->score < game->highScores[i].value)
        {
            ShiftPlace(game, i+1);

            strcpy( game->highScores[i+1].name, "---");
            game->highScores[i+1].value = player->score;
            break;
        }
        else if (player->score == game->highScores[i].value)
        {
            ShiftPlace(game, i);

            strcpy( game->highScores[i].name, "---");
            game->highScores[i].value = player->score;
            break;
        }
        else if (i == 0)
        {
            ShiftPlace(game, 0);

            strcpy( game->highScores[0].name, "---");
            game->highScores[0].value = player->score;
        }
    }

    game->state = BOARD;
}

void LoadHighScore(Game* game)
{
    FILE* fp = fopen("saveData.bin", "rb");
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

void SaveHighScore(const Game* game)
{
    FILE* fp = fopen("saveData.bin", "wb");
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

void SnakeSnakeCollision(Game* game)
{
    Player* player = &game->player;

    Body* current = player->head->next;
    while (current != NULL)
    {
        if ((player->head->xGrid == current->xGrid) &&
            (player->head->yGrid == current->yGrid))
        {
            // Collision!

            if (player->score >= game->highScores[4].value)
            {
                NewHighscore(game);
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

void SnakeWallCollision(Game* game)
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
    SnakeAppleCollision(game);
    SnakeSnakeCollision(game);
    SnakeWallCollision(game);
}


Body *AddBody(Body *tail)
{
    Body *newBody = malloc(sizeof(Body));
    tail->next = newBody;

    newBody->next = NULL;
    newBody->xGrid = tail-> pastXGrid;
    newBody->yGrid = tail-> pastYGrid;

    return newBody;
}

void DeleteSnake(Body *head)
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

void ResetPlayer(Game* game)
{
    Player* player = &game->player;

    player->head = NULL;
    player->tail = NULL;

    player->head = malloc(sizeof(Body));
    player->tail = player->head;

    player->pos = 0;
    player->direction = EAST;
    player->parts = game->initialSize;
    player->partsDrawn = 1;
    player->score = 0;

    player->head->xGrid = 5;
    player->head->yGrid = 10;
    player->head->next = NULL;
}

void ResetGame(Game* game)
{
    game->gameOver = 0;
    game->asw = 3;
    game->ok = 0;
    game->state = GAME;

    strcpy(game->text, "---");
    game->inputLength = 0;

    game->isRunning = SDL_TRUE;

    DeleteSnake(game->player.head);
    InitSnake(game);
    GetRandomApplePos(game);
}


/**
 * @brief Deallocates memory and quits SDL
 *
 * @param game  Game struct
 */
void DestroyGame(Game* game)
{
    DeleteSnake(game->player.head);

    TTF_CloseFont(game->font);
    TTF_Quit();

    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);

    SaveHighScore(game);
    free(game);

    SDL_Quit();
}

/**
 * @brief Keeps the FPS within bounds
 *
 * @param game      Game struct
 * @param targetFPS Maximum FPS value
 */
void LimitFPS(Game* game, unsigned int targetFPS)
{
    static int lastTime = 0;
    int currentTime = SDL_GetTicks();

    float dt = currentTime - lastTime;
    float targetTime = (1.0f / targetFPS)*1000;
    float delayTime = 0;

    if (dt < targetTime)
    {
        delayTime = targetTime - dt;
        SDL_Delay(delayTime);
        dt += delayTime;
    }

    float fps = 1.0f / (dt / 1000.0f);
    game->fps = fps;
    game->dt = dt;

    lastTime = currentTime;
}

/**
 * @brief Deals with SDL events
 *
 * @param game  Game struct
 */
void GetInput(Game* game)
{
    SDL_Event* event = &game->event;
    while (SDL_PollEvent(event))
    {
        switch (event->type)
        {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                if (game->window)
                {
                    SDL_DestroyWindow(game->window);
                    game->window = NULL;
                }
                game->isRunning = SDL_FALSE;
                break;
            case SDL_TEXTINPUT:
                if (game->state == BOARD)
                {
                    // Add new text onto the end of our text
                    if (game->inputLength < 3)
                        game->text[game->inputLength++] = event->edit.text[0];
                }
                break;
            case SDL_TEXTEDITING:
                if (game->state == BOARD)
                {
                    // Update the composition text.
                    // Update the cursor position.
                    // Update the selection length (if any).
                    game->composition = event->edit.text;
                    game->cursor = event->edit.start;
                    game->selection_len = event->edit.length;
                }
                break;
        }
    }
}

void MoveSnake(Game* game)
{
    Player* player = &game->player;

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
    Body* current = player->head->next;
    Body* previous = player->head;

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

SDL_bool WasJustPressed(Game* game, Uint8 scancode)
{
    // Is the key being pressed?
    if (game->keyboard[scancode])
    {
        // Was it just pressed or is it being held?
        if (game->keyBeingHeld == scancode)
        {
            if (game->keyJustPressed == scancode)
                game->keyJustPressed = 0;
            return SDL_FALSE;
        }
        else
        {
            game->keyJustPressed = scancode;
            game->keyBeingHeld = scancode;
            return SDL_TRUE;
        }
    }
    else
    {
        // Was it just recently released?
        if (game->keyJustPressed == scancode) game->keyJustPressed = 0;
        if (game->keyBeingHeld == scancode) game->keyBeingHeld = 0;
    }


    return SDL_FALSE;
}

SDL_bool WantsToHold(Game* game, Uint8 scancode)
{
    static SDL_bool hasTimerStarted = SDL_FALSE;
    static SDL_bool isKeyintentionalyHeld = SDL_FALSE;
    static int timeStarted = 0;
    const int delayTime = 300;

    // Is key being held?
    if (game->keyBeingHeld == scancode)
    {
        // Has already went through the timer?
        if (isKeyintentionalyHeld)
            return SDL_TRUE;
        else
        {
            // Has The timer not started?
            if (!hasTimerStarted)
            {
                hasTimerStarted = SDL_TRUE;
                timeStarted = SDL_GetTicks();
            }
            // Has the timer finished?
            else if (SDL_GetTicks() - timeStarted >= delayTime)
            {
                hasTimerStarted = SDL_FALSE;
                timeStarted = 0;
                isKeyintentionalyHeld = SDL_TRUE;
                return SDL_TRUE;
            }
        }
    }
    else
    {
        if (isKeyintentionalyHeld)
        {
            isKeyintentionalyHeld = SDL_FALSE;
        }
        // Abort timer
        if (hasTimerStarted)
        {
            hasTimerStarted = SDL_FALSE;
            timeStarted = 0;
        }
    }


    return SDL_FALSE;
}

SDL_bool WasReleased(Game* game, Uint8 scancode)
{
    if (game->keyboard[scancode])
    {
        game->keyJustPressed = scancode;
    }
    else if (game->keyJustPressed == scancode)
    {
        game->keyJustPressed = 0;
        return SDL_TRUE;
    }
    return SDL_FALSE;
}

/**
 * @brief Updates game logic
 *
 * @param game  Game struct
 */
void Update(Game* game)
{
    if (game->keyboard[SDL_SCANCODE_ESCAPE])
        game->isRunning = 0;

    if (WasReleased(game, SDL_SCANCODE_P))
    {
        if (game->state == GAME)
            game->state = PAUSE;
        else if (game->state == PAUSE)
            game->state = GAME;
    }

    switch(game->state)
    {
        case GAME:
            while (game->player.partsDrawn < game->player.parts)
            {
                game->player.tail = AddBody(game->player.tail);
                game->player.partsDrawn++;
            }

            if (game->keyboard[SDL_SCANCODE_UP])
            {
                if (game->player.parts == 1 || game->player.direction != SOUTH)
                    game->player.direction = NORTH;
            }
            if (game->keyboard[SDL_SCANCODE_DOWN])
            {
                if (game->player.parts == 1 || game->player.direction != NORTH)
                    game->player.direction = SOUTH;
            }
            if (game->keyboard[SDL_SCANCODE_LEFT])
            {
                if (game->player.parts == 1 || game->player.direction != EAST)
                    game->player.direction = WEST;
            }
            if (game->keyboard[SDL_SCANCODE_RIGHT])
            {
                if (game->player.parts == 1 || game->player.direction != WEST)
                    game->player.direction = EAST;
            }

            if (game->keyboard[SDL_SCANCODE_Y])
                if (game->gameOver)
                    game->asw = 1;
            if (game->keyboard[SDL_SCANCODE_N])
                game->asw = 0;

            // Calls MoveSnake every moveSpeed seconds
            static int calls = 0;
            calls++;
            float moveSpeed = 0.25;
            if (calls >= game->fps * moveSpeed)
            {
                calls = 0;
                MoveSnake(game);
            }
            CollisionCheck(game);
            break;
        case BOARD:
            if (WasJustPressed(game, SDL_SCANCODE_BACKSPACE) ||
                WantsToHold(game, SDL_SCANCODE_BACKSPACE))
            {
                if (game->inputLength > 0)
                    game->text[--game->inputLength] = '?';
            }
            if (game->keyboard[SDL_SCANCODE_RETURN])
            {
                if (game->inputLength < 3)
                    game->ok = 3;
                else
                    game->ok = 1;
            }
    }
}

/**
 * @brief Game loop...
 *
 * @param game Game struct
 *
 * @see LimitFPS
 * @see GetInput
 * @see Update
 * @see Render
 */
void GameLoop(Game* game)
{
    LoadHighScore(game);

    while (game->isRunning)
    {
        LimitFPS(game, 30);

        GetInput(game);
        Update(game);

        Render(game);
    }
}
