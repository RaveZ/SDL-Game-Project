#include "EntityManager.h"

EntityManager::EntityManager()
{
	nextEntity = 0;

}

EntityManager::~EntityManager()
{

}

template<typename T>
void EntityManager::AddComponent(Entity entity, T component)
{
    auto& componentMap = GetComponentMap<T>();
    componentMap[entity] = component;
}

template<typename T>
void EntityManager::RemoveComponent(Entity entity)
{
    auto& componentMap = GetComponentMap<T>();
    componentMap.erase(entity);
}

template<typename T>
T& EntityManager::GetComponent(Entity entity)
{
    return GetComponentMap<T>()[entity];
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
}


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

