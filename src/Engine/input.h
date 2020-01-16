#pragma once

#include <SDL2/SDL.h>

/**
 * @struct Key
 * @brief Linked List node that contains information about a Key event
 */
typedef struct Key {
    SDL_Keycode keycode;    // SDL_Keycode of the key
    Uint8       state;      // Whether the key is being pressed or not
    Uint8       repeat;     // Whether the key is being held or not
    struct Key* nextKey;    // Pointer to the next node
}Key;

/**
 * @struct Input
 * @brief Contains basic input information
 */
typedef struct Input {
    Key* keysEvent; // List of key Input events to Process

    // Text
    char text[4];
    char* composition;
    Sint32 cursor;
    Sint32 selection_len;
    int textLength;
    int asw;
    SDL_bool isTooShort;
} Input;

#include "game.h"

/**
 * @brief Dealocates the Keys Events list
 *
 * @param input Needed to access the list
 */
void Input_EmptyList(Input* input);

/**
 * @brief Adds Input event to a list so that it can be processed later
 *
 * @param game Game struct to access Input information
 * @param event Event being handled right now
 *
 * @see AddKey
 */
void Input_Handle(Game* game,
                  SDL_Event* event);

/**
 * @brief Check if a particular key has just been pressed
 *
 * @param input Needed to access the Keys Event list
 * @param key   The SDL_Keycode of the key
 *
 * @return Returns SDL_TRUE if it was, SDL_FALSE otherwise
 *
 * @see Input_KeyIsBeingHeld
 * @see Input_KeyWasReleased
 */
SDL_bool Input_KeyWasPressed(const Input* input,
                             const SDL_Keycode key);

/**
 * @brief Check if a particular key is currently being held
 *
 * @param input Needed to access the Keys Event list
 * @param key   The SDL_Keycode of the key
 *
 * @return Returns SDL_TRUE if it is, SDL_FALSE otherwise
 *
 * @see Input_KeyWasPressed
 * @see Input_KeyWasReleased
 */
SDL_bool Input_KeyIsBeingHeld(const Input* input,
                              const SDL_Keycode key);

/**
 * @brief Check if a particular key has just been released
 *
 * @param input Needed to access the Keys Event list
 * @param key   The SDL_Keycode of the key
 *
 * @return Returns SDL_TRUE if it was, SDL_FALSE otherwise
 *
 * @see Input_KeyWasPressed
 * @see Input_KeyIsBeingHeld
 */
SDL_bool Input_KeyWasReleased(const Input* input,
                              const SDL_Keycode key);
