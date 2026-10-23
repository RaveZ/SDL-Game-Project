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
	//ID assigned to the next entity created
	Entity nextEntity;

	//Component storage 
	std::unordered_map<Entity, InputComponent> inputComponents;
	std::unordered_map<Entity, TransformComponent> transformComponents;
	std::unordered_map<Entity, RenderComponent> renderComponents;
	std::unordered_map<Entity, CollisionComponent> collisionComponents;


public:
	EntityManager();
	~EntityManager();

	// Creates a new entity and returns its ID.
	Entity createEntity();

	//destroy entity
	void destroyEntity(Entity entity);
	
	template<typename T>
	void AddComponent(Entity entity, T component);

	template<typename T>
	void RemoveComponent(Entity entity);

	template<typename T>
	T& GetComponent(Entity entity);

private:
	template<typename T>
	std::unordered_map<Entity, T>& GetComponentMap();

};