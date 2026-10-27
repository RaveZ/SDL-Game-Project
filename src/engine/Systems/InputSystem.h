#pragma once
#include "../EntityManager.h"
#include <SDL3/SDL.h>

class InputSystem {
private:
    bool wasJumpPressed = false; 

public:
    void update(EntityManager& entityManager, Entity player, const bool* keys);
};