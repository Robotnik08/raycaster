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

// forward declarations
void drawFrame(SDL_Renderer *renderer);
void movePlayer(double dt);
double toRadians (double degrees);
// window
SDL_Window* window = NULL;
int windowWidth = 0;
int windowHeight = 0;


// colors
int colors[] = {
    0x000000,
    0xFFFFFF,
    0xFF0000,
    0x00FF00,
    0x0000FF
};
int ceilingColor = 0x595959;
int floorColor = 0xfffcd9;
int fogColor = 0x000000;

// player
int playerSpeed = 200;
Vector2 position = {120, 120};
double angle = 0;

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

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
        sprintf(title, "Slome 3d | %i FPS | X: %i | Y: %i", FPS, (int)position.x / CELL_SIZE, (int)position.y / CELL_SIZE);
        //set window title
        SDL_SetWindowTitle(window, title);
        free(title);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
void drawFrame(SDL_Renderer* renderer) {
    // clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
    SDL_RenderClear(renderer);
    int start = windowWidth/2 - windowHeight/2;
    int end = windowWidth/2 + windowHeight/2;

    //draw floor and ceiling
    for (int i = 0; i < windowHeight/2; i++) {
        double shade = ((double)i / (double)(windowHeight/2.0));
        int color = blendColor(ceilingColor, fogColor, shade);
        SDL_SetRenderDrawColor(renderer, getRed(color), getGreen(color), getBlue(color), 255);
        SDL_Rect rect = {start, i, windowWidth, 1};
        SDL_RenderFillRect(renderer, &rect);

        color = blendColor(floorColor, fogColor, shade);
        SDL_SetRenderDrawColor(renderer, getRed(color), getGreen(color), getBlue(color), 255);
        rect = (SDL_Rect){start, windowHeight - i, windowWidth, 1};
        SDL_RenderFillRect(renderer, &rect);
    }


    //raycasting
    for (int i = 0; i < windowHeight; i++) {
        double a = toRadians(correctAngle(angle - FOV/2.0 + FOV * (double)i / (double)windowHeight));
        RaycastResult ray = castRayDistance(a, position);
        fixFishEye(&ray.distance, a, toRadians(angle), windowHeight);
        double lineHeight = (double)(RENDER_DISTANCE*CELL_SIZE) / ray.distance;
        double shade = ray.distance * WALL_HEIGHT / (double)(RENDER_DISTANCE*CELL_SIZE);
        int color = 0x000000;
        if (getWallType(ray.wallType) > 0) {
            color = colors[getWallType(ray.wallType)];
        } else {
            switch (getWallType(ray.wallType))
            {
                case -1:
                    color = rainbow(4);
                    break;
            }
        }
        int renderColor = blendColor(color,VERTICLE_SHADE_COLOR, (getWallSide(ray.wallType) ? VERTICLE_SHADE : 0));
        renderColor = blendColor(renderColor, fogColor, fmin(1, shade + 0.05F));
        // simpletexturelagtest
        for (int j = 0; j < 8; j++) {
            if (j % 2 == 0) {
                renderColor = blendColor(0,VERTICLE_SHADE_COLOR, (getWallSide(ray.wallType) ? VERTICLE_SHADE : 0));
                renderColor = blendColor(renderColor, fogColor, fmin(1, shade + 0.05F));
            }
            else {
                renderColor = blendColor(0xFFFFFF, VERTICLE_SHADE_COLOR, (getWallSide(ray.wallType) ? VERTICLE_SHADE : 0));
                renderColor = blendColor(renderColor, fogColor, fmin(1, shade + 0.05F));
            }
            SDL_SetRenderDrawColor(renderer, getRed(renderColor), getGreen(renderColor), getBlue(renderColor), 255);
            SDL_Rect rect = {windowWidth/2 - windowHeight/2 + i, windowHeight/2 - lineHeight/2 + j*lineHeight/8, 1, lineHeight/8};
            SDL_RenderDrawRect(renderer, &rect);
        }
        // SDL_SetRenderDrawColor(renderer, getRed(renderColor), getGreen(renderColor), getBlue(renderColor), 255);
        // SDL_Rect rect = {windowWidth/2 - windowHeight/2 + i, windowHeight/2 - lineHeight/2, 1, lineHeight};
        // SDL_RenderDrawRect(renderer, &rect);
    }

    // draw black on both sides
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect rect = {0, 0, start, windowHeight};
    SDL_RenderFillRect(renderer, &rect);
    rect = (SDL_Rect){end, 0, windowWidth, windowHeight};
    SDL_RenderFillRect(renderer, &rect);
}
void movePlayer (double dt) {
    if (checkInput(SDL_SCANCODE_W)) {
        position.x += cos(toRadians(angle)) * playerSpeed * dt;
        position.y += sin(toRadians(angle)) * playerSpeed * dt;
    }
    if (checkInput(SDL_SCANCODE_S)) {
        position.x -= cos(toRadians(angle)) * playerSpeed * dt;
        position.y -= sin(toRadians(angle)) * playerSpeed * dt;
    }
    if (checkInput(SDL_SCANCODE_A)) {
        position.x += cos(toRadians(angle) - PI/2) * playerSpeed * dt;
        position.y += sin(toRadians(angle) - PI/2) * playerSpeed * dt;
    }
    if (checkInput(SDL_SCANCODE_D)) {
        position.x += cos(toRadians(angle) + PI/2) * playerSpeed * dt;
        position.y += sin(toRadians(angle) + PI/2) * playerSpeed * dt;
    }
    if (checkInput(SDL_SCANCODE_RIGHT)) {
        angle += 80 * dt;
        angle = correctAngle(angle);
    }
    if (checkInput(SDL_SCANCODE_LEFT)) {
        angle -= 80 * dt;
        angle = correctAngle(angle);
    }
}

double toRadians (double degrees) {
    return degrees * PI / 180.0;
}