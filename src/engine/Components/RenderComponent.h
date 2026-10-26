#pragma once

#include <SDL3/SDL.h>

const int TILE_SIZE = 32;

struct RenderComponent
{
	SDL_FRect srcRect { 0, 0, TILE_SIZE, TILE_SIZE };
	SDL_FRect destRect { 0.f, 0.f, TILE_SIZE, TILE_SIZE };
	SDL_Texture* texture;

	RenderComponent()
	{

	}
	RenderComponent(SDL_Texture* tex) : texture(tex)
	{
	}
};