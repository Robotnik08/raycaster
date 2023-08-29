#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "includes/vector.h"
#include "includes/input.h"
#include "includes/color.h"
#include "includes/gametime.h"

// function declarations
void drawFrame(SDL_Renderer *renderer);
void movePlayer(float dt);

// window
SDL_Window* window = NULL;
int windowWidth = 0;
int windowHeight = 0;

int playerSpeed = 100;

int map[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1,
    1, 0 ,0 ,2 ,0 ,2 ,0 ,0 ,0 ,1,
    1, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1,
    1, 0 ,2 ,0 ,0 ,0 ,2 ,0 ,0 ,1,
    1, 0 ,0 ,2 ,2 ,2 ,0 ,0 ,0 ,1,
    1, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1,
    1, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1,
    1, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};
int colors[] = {
    0x000000,
    0xFFFFFF,
    0xFF0000
};

Vector2 position = {0, 0};
Vector2 direction = {1, 0};

int WinMain(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "Slome 3D",                 // Window title
        SDL_WINDOWPOS_UNDEFINED,    // Initial x position
        SDL_WINDOWPOS_UNDEFINED,    // Initial y position
        1000,                       // Width, in pixels
        1000,                       // Height, in pixels
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
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        SDL_RenderPresent(renderer);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
            else {
                setInput(&event);
            }
        }
        if (checkInputDown(SDL_SCANCODE_F11)) {
            SDL_SetWindowFullscreen(window, SDL_GetWindowFlags(window) ^ SDL_WINDOW_FULLSCREEN_DESKTOP);
        }
        movePlayer(deltaTime);

        drawFrame(renderer);
        inputRefresh();
        updateTime();
        char* title = malloc(sizeof(char)*30);
        sprintf(title, "Slome 3d | %i FPS", FPS);
        //set window title
        SDL_SetWindowTitle(window, title);
        free(title);
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
    // draw the map
    for (int i = 0; i < sizeof(map)/sizeof(int); i++) {
        if (map[i]) {
            SDL_SetRenderDrawColor(renderer,getRed(colors[map[i]]), getGreen(colors[map[i]]), getBlue(colors[map[i]]), 255);
            SDL_Rect rect = {i % 10 * 100, i / 10 * 100, 100, 100};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    //draw the player
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_Rect rect = {position.x, position.y, 20, 20};
    SDL_RenderFillRect(renderer, &rect);
    // draw 1 ray from the player
    SDL_RenderDrawLine(renderer, position.x + 10, position.y + 10, position.x + 10 + direction.x * 100, position.y + 10 + direction.y * 100);
}
void movePlayer (float dt) {
    normalize(&direction);
    if (checkInput(SDL_SCANCODE_W)) {
        position.x += direction.x * dt * playerSpeed;
        position.y += direction.y * dt * playerSpeed;
    }
    if (checkInput(SDL_SCANCODE_S)) {
        position.x -= direction.x * dt * playerSpeed;
        position.y -= direction.y * dt * playerSpeed;
    }
    if (checkInput(SDL_SCANCODE_A)) {
        position.x += direction.y * dt * playerSpeed;
        position.y -= direction.x * dt * playerSpeed;
    }
    if (checkInput(SDL_SCANCODE_D)) {
        position.x -= direction.y * dt * playerSpeed;
        position.y += direction.x * dt * playerSpeed;
    }
    if (checkInput(SDL_SCANCODE_RIGHT)) {
        direction.x = cos(dt) * direction.x - sin(dt) * direction.y;
        direction.y = sin(dt) * direction.x + cos(dt) * direction.y;
    }
    if (checkInput(SDL_SCANCODE_LEFT)) {
        direction.x = cos(-dt) * direction.x - sin(-dt) * direction.y;
        direction.y = sin(-dt) * direction.x + cos(-dt) * direction.y;
    }
}