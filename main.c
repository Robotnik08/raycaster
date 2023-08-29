#include <stdio.h>
#include <SDL2/SDL.h>
#include "includes/vector.h"
#include "includes/input.h"

void drawFrame(SDL_Renderer *renderer);

int WinMain(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Slome C edition",                // Window title
        SDL_WINDOWPOS_UNDEFINED,    // Initial x position
        SDL_WINDOWPOS_UNDEFINED,    // Initial y position
        1000,                        // Width, in pixels
        1000,                        // Height, in pixels
        SDL_WINDOW_SHOWN            // Flags
    );

    if (window == NULL) {
        printf("Failed to create window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    int quit = 0;
    //frameloop
    while (!quit) {
        SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
            else {
                setInput(&event);
            }
        }
        if (checkInput(SDLK_ESCAPE)) {
            quit = 1;
        }

        drawFrame(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

void drawFrame (SDL_Renderer *renderer) {
    // clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // draw gradient squares
    float darknessX = 0;
    for (int x = 0; x < 1000; x++) {
        darknessX += 255.0F/1000.0F/2.0F;
        float darknessY = 0;
        for (int y = 0; y < 1000; y++) {
            darknessY += 255.0F/1000.0F/2.0F;
            SDL_SetRenderDrawColor(renderer, darknessX + darknessY, darknessX + darknessY, 255 - darknessX - darknessY, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}