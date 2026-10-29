#include "CollisionSystem.h"
#include <cmath>
#include <SDL3/SDL.h>

void CollisionSystem::update(EntityManager& entityManager, GameState& gs, float deltaTime)
{
    auto& collisionMap = entityManager.GetComponentMap<CollisionComponent>();
    auto& transformMap = entityManager.GetComponentMap<TransformComponent>();

    for (auto& [entity, collider] : collisionMap)
    {
        collider.grounded = false;
    }

    for (auto itA = collisionMap.begin(); itA != collisionMap.end(); ++itA)
    {
        Entity entityA = itA->first;

        auto transformAIt = transformMap.find(entityA);
        if (transformAIt == transformMap.end()) continue;
        auto& transformA = transformAIt->second;
        auto& colliderA = itA->second;

        SDL_FRect rectA{
           .x = transformA.position.x + colliderA.colliderRect.x,
           .y = transformA.position.y + colliderA.colliderRect.y,
           .w = colliderA.colliderRect.w,
           .h = colliderA.colliderRect.h
        };

        SDL_FRect sensorA{
            .x = rectA.x + (rectA.w/4),
            .y = rectA.y + rectA.h,
            .w = rectA.w/2,
            .h = 1.0f
        };

        for (auto itB = std::next(itA); itB != collisionMap.end(); ++itB)
        {
            Entity entityB = itB->first;

            auto transformBIt = transformMap.find(entityB);
            if (transformBIt == transformMap.end()) continue;
            auto& transformB = transformBIt->second;
            auto& colliderB = itB->second;

            SDL_FRect rectB{
                .x = transformB.position.x + colliderB.colliderRect.x,
                .y = transformB.position.y + colliderB.colliderRect.y,
                .w = colliderB.colliderRect.w,
                .h = colliderB.colliderRect.h
            };

            SDL_FRect sensorB{
                .x = rectB.x + (rectB.w/4),
                .y = rectB.y + rectB.h,
                .w = rectB.w/2,
                .h = 1.0f
            };

            if (SDL_HasRectIntersectionFloat(&sensorA, &rectB))
            {
                colliderA.grounded = true;
            }
            if (SDL_HasRectIntersectionFloat(&sensorB, &rectA))
            {
                colliderB.grounded = true;
            }

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

            SDL_FRect sensorDebug{
               .x = sensorA.x - gs.mapViewport.x,
               .y = sensorA.y,
               .w = sensorA.w,
               .h = sensorA.h
            };

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 150);
            SDL_RenderFillRect(renderer, &rectDebug);

            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(renderer, &sensorDebug);

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

    if (!physicsA && !physicsB) return;

    if (overlap.w < overlap.h)
    {
        if (physicsA)
        {
            if (physicsA->velocity.x > 0 && rectA.x < rectB.x)
            {
                transformA->position.x -= overlap.w;
                physicsA->velocity.x = 0;
            }
            else if (physicsA->velocity.x < 0 && rectA.x > rectB.x)
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
        if (physicsA)
        {
            if (physicsA->velocity.y > 0 && rectA.y < rectB.y)
            {
                transformA->position.y -= overlap.h;
                physicsA->velocity.y = 0;
            }
            else if (physicsA->velocity.y < 0 && rectA.y > rectB.y)
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