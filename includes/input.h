#include <SDL2/SDL.h>

#define TOTAL_KEYS 256

int keys[TOTAL_KEYS];

void initializeInput()
{
    for (int i = 0; i < TOTAL_KEYS; i++)
    {
        keys[i] = 0; // Initialize all keys as not pressed
    }
}

int checkInput(char in)
{
    return keys[(int)in];
}

void setInput(SDL_Event *event)
{
    if (event->type == SDL_KEYDOWN)
    {
        keys[event->key.keysym.sym] = 1; // Key pressed
    }
    else if (event->type == SDL_KEYUP)
    {
        keys[event->key.keysym.sym] = 0; // Key released
    }
}