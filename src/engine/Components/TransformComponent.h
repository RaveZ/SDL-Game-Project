#pragma once

#include <glm/glm.hpp>

// Component that stores an entity's transform in 2D space.
struct TransformComponent
{
    glm::vec2 position;
    float rotation;
};