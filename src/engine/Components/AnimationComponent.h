#pragma once
#include <SDL3/SDL.h>

struct AnimationComponent
{
    SDL_Texture* texture = nullptr;
    int frameWidth = 32;
    int frameHeight = 32;

    int frameCount = 0;
    float totalDuration = 1.0f;
    float timer = 0.0f;
    int currentFrame = 0;

    bool isLooping = true;
    bool isFinished = false;
};