#include "InputSystem.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/TransformComponent.h"

void InputSystem::update(EntityManager& entityManager, Entity player, const bool* keys)
{
    auto* playerPhysics = entityManager.GetComponent<PhysicsComponent>(player);
    auto* playerTransform = entityManager.GetComponent<TransformComponent>(player);
    auto* playerCollision = entityManager.GetComponent<CollisionComponent>(player);

    // Safety check: Player must have physics and transform
    if (!playerPhysics || !playerTransform) return;

    // --- 1. HORIZONTAL MOVEMENT ---
    float direction = 0.0f;
    if (keys[SDL_SCANCODE_A]) direction -= 1.0f;
    if (keys[SDL_SCANCODE_D]) direction += 1.0f;

    if (direction != 0.0f)
    {
        playerTransform->direction = (direction > 0) ? 1 : -1;
        playerPhysics->acceleration.x = direction * 300.0f; // Acceleration force
    }
    else
    {
        // No input, let the MovementSystem's friction handle deceleration
        playerPhysics->acceleration.x = 0.0f;
    }

    // --- 2. JUMPING (Edge Trigger) ---
    bool isJumpPressed = keys[SDL_SCANCODE_SPACE];

    // Only jump if: The key is pressed, it wasn't pressed last frame, and player is on the ground
    bool jumpJustPressed = isJumpPressed && !wasJumpPressed;
    bool isGrounded = playerCollision && playerCollision->grounded;

    if (jumpJustPressed && isGrounded)
    {
        playerPhysics->velocity.y = -200.0f; // Apply jump force
        playerCollision->grounded = false;   // Immediately leave the ground
    }

    // Save the state for the next frame
    wasJumpPressed = isJumpPressed;
}