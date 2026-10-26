#pragma once

#include <unordered_map>
#include "Components/TransformComponent.h"
#include "Components/CollisionComponent.h"
#include "Components/RenderComponent.h"
#include "Components/InputComponent.h"

// Type used to uniquely identify entities in the ECS.
using Entity = unsigned int;

class EntityManager {

private:
	Entity nextEntity;

	std::unordered_map<Entity, InputComponent> inputComponents;
	std::unordered_map<Entity, TransformComponent> transformComponents;
	std::unordered_map<Entity, RenderComponent> renderComponents;
	std::unordered_map<Entity, CollisionComponent> collisionComponents;

public:
	EntityManager();
	~EntityManager();

	Entity createEntity();
	void destroyEntity(Entity entity);

	// We declare them here, but the implementation is right below the class
	template<typename T>
	void AddComponent(Entity entity, T component);

	template<typename T>
	void RemoveComponent(Entity entity);

	template<typename T>
	T& GetComponent(Entity entity);

	template<typename T>
	std::unordered_map<Entity, T>& GetComponentMap();
};



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