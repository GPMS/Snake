#include "input.h"
#include <stdio.h>

Input* Input_Create()
{
    Input* input = (Input*) calloc(1, sizeof(Input));
    return input;
}

void Input_Destroy(Input* input)
{
    Input_EmptyList(input);
    free(input);
}

// Allocates memory for a new Key struct
Key* NewKey(const SDL_Keycode keycode,
                   const Uint8 state,
                   const Uint8 repeat)
{
    Key *key = malloc(sizeof(Key));

    key->keycode = keycode;
    key->state = state;
    key->repeat = repeat;
    key->next = NULL;

    return key;
}

// Allocates memory for a new Key event and adds it to a list
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
            curKey = curKey->next;
        }

        prevKey->next = NewKey(keycode, state, repeat);
    }
}

// Allocates memory for a new button struct
Button* NewButton(const Uint8 button,
                         const Uint8 clicks,
                         const Uint8 state)
{
    Button *newButton = malloc(sizeof(Button));

    newButton->button = button;
    newButton->clicks = clicks;
    newButton->state = state;
    newButton->next = NULL;

    return newButton;
}

// Allocates memory for a new Button event and adds it to a list
void AddButton(Input* input,
               const Uint8 button,
               const Uint8 clicks,
               const Uint8 state)
{
    if (input->mouse.buttonEvents == NULL)
    {
        input->mouse.buttonEvents = NewButton(button, clicks, state);
    }
    else
    {
        Button* curButton = input->mouse.buttonEvents;
        Button* prevButton = NULL;

        while(curButton != NULL)
        {
            if (curButton->button == button)
            {
                curButton->clicks = clicks;
                curButton->state = state;
                return;
            }
            prevButton = curButton;
            curButton = curButton->next;
        }

        prevButton->next = NewButton(button, clicks, state);
    }
}

// Dealocates the Keys Events list
void EmptyKeyEvents(Input* input)
{
    Key* key = input->keysEvent;
    Key* nextKey = NULL;

    while (key != NULL)
    {
        nextKey = key->next;
        free(key);
        key = nextKey;
    }

    input->keysEvent = NULL;
}

// Dealocates the Buttons Events list
void EmptyButtonEvents(Input* input)
{
    Button* button = input->mouse.buttonEvents;
    Button* nextButton = NULL;

    while (button != NULL)
    {
        nextButton = button->next;
        free(button);
        button = nextButton;
    }

    input->mouse.buttonEvents = NULL;
}

void Input_EmptyList(Input* input)
{
    EmptyKeyEvents(input);
    EmptyButtonEvents(input);
}

void Input_Process(Input* input,
                   SDL_Event* event)
{
    SDL_KeyboardEvent key = event->key;
    Mouse* mouse = &input->mouse;

    switch (event->type)
    {
        // Keyboard Events
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            AddKey(input, key.keysym.sym, key.state, key.repeat);
            break;

        case SDL_TEXTINPUT:
            if (input->textLength < 3)
                input->text[input->textLength++] = event->edit.text[0];
            break;
        case SDL_TEXTEDITING:
            input->composition = event->edit.text;
            input->cursor = event->edit.start;
            input->selection_len = event->edit.length;
            break;

        case SDL_MOUSEMOTION:
            // Update position
            mouse->x = event->motion.x;
            mouse->y = event->motion.y;
            break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            AddButton(input,
                      event->button.button,
                      event->button.clicks,
                      event->button.state);
            break;
        case SDL_MOUSEWHEEL:
            break;
    }
}

int Input_Get(Input* input)
{
    Input_EmptyList(input);

    SDL_Event event;
    int isRunning = 1;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT ||
            event.type == SDL_WINDOWEVENT_CLOSE)
        {
            isRunning = 0;
        }

        Input_Process(input, &event);
    }

    return isRunning;
}

// Util function used to find a particular button from the Button Event list
struct Button* GetButton(const Input* input,
                         const Uint8 button)
{
    Button* curButton = input->mouse.buttonEvents;

    while (curButton != NULL)
    {
        if (curButton->button == button)
        {
            return curButton;
        }
        curButton = curButton->next;
    }
    return NULL;
}

SDL_bool Input_ButtonIsDoubleClicked(Input* input,
                                     const Uint8 buttonID)
{
    Button* button = GetButton(input, buttonID);

    if (button == NULL)
        return SDL_FALSE;

    if (button->state != SDL_PRESSED || button->clicks % 2 != 0)
        return SDL_FALSE;

    return SDL_TRUE;
}

SDL_bool Input_ButtonIsPressed(Input* input,
                               const Uint8 buttonID)
{
    Button* button = GetButton(input, buttonID);

    if (button == NULL)
        return SDL_FALSE;

    if (button->state != SDL_PRESSED)
        return SDL_FALSE;

    return SDL_TRUE;
}

SDL_bool Input_ButtonIsReleased(Input* input,
                                const Uint8 buttonID)
{
    Button* button = GetButton(input, buttonID);

    if (button == NULL)
        return SDL_FALSE;

    if (button->state != SDL_RELEASED)
        return SDL_FALSE;

    return SDL_TRUE;
}

// Util function used to find a particular key from the Keys Event list
struct Key* GetKey(const Input* input,
                   const SDL_Keycode key)
{
    Key* curKey = input->keysEvent;

    while (curKey != NULL)
    {
        if (curKey->keycode == key)
        {
            return curKey;
        }
        curKey = curKey->next;
    }
    return NULL;
}

SDL_bool Input_KeyWasPressed(const Input* input,
                             const SDL_Keycode key)
{
    Key* keyStruct = GetKey(input, key);

    if (!keyStruct)
        return SDL_FALSE;

    if (keyStruct->state != SDL_PRESSED || keyStruct->repeat != 0)
        return SDL_FALSE;

    return SDL_TRUE;
}

SDL_bool Input_KeyIsBeingHeld(const Input* input,
                              const SDL_Keycode key)
{
    Key* keyStruct = GetKey(input, key);

    if (!keyStruct)
        return SDL_FALSE;

    if (keyStruct->state != SDL_PRESSED || keyStruct->repeat != 1)
        return SDL_FALSE;

    return SDL_TRUE;
}

SDL_bool Input_KeyWasReleased(const Input* input,
                              const SDL_Keycode key)
{
    Key* keyStruct = GetKey(input, key);

    if (!keyStruct)
        return SDL_FALSE;

    if (keyStruct->state != SDL_RELEASED || keyStruct->repeat != 0)
        return SDL_FALSE;

    return SDL_TRUE;
}
