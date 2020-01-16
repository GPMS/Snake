#include "player.h"

void Player_Reset(Player* player)
{
    player->head = calloc(1, sizeof(Body));
    player->tail = player->head;

    player->head->xGrid = 5;
    player->head->yGrid = 10;
    player->head->next = NULL;

    player->pos = 0;
    player->direction = EAST;
    player->parts = player->initialSize;
    player->partsDrawn = 1;
    player->score = 0;
}

Body* Player_AddBody(Body *tail)
{
    Body *newBody = malloc(sizeof(Body));
    tail->next = newBody;

    newBody->next = NULL;
    newBody->xGrid = tail-> pastXGrid;
    newBody->yGrid = tail-> pastYGrid;

    return newBody;
}

void Player_Destroy(Player *player)
{
    Body *freeMe = player->head;
    Body *holdMe = NULL;

    while(freeMe != NULL)
    {
        holdMe = freeMe->next;
        free(freeMe);
        freeMe = holdMe;
    }
}

void Player_Move(Game* game)
{
    Player* player = &game->player;

    // MOVE HEAD
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

    // MOVE BODY
    Body* current = player->head->next;
    Body* previous = player->head;

    // Loop through the body of the player and move each part
    // to the past position of the previous part and update
    // the past position of the current part
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