#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "includes/vector.h"
#include "includes/input.h"
#include "includes/color.h"
#include "includes/gametime.h"

#define PI 3.14159265359F
#define HALF_PI PI/2.0F
#define ONE_HALF_PI 3*PI/2.0F
#define CELL_SIZE 64
#define RENDER_DISTANCE 10
#define FOV 60
#define EPSILON 0.0001F
#define VERTICLE_SHADE 0.2F
#define VERTICLE_SHADE_COLOR 0x000000

struct {
    float distance;
    int wallType;
} typedef RaycastResult;

// function declarations
void drawFrame2D(SDL_Renderer *renderer);
void drawFrame3D(SDL_Renderer *renderer);
void movePlayer(float dt);
float toRadians (float degrees);
void fixFishEye (float* distance, float rayAngle, float camAngle);
RaycastResult castRayDistance (float a, Vector2 pos);
float correctAngle (float a) {
    if (a > 360) {
        a -= 360;
    }
    else if (a < 0) {
        a += 360;
    }
    return a;
}
int getWallType (int i) {
    return i >> 1;
}
int getWallSide (int i) {
    return i & 1;
}
// window
SDL_Window* window = NULL;
int windowWidth = 0;
int windowHeight = 0;
int playerSpeed = 200;
int mode3D = 0;
int modeFishEye = 1;

int map[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2,
    2, 0, 3, 0, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 0, 3, 0, 0, 3, 2,
    2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2,
    2, 0, 3, 3, 3, 0, 3, 3, 3, 0, 0, 0, 3, 3, 3, 0, 3, 3, 3, 2,
    2, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 2,
    2, 0, 3, 0, 3, 3, 3, 0, 3, 0, 0, 0, 3, 0, 3, 3, 3, 0, 3, 2,
    2, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 0, 3, 0, 3, 0, 3, 3, 3, 3, 0, 3, 3, 0, 3, 0, 3, 0, 3, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 2,
    2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 0, 3, 0, 3, 3, 3, 0, 3, 0, -1, 0, 3, 0, 3, 3, 3, 0, 3, 2,
    2, 0, 0, 0, 0, 0, 3, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 0, 3, 3, 3, 0, 3, 3, 3, 3, -1, 3, 0, 0, 0, 3, 0, 0, 3, 2,
    2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2,
    2, 0, 3, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2,
    2, 0, 0, 0, 3, 3, 3, 0, 0, 3, 0, 0, 0, 0, 3, 3, 3, 0, 0, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 2,
    2, 0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 2,
    2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
    2, 0, 3, 0, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3, 0, 3, 3, 0, 0, 2,
    2, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 2,
    2, 0, 3, 0, 3, 0, 3, 3, 3, 3, 0, -1, -1, -1, -1, 3, 0, 3, 0, 2,
    2, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, -1, -1, -1, -1, 0, 0, 0, 0, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};
int MAP_WIDTH = 20;
int MAP_HEIGHT = 28;
int colors[] = {
    0x000000,
    0xFFFFFF,
    0xFF0000,
    0x00FF00,
    0x0000FF
};
int ceilingColor = 0x595959;
int floorColor = 0x8B4513;
int fogColor = 0xFFFFFF;

Vector2 position = {120, 120};
float angle = 0;

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
        if (checkInputDown(SDL_SCANCODE_SPACE)) {
            mode3D = !mode3D;
        }
        if (checkInputDown(SDL_SCANCODE_F) && mode3D) {
            modeFishEye = !modeFishEye;
        }
        if (mode3D) drawFrame3D(renderer);
        else drawFrame2D(renderer);
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
void drawFrame3D(SDL_Renderer* renderer) {
    // clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
    SDL_RenderClear(renderer);

    //draw floor and ceiling
    SDL_SetRenderDrawColor(renderer, getRed(ceilingColor), getGreen(ceilingColor), getBlue(ceilingColor), 255);
    SDL_Rect ceilRect = {0, 0, windowWidth, windowHeight/2};
    SDL_RenderFillRect(renderer, &ceilRect);
    SDL_SetRenderDrawColor(renderer, getRed(floorColor), getGreen(floorColor), getBlue(floorColor), 255);
    ceilRect = (SDL_Rect){0, windowHeight/2, windowWidth, windowHeight/2};
    SDL_RenderFillRect(renderer, &ceilRect);

    //raycasting
    for (int i = 0; i < windowHeight; i++) {
        float a = toRadians(correctAngle(angle - FOV/2.0F + FOV * (float)i / (float)windowHeight));
        RaycastResult ray = castRayDistance(a, position);
        if (modeFishEye) fixFishEye(&ray.distance, a, toRadians(angle));
        float lineHeight = ((float)(RENDER_DISTANCE*CELL_SIZE) / ray.distance) * 100.0F;
        float shade = ray.distance / (float)(RENDER_DISTANCE*CELL_SIZE);
        int color = 0xFFFFFF;
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
        SDL_SetRenderDrawColor(renderer, getRed(renderColor), getGreen(renderColor), getBlue(renderColor), 255);
        SDL_Rect rect = {windowWidth/2 - windowHeight/2 + i, windowHeight/2 - lineHeight/2, 1, lineHeight};
        SDL_RenderDrawRect(renderer, &rect);
    }

    // draw black on both sides
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect rect = {0, 0, windowWidth/2 - windowHeight/2, windowHeight};
    SDL_RenderFillRect(renderer, &rect);
    rect = (SDL_Rect){windowWidth/2 + windowHeight/2, 0, windowWidth/2 - windowHeight/2, windowHeight};
    SDL_RenderFillRect(renderer, &rect);


}
void drawFrame2D(SDL_Renderer* renderer) {
    // clear the screen
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderClear(renderer);
    
    for (int i = 0; i < MAP_HEIGHT * MAP_WIDTH; i++) {
        int x = i % MAP_WIDTH;
        int y = i / MAP_WIDTH;
        int color = 0x000000;
        if (map[i] > 0) {
            color = colors[map[i]];
        } else {
            switch (map[i])
            {
                case -1:
                    color = rainbow(1);
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, getRed(color), getGreen(color), getBlue(color), 255);
        SDL_Rect rect = {x * CELL_SIZE + 2, y * CELL_SIZE + 2, CELL_SIZE - 4, CELL_SIZE - 4};
        SDL_RenderFillRect(renderer, &rect);
        //render number of cell (with squares from light to dark)
        SDL_SetRenderDrawColor(renderer, 255 - i*2.55F, 255 - i*2.55F, 255 - i*2.55F, 255);
        SDL_Rect rect2 = {x * CELL_SIZE + 16, y * CELL_SIZE + 16, 32, 32};
        SDL_RenderFillRect(renderer, &rect2);
    }
    SDL_SetRenderDrawColor(renderer, getRed(rainbow(1)), getGreen(rainbow(1)), getBlue(rainbow(1)), 255);
    SDL_Rect rect = {position.x - 16, position.y - 16, 32, 32};
    SDL_RenderFillRect(renderer, &rect);
    for (int i = 0; i < windowWidth; i++) {
        float a = toRadians(correctAngle(angle - FOV/2.0F + FOV * (float)i / (float)windowWidth));
        RaycastResult ray = castRayDistance(a, position);
        fixFishEye(&ray.distance, a, toRadians(angle));
        Vector2 endLoc = {position.x + cos(a) * ray.distance, position.y + sin(a) * ray.distance};
        SDL_RenderDrawLine(renderer, position.x, position.y, endLoc.x, endLoc.y);
    }

    // draw view ray
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, position.x, position.y, position.x + cos(toRadians(angle)) * 100, position.y + sin(toRadians(angle)) * 100);
}
RaycastResult castRayDistance (float a, Vector2 pos) {
    RaycastResult result;
    float rayAngle = a;
    Vector2 rayPos = pos;
    Vector2 offset = {0, 0};
    int depth = 0;

    //horizontal
    float angleTan = -1/tan(rayAngle);
    float horizontalDistance = RENDER_DISTANCE*CELL_SIZE;
    int horizontalWallHit = 0;
    if (rayAngle > PI) {
        rayPos.y = (((int)pos.y >> 6) << 6) - 0.0001F;
        rayPos.x = (pos.y - rayPos.y) * angleTan + pos.x;
        offset.y = -64;
        offset.x = -offset.y * angleTan;
    } else if (rayAngle < PI) {
        rayPos.y = (((int)pos.y >> 6) << 6) + 64;
        rayPos.x = (pos.y - rayPos.y) * angleTan + pos.x;
        offset.y = 64;
        offset.x = -offset.y * angleTan;
    }
    else if (fabs(rayAngle - PI) < EPSILON || fabs(rayAngle) < EPSILON) {
        rayPos.y = pos.y;
        rayPos.x = pos.x;
        offset.y = 0;
        offset.x = 0;
        depth = RENDER_DISTANCE;
    }
    while (depth < RENDER_DISTANCE) {
        int mapX = (int)rayPos.x >> 6;
        int mapY = (int)rayPos.y >> 6;
        int mapIndex = mapY * MAP_WIDTH + mapX;
        if (mapIndex < 0 || mapIndex >= MAP_WIDTH * MAP_HEIGHT || mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
            break;
        }
        if (map[mapIndex]) {
            horizontalDistance = sqrt(pow(pos.x - rayPos.x, 2) + pow(pos.y - rayPos.y, 2));
            horizontalDistance = fmin(horizontalDistance, RENDER_DISTANCE*CELL_SIZE);
            horizontalWallHit = map[mapIndex] << 1;
            break;
        }
        rayPos.x += offset.x;
        rayPos.y += offset.y;
        depth++;
    }
    //vertical
    depth = 0;
    float verticalDistance = RENDER_DISTANCE*CELL_SIZE;
    float negativeAngleTan = -tan(rayAngle);
    int verticalWallHit = 0;
    if (rayAngle > HALF_PI && rayAngle < ONE_HALF_PI) {
        rayPos.x = (((int)pos.x >> 6) << 6) - 0.0001F;
        rayPos.y = (pos.x - rayPos.x) * negativeAngleTan + pos.y;
        offset.x = -64;
        offset.y = -offset.x * negativeAngleTan;
    } else if (rayAngle < HALF_PI || rayAngle > ONE_HALF_PI) {
        rayPos.x = (((int)pos.x >> 6) << 6) + 64;
        rayPos.y = (pos.x - rayPos.x) * negativeAngleTan + pos.y;
        offset.x = 64;
        offset.y = -offset.x * negativeAngleTan;
    }
    else if (fabs(rayAngle - PI) < EPSILON || fabs(rayAngle) < EPSILON) {
        rayPos.y = pos.y;
        rayPos.x = pos.x;
        offset.y = 0;
        offset.x = 0;
        depth = RENDER_DISTANCE;
    }
    while (depth < RENDER_DISTANCE) {
        int mapX = (int)rayPos.x >> 6;
        int mapY = (int)rayPos.y >> 6;
        int mapIndex = mapY * MAP_WIDTH + mapX;
        if (mapIndex < 0 || mapIndex >= MAP_WIDTH * MAP_HEIGHT || mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) {
            break;
        }
        if (map[mapIndex]) {
            verticalDistance = sqrt(pow(pos.x - rayPos.x, 2) + pow(pos.y - rayPos.y, 2));
            verticalDistance = fmin(verticalDistance, RENDER_DISTANCE*CELL_SIZE);
            verticalWallHit = map[mapIndex] << 1 | 1;
            break;
        }
        rayPos.x += offset.x;
        rayPos.y += offset.y;
        depth++;
    }
    if (horizontalDistance < verticalDistance) {
        result.distance = horizontalDistance;
        result.wallType = horizontalWallHit;
    }
    else {
        result.distance = verticalDistance;
        result.wallType = verticalWallHit;
    }
    return result;
}

void movePlayer (float dt) {
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

float toRadians (float degrees) {
    return degrees * PI / 180.0F;
}
void fixFishEye (float* distance, float rayAngle, float camAngle) {
    float combinedAngle = rayAngle - camAngle;
    if (combinedAngle < 0) combinedAngle += 2*PI;
    else if (combinedAngle > 2*PI) combinedAngle -= 2*PI;
    *distance *= cos(combinedAngle);
}