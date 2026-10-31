#pragma once
#include "../EntityManager.h"
#include <SDL3/SDL.h>
#include "../../Resources.h"

class InputSystem {
private:
    bool wasJumpPressed = false;

    float shootCooldown = 0.0f;
    const float shootFireRate = 0.15f;

public:
    void update(EntityManager& entityManager, Entity player, Resources& res, const bool* keys, float deltaTime);
};