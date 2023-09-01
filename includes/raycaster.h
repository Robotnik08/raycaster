#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <math.h>
#include "vector.h"
#include "constants.h"
#include "map.h"

// structs
struct {
    float distance;
    int wallType;
} typedef RaycastResult;

// forward declarations
RaycastResult castRayDistance (float a, Vector2 pos);
void fixFishEye (float* distance, float rayAngle, float camAngle, int windowHeight);
float correctAngle (float a);
int getWallType (int i);
int getWallSide (int i);




// functions

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
void fixFishEye (float* distance, float rayAngle, float camAngle, int windowHeight) {
    float combinedAngle = rayAngle - camAngle;
    if (combinedAngle < 0) combinedAngle += 2*PI;
    else if (combinedAngle > 2*PI) combinedAngle -= 2*PI;
    *distance *= cos(combinedAngle)/(WALL_HEIGHT*(windowHeight/1000.0F));
}


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

#endif