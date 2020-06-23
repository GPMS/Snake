#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

// Linked List node that contains information about a Key event
typedef struct Key {
    SDL_Keycode keycode;    // SDL_Keycode of the key
    Uint8       state;      // Whether the key is being pressed or not
    Uint8       repeat;     // Whether the key is being held or not
    struct Key* next;       // Pointer to the next node
} Key;

// Linked List node that contains information about a Key event
typedef struct Button {
    Uint8 button;    // SDL_Keycode of the key
    Uint8 clicks;   // How many times was it clicked
    Uint8 state;     // Whether the key is being pressed or not
    struct Button* next; // Pointer to the next node
} Button;

// Contains info on the mouse position and state of the buttons
typedef struct Mouse {
    int x, y;
    Button* buttonEvents;
} Mouse;

// Contains basic input information
typedef struct Input {
    Key* keysEvent;
    Mouse mouse;

    // Text editting
    char text[4];
    int textLength;
    char* composition;
    Sint32 cursor;
    Sint32 selection_len;
} Input;

Input* Input_Create();
void   Input_Destroy(Input* input);

// Dealocates the Keys/Buttons Events list
void Input_EmptyList(Input* input);

// Adds Input event to a list so that it can be processed later
void Input_Process(Input* input,
                   SDL_Event* event);

int Input_Get(Input* input);

// Check if a particular mouse button was double clicked
SDL_bool Input_ButtonIsDoubleClicked(Input* input,
                                     const Uint8 buttonID);

// Check if a particular mouse button was clicked
SDL_bool Input_ButtonIsPressed(Input* input,
                               const Uint8 buttonID);

// Check if a particular mouse button was released
SDL_bool Input_ButtonIsReleased(Input* input,
                                const Uint8 buttonID);

// Check if a particular key has just been pressed
SDL_bool Input_KeyWasPressed(const Input* input,
                             const SDL_Keycode key);

// Check if a particular key is currently being held
SDL_bool Input_KeyIsBeingHeld(const Input* input,
                              const SDL_Keycode key);

// Check if a particular key has just been released
SDL_bool Input_KeyWasReleased(const Input* input,
                              const SDL_Keycode key);

#endif
