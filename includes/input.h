#include <SDL2/SDL.h>

int keys[SDL_NUM_SCANCODES];
int keysJustDown[SDL_NUM_SCANCODES];

void initializeInput()
{
    for (int i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        keys[i] = 0; // Initialize all keys as not pressed
    }
}

int checkInput(SDL_Scancode in)
{
    return keys[(int)in];
}
int checkInputDown(SDL_Scancode in)
{
    return keysJustDown[(int)in];
}

void setInput(SDL_Event *event)
{
    if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP)
    {
        if (event->key.keysym.scancode >= SDL_NUM_SCANCODES) return;
        keys[event->key.keysym.scancode] = (event->type == SDL_KEYDOWN) ? 1 : 0;
        keysJustDown[event->key.keysym.scancode] = (event->type == SDL_KEYDOWN) ? 1 : 0;
    }
}
void inputRefresh()
{
    for (int i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        keysJustDown[i] = 0;
    }
}