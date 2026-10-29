#include "InputSystem.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/TransformComponent.h"
#include <iostream> 

void InputSystem::update(EntityManager& entityManager, Entity player, const bool* keys)
{
    auto* playerPhysics = entityManager.GetComponent<PhysicsComponent>(player);
    auto* playerTransform = entityManager.GetComponent<TransformComponent>(player);
    auto* playerCollision = entityManager.GetComponent<CollisionComponent>(player);

    if (!playerPhysics || !playerTransform) return;

    //Horizontal Movement
    float direction = 0.0f;
    if (keys[SDL_SCANCODE_A]) direction -= 1.0f;
    if (keys[SDL_SCANCODE_D]) direction += 1.0f;

    if (direction != 0.0f) {
        playerTransform->direction = (direction > 0) ? 1 : -1;
        playerPhysics->acceleration.x = direction * 300.0f;
    }
    else {
        playerPhysics->acceleration.x = 0.0f;
    }

    //jump
    bool isJumpPressed = keys[SDL_SCANCODE_SPACE];
    bool jumpJustPressed = isJumpPressed && !wasJumpPressed;
    bool isGrounded = playerCollision && playerCollision->grounded;

    
    if (jumpJustPressed && isGrounded)
    {
        playerPhysics->velocity.y = -200.0f;
        playerCollision->grounded = false; 
    }

    wasJumpPressed = isJumpPressed;

    //shooting
	bool bShootPressed = keys[SDL_SCANCODE_E];


}