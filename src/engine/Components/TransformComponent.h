#pragma once

#include <glm/glm.hpp>

// Component that stores an entity's transform in 2D space.
struct TransformComponent
{
    glm::vec2 position;
    float direction;

    TransformComponent()
    {

    }
    TransformComponent(float x, float y)
    {
        position = glm::vec2(x, y);
		direction = 1.0f; 
    }
};