#include "EntityManager.h"

Entity EntityManager::createEntity()
{
    return nextEntity++;
}

void EntityManager::destroyEntity(Entity entity)
{
    // Just erase the entity from all maps
	RemoveComponent<TransformComponent>(entity);
	RemoveComponent<InputComponent>(entity);
	RemoveComponent<RenderComponent>(entity);
	RemoveComponent<CollisionComponent>(entity);
	RemoveComponent<PhysicsComponent>(entity);
}


// ==========================================
// EXPLICIT TEMPLATE SPECIALIZATIONS
// ==========================================

template<>
std::unordered_map<Entity, InputComponent>& EntityManager::GetComponentMap<InputComponent>()
{
    return inputComponents;
}

template<>
std::unordered_map<Entity, TransformComponent>& EntityManager::GetComponentMap<TransformComponent>()
{
    return transformComponents;
}

template<>
std::unordered_map<Entity, RenderComponent>& EntityManager::GetComponentMap<RenderComponent>()
{
    return renderComponents;
}

template<>
std::unordered_map<Entity, CollisionComponent>& EntityManager::GetComponentMap<CollisionComponent>()
{
    return collisionComponents;
}

template<>
std::unordered_map<Entity, PhysicsComponent>& EntityManager::GetComponentMap<PhysicsComponent>()
{
    return physicsComponents;
}


template<>
std::unordered_map<Entity, AnimationComponent>& EntityManager::GetComponentMap<AnimationComponent>()
{
    return animationComponents;
}