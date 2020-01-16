#include "input.h"

void Input_Init(Input* input)
{
    //input->keysEvent = NULL;
}

/**
 * @brief Allocates memory for a new Key struct
 *
 * @param keycode   SDL_Keycode of the key
 * @param state     Is the key currently being pressed or no
 * @param repeat    Is the key being held or no
 *
 * @return The new Key struct
 */
Key* NewKey(const SDL_Keycode keycode,
            const Uint8 state,
            const Uint8 repeat)
{
    Key *key = malloc(sizeof(Key));

    key->keycode = keycode;
    key->state = state;
    key->repeat = repeat;
    key->nextKey = NULL;

    return key;
}

/**
 * @brief Allocates memory for a new Key event and adds it to a list
 *
 * @param input     Pointer needed to access the list where the key will
 *                  be stored
 * @param keycode   SDL_Keycode of the key
 * @param state     Is the key currently being pressed or no
 * @param repeat    Is the key being held or no
 *
 * @see NewKey
 */
void AddKey(Input* input,
            const SDL_Keycode keycode,
            const Uint8 state,
            const Uint8 repeat)
{
    if (input->keysEvent == NULL)
    {
        input->keysEvent = NewKey(keycode, state, repeat);
    }
    else
    {
        Key* curKey = input->keysEvent;
        Key* prevKey = NULL;

        while(curKey != NULL)
        {
            if (curKey->keycode == keycode)
            {
                curKey->state = state;
                curKey->repeat = repeat;
                return;
            }
            prevKey = curKey;
            curKey = curKey->nextKey;
        }

        prevKey->nextKey = NewKey(keycode, state, repeat);
    }
}

void Input_EmptyList(Input* input)
{
    Key* key = input->keysEvent;
    Key* nextKey = NULL;

    while (key != NULL)
    {
        nextKey = key->nextKey;
        free(key);
        key = nextKey;
    }

    input->keysEvent = NULL;
}

void Input_Handle(Game* game,
                  SDL_Event* event)
{
    Input* input = &game->input;
    switch (event->type)
    {
        // Key Events
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            ; // Cant declare variable after case... Weird
            SDL_KeyboardEvent key = event->key;
            AddKey(input, key.keysym.sym, key.state, key.repeat);
            break;
        case SDL_TEXTINPUT:
            if (game->state == HIGHSCORE)
            {
                // Add new text onto the end of our text
                if (input->textLength < 3)
                    input->text[input->textLength++] = event->edit.text[0];
            }
            break;
        case SDL_TEXTEDITING:
            if (game->state == HIGHSCORE)
            {
                // Update the composition text.
                // Update the cursor position.
                // Update the selection length (if any).
                input->composition = event->edit.text;
                input->cursor = event->edit.start;
                input->selection_len = event->edit.length;
            }
            break;
    }
}

/**
 * @brief Util function used to find a particular key from the Keys Event list
 *
 * @param input Needed to access the Keys Event list
 * @param key   SDL_Keycode of the key
 *
 * @return The Key if it was found, NULL otherwise
 */
Key* GetKey(const Input* input,
            const SDL_Keycode key)
{
    Key* curKey = input->keysEvent;

    while (curKey != NULL)
    {
        if (curKey->keycode == key)
        {
            return curKey;
        }
        curKey = curKey->nextKey;
    }
    return NULL;
}

SDL_bool Input_KeyWasPressed(const Input* input,
                             const SDL_Keycode key)
{
    Key* keyStruct = GetKey(input, key);

    if (!keyStruct)
        return SDL_FALSE;

    if (keyStruct->state == SDL_PRESSED && keyStruct->repeat == 0)
        return SDL_TRUE;

    return SDL_FALSE;
}

SDL_bool Input_KeyIsBeingHeld(const Input* input,
                              const SDL_Keycode key)
{
    Key* keyStruct = GetKey(input, key);

    if (!keyStruct)
        return SDL_FALSE;

    if (keyStruct->state == SDL_PRESSED && keyStruct->repeat == 1)
        return SDL_TRUE;

    return SDL_FALSE;
}

SDL_bool Input_KeyWasReleased(const Input* input,
                              const SDL_Keycode key)
{
    Key* keyStruct = GetKey(input, key);

    if (!keyStruct)
        return SDL_FALSE;

    if (keyStruct->state == SDL_RELEASED && keyStruct->repeat == 0)
        return SDL_TRUE;

    return SDL_FALSE;
}
