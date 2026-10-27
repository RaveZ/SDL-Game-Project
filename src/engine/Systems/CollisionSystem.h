#pragma once
#include "../EntityManager.h"
#include "../GameState.h"

class CollisionSystem
{
public:
    CollisionSystem(SDL_Renderer* inRenderer) : renderer(inRenderer)
    {
    }
    void update(EntityManager& entityManager, GameState& gs, float deltaTime);

private:
    void ResolveCollision(
        EntityManager& entityManager,
        Entity entityA, Entity entityB,
        const SDL_FRect& rectA, const SDL_FRect& rectB, const SDL_FRect& overlap
    );

private:
    SDL_Renderer* renderer;
};