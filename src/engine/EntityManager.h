#pragma once

#include <unordered_map>
#include "Components/TransformComponent.h"
#include "Components/CollisionComponent.h"
#include "Components/RenderComponent.h"
#include "Components/InputComponent.h"
#include "Components/PhysicsComponent.h"

using Entity = unsigned int;

class EntityManager {
private:
    Entity nextEntity = 0;

    Entity player;

    // Component storage (Maps)
    std::unordered_map<Entity, InputComponent> inputComponents;
    std::unordered_map<Entity, TransformComponent> transformComponents;
    std::unordered_map<Entity, RenderComponent> renderComponents;
    std::unordered_map<Entity, CollisionComponent> collisionComponents;
    std::unordered_map<Entity, PhysicsComponent> physicsComponents;

public:
    EntityManager() = default;
    ~EntityManager() = default;

    Entity createEntity();
    void destroyEntity(Entity entity);

    template<typename T>
    void AddComponent(Entity entity, T component);

    template<typename T>
    void RemoveComponent(Entity entity);

    template<typename T>
    T* GetComponent(Entity entity);

    template<typename T>
    std::unordered_map<Entity, T>& GetComponentMap();


};

// ==========================================
// TEMPLATE IMPLEMENTATIONS
// ==========================================

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
T* EntityManager::GetComponent(Entity entity)
{
    auto& componentMap = GetComponentMap<T>();
    auto it = componentMap.find(entity);

    if (it == componentMap.end()) {
        return nullptr; // Entity doesn't have this component
    }

    return &it->second;
}