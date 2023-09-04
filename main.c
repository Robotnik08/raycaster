// C and libs
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

// my headers
#include "includes/constants.h"
#include "includes/raycaster.h"
#include "includes/vector.h"
#include "includes/input.h"
#include "includes/color.h"
#include "includes/gametime.h"
#include "includes/textures.h"
#include "includes/player.h"
#include "includes/map.h"
#include "includes/draw.h"

// window
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL; 
Vector2Int windowSize = {0, 0};

// forward declarations
int QUIT (int code);

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) QUIT(1);

    window = SDL_CreateWindow("Slome 3D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,1000, 1000, SDL_WINDOW_SHOWN);

    if (window == NULL) QUIT(1);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) QUIT(1);


    SDL_Event event;
    int quit = 0;

    //frameloop
    while (!quit) {
        SDL_GetWindowSize(window, &windowSize.x, &windowSize.y);
        SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
            else {
                setInput(&event);
            }
        }

        // fullscreen
        if (checkInputDown(SDL_SCANCODE_F11)) {
            SDL_SetWindowFullscreen(window, SDL_GetWindowFlags(window) ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
        }

        movePlayer();
        drawFrame(renderer,windowSize);
        inputRefresh();
        updateTime();

        //set window title
        char* title = malloc(sizeof(char)*30);
        sprintf(title, "Slome 3d | %i FPS | X: %i | Y: %i", FPS, (int)position.x / CELL_SIZE, (int)position.y / CELL_SIZE);
        SDL_SetWindowTitle(window, title);
        free(title);
    }
    return QUIT(0);
}

int QUIT (int code) {
    if (renderer != NULL) SDL_DestroyRenderer(renderer);
    if (window != NULL) SDL_DestroyWindow(window);
    SDL_Quit();
    return code;
}