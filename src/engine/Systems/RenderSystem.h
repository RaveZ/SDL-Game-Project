#pragma once

#include <SDL3/SDL.h>
#include "../EntityManager.h"

class RenderSystem
{
public:
	RenderSystem(SDL_Renderer* inRenderer) : renderer(inRenderer)
	{

	}
	void update(EntityManager& entityManager);

private:
	SDL_Renderer* renderer;

};