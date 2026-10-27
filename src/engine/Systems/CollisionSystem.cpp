#include "CollisionSystem.h"
#include <cmath>
#include <SDL3/SDL.h>

void CollisionSystem::update(EntityManager& entityManager, GameState& gs, float deltaTime)
{
    auto& collisionMap = entityManager.GetComponentMap<CollisionComponent>();
    auto& transformMap = entityManager.GetComponentMap<TransformComponent>();

    for (auto itA = collisionMap.begin(); itA != collisionMap.end(); ++itA)
    {
        Entity entityA = itA->first;

        auto transformAIt = transformMap.find(entityA);
        if (transformAIt == transformMap.end()) continue;
        auto& transformA = transformAIt->second;
        auto& colliderA = itA->second;

       

        for (auto itB = std::next(itA); itB != collisionMap.end(); ++itB)
        {
            Entity entityB = itB->first;

            auto transformBIt = transformMap.find(entityB);
            if (transformBIt == transformMap.end()) continue;
            auto& transformB = transformBIt->second;
            auto& colliderB = itB->second;

            SDL_FRect rectA{
               .x = transformA.position.x + colliderA.colliderRect.x,
               .y = transformA.position.y + colliderA.colliderRect.y,
               .w = colliderA.colliderRect.w,
               .h = colliderA.colliderRect.h
            };

            SDL_FRect rectB{
                .x = transformB.position.x + colliderB.colliderRect.x,
                .y = transformB.position.y + colliderB.colliderRect.y,
                .w = colliderB.colliderRect.w,
                .h = colliderB.colliderRect.h
            };

            SDL_FRect overlap{ 0 };
            if (SDL_GetRectIntersectionFloat(&rectA, &rectB, &overlap))
            {
				ResolveCollision(entityManager, entityA, entityB, rectA, rectB, overlap);
            }
        }

        if (gs.debugMode)
        {
            SDL_FRect rectDebug{
               .x = transformA.position.x + colliderA.colliderRect.x - gs.mapViewport.x,
               .y = transformA.position.y + colliderA.colliderRect.y,
               .w = colliderA.colliderRect.w,
               .h = colliderA.colliderRect.h
            };

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 150);
            SDL_RenderFillRect(renderer, &rectDebug);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        }
    }
}

void CollisionSystem::ResolveCollision(
    EntityManager& entityManager,
    Entity entityA, Entity entityB,
    const SDL_FRect& rectA, const SDL_FRect& rectB, const SDL_FRect& overlap)
{
    auto* transformA = entityManager.GetComponent<TransformComponent>(entityA);
    auto* transformB = entityManager.GetComponent<TransformComponent>(entityB);
    auto* physicsA = entityManager.GetComponent<PhysicsComponent>(entityA);
    auto* physicsB = entityManager.GetComponent<PhysicsComponent>(entityB);

    if (!physicsA && !physicsB) return; // Nothing to push

    // Decide axis based on the smaller overlap dimension
    if (overlap.w < overlap.h)
    {
        // Horizontal collision
        if (physicsA)
        {
            if (physicsA->velocity.x > 0 && rectA.x < rectB.x) // A is left of B
            {
                transformA->position.x -= overlap.w;
                physicsA->velocity.x = 0;
            }
            else if (physicsA->velocity.x < 0 && rectA.x > rectB.x) // A is right of B
            {
                transformA->position.x += overlap.w;
                physicsA->velocity.x = 0;
            }
        }
        if (physicsB)
        {
            if (physicsB->velocity.x > 0 && rectB.x < rectA.x)
            {
                transformB->position.x -= overlap.w;
                physicsB->velocity.x = 0;
            }
            else if (physicsB->velocity.x < 0 && rectB.x > rectA.x)
            {
                transformB->position.x += overlap.w;
                physicsB->velocity.x = 0;
            }
        }
    }
    else
    {
        // Vertical collision
        if (physicsA)
        {
            if (physicsA->velocity.y > 0 && rectA.y < rectB.y) // A is above B (falling)
            {
                transformA->position.y -= overlap.h;
                physicsA->velocity.y = 0;
            }
            else if (physicsA->velocity.y < 0 && rectA.y > rectB.y) // A is below B (jumping into ceiling)
            {
                transformA->position.y += overlap.h;
                physicsA->velocity.y = 0;
            }
        }
        if (physicsB)
        {
            if (physicsB->velocity.y > 0 && rectB.y < rectA.y)
            {
                transformB->position.y -= overlap.h;
                physicsB->velocity.y = 0;
            }
            else if (physicsB->velocity.y < 0 && rectB.y > rectA.y)
            {
                transformB->position.y += overlap.h;
                physicsB->velocity.y = 0;
            }
        }
    }
}