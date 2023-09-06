#ifndef DRAW_H
#define DRAW_H

#include <SDL2/SDL.h>
#include "raycaster.h"
#include "color.h"
#include "textures.h"
#include "vector.h"
#include "player.h"

int ceilingColor = 0x595959;
int floorColor = 0xfffcd9;
int fogColor = 0x000000;

void drawFrame(SDL_Renderer *renderer, Vector2Int windowSize);

void drawFrame(SDL_Renderer* renderer, Vector2Int windowSize) {
    // clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
    SDL_RenderClear(renderer);
    int start = windowSize.x/2 - windowSize.y/2;
    int end = windowSize.x/2 + windowSize.y/2;

    //draw floor and ceiling
    for (int i = 0; i < windowSize.y/2; i++) {
        double shade = ((double)i / (double)(windowSize.y/2.0));
        int color = blendColor(ceilingColor, fogColor, shade);
        SDL_SetRenderDrawColor(renderer, getRed(color), getGreen(color), getBlue(color), 255);
        SDL_Rect rect = {start, i, windowSize.x, 1};
        SDL_RenderFillRect(renderer, &rect);

        color = blendColor(floorColor, fogColor, shade);
        SDL_SetRenderDrawColor(renderer, getRed(color), getGreen(color), getBlue(color), 255);
        rect = (SDL_Rect){start, windowSize.y - i, windowSize.x, 1};
        SDL_RenderFillRect(renderer, &rect);
    }


    //raycasting
    for (int i = 0; i < windowSize.y; i++) {
        double a = toRadians(correctAngle(angle - FOV/2.0 + FOV * (double)i / (double)windowSize.y));
        RaycastResult ray = castRayDistance(a, position);
        fixFishEye(&ray.distance, a, toRadians(angle), windowSize.y);
        double lineHeight = (double)(RENDER_DISTANCE*CELL_SIZE) / ray.distance;
        double shade = ray.distance * WALL_HEIGHT / (double)(RENDER_DISTANCE*CELL_SIZE);
        for (int j = 0; j < 8; j++) {
            int renderColor = blendColor(getColor(TEXTURE_BRICK[j*8 + getTextureX(ray.wallType)]) , VERTICLE_SHADE_COLOR, (getWallSide(ray.wallType) ? VERTICLE_SHADE : 0));
            renderColor = blendColor(renderColor, fogColor, fmin(1, shade + 0.05));
            SDL_SetRenderDrawColor(renderer, getRed(renderColor), getGreen(renderColor), getBlue(renderColor), 255);
            SDL_RenderDrawLine(renderer, windowSize.x/2 - windowSize.y/2 + i, windowSize.y/2 - lineHeight/2 + j*lineHeight/8, windowSize.x/2 - windowSize.y/2 + i, windowSize.y/2 - lineHeight/2 + (j+1)*lineHeight/8);
        }
    }

    // draw black on both sides
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect rect = {0, 0, start, windowSize.y};
    SDL_RenderFillRect(renderer, &rect);
    rect = (SDL_Rect){end, 0, windowSize.x, windowSize.y};
    SDL_RenderFillRect(renderer, &rect);
}

#endif