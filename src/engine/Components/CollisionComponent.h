#pragma once

#include <SDL3/SDL.h>

//component that stores an entity's collision data.
struct CollisionComponent
{
	SDL_FRect colliderRect;
	bool grounded = false;

	CollisionComponent()
	{
		colliderRect = SDL_FRect{ 0 };
	}

	CollisionComponent(SDL_FRect collider) : colliderRect(collider)
	{

	}

};