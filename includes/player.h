#ifndef PLAYER_H
#define PLAYER_H

#include "map.h"
#include "input.h"
#include "raycaster.h"
#include "gametime.h"

int playerSpeed = 200;
Vector2 position = {120, 120};
double angle = 0;

// forward declaration
void movePlayer();

void movePlayer () {
    if (checkInput(SDL_SCANCODE_W)) {
        position.x += cos(toRadians(angle)) * playerSpeed * deltaTime;
        position.y += sin(toRadians(angle)) * playerSpeed * deltaTime;
    }
    if (checkInput(SDL_SCANCODE_S)) {
        position.x -= cos(toRadians(angle)) * playerSpeed * deltaTime;
        position.y -= sin(toRadians(angle)) * playerSpeed * deltaTime;
    }
    if (checkInput(SDL_SCANCODE_A)) {
        position.x += cos(toRadians(angle) - PI/2) * playerSpeed * deltaTime;
        position.y += sin(toRadians(angle) - PI/2) * playerSpeed * deltaTime;
    }
    if (checkInput(SDL_SCANCODE_D)) {
        position.x += cos(toRadians(angle) + PI/2) * playerSpeed * deltaTime;
        position.y += sin(toRadians(angle) + PI/2) * playerSpeed * deltaTime;
    }
    if (checkInput(SDL_SCANCODE_RIGHT)) {
        angle += 100 * deltaTime;
        angle = correctAngle(angle);
    }
    if (checkInput(SDL_SCANCODE_LEFT)) {
        angle -= 100 * deltaTime;
        angle = correctAngle(angle);
    }
}


#endif