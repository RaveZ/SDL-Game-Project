#include "EntityManager.h"

EntityManager::EntityManager()
{
    nextEntity = 0;
}

EntityManager::~EntityManager()
{
}

Entity EntityManager::createEntity()
{
    return nextEntity++;
}

void EntityManager::destroyEntity(Entity entity)
{
    RemoveComponent<TransformComponent>(entity);
    RemoveComponent<CollisionComponent>(entity);
    RemoveComponent<RenderComponent>(entity);
    RemoveComponent<InputComponent>(entity); // Don't forget to remove InputComponent too!
}

// A fallback map for unsupported types (keeps the compiler happy)
template<typename T>
std::unordered_map<Entity, T>& EntityManager::GetComponentMap()
{
    static std::unordered_map<Entity, T> emptyMap;
    return emptyMap;
}

// Explicit template specialization definitions
template<>
std::unordered_map<Entity, RenderComponent>& EntityManager::GetComponentMap<RenderComponent>()
{
    return renderComponents;
}

template<>
std::unordered_map<Entity, TransformComponent>& EntityManager::GetComponentMap<TransformComponent>()
{
    return transformComponents;
}

template<>
std::unordered_map<Entity, InputComponent>& EntityManager::GetComponentMap<InputComponent>()
{
    return inputComponents;
}

template<>
std::unordered_map<Entity, CollisionComponent>& EntityManager::GetComponentMap<CollisionComponent>()
{
    return collisionComponents;
}