#ifndef GAMETIME_H
#define GAMETIME_H

#include <time.h>

float deltaTime = 0.0F;
float lastFrame = 0.0F;
int framesThisSecond = 0;
int FPS = 0;
int lastFrameTimeSecond;

void updateTime()
{
    clock_t currentFrame = clock();
    deltaTime = (double)(currentFrame - lastFrame) / CLOCKS_PER_SEC;
    lastFrame = currentFrame;

    framesThisSecond++;
    if (lastFrameTimeSecond < (int)(currentFrame / CLOCKS_PER_SEC)) {
        lastFrameTimeSecond = (int)(currentFrame / CLOCKS_PER_SEC);
        FPS = framesThisSecond;
        framesThisSecond = 0;
    }
}

#endif