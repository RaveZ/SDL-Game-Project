#pragma once

#include "glm/glm.hpp"

struct PhysicsComponent {
    glm::vec2 velocity = { 0.0f, 0.0f };
    glm::vec2 acceleration = { 0.0f, 0.0f };
    float maxSpeedX = 100.0f;
    bool useGravity = true;
};