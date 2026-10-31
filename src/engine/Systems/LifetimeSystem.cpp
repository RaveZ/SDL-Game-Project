#include "LifetimeSystem.h"

void LifetimeSystem::update(
    EntityManager& entityManager,
    float deltaTime
)
{
    auto& lifetimes = entityManager.GetComponentMap<LifetimeComponent>();


    std::vector<Entity> entitiesToDestroy;


    for (auto& [entity, lifetime] : lifetimes)
    {
        lifetime.TTL -= deltaTime;


        if (lifetime.TTL <= 0.f)
        {
            entitiesToDestroy.push_back(entity);
        }
    }


    for (Entity entity : entitiesToDestroy)
    {
        entityManager.destroyEntity(entity);
    }
}