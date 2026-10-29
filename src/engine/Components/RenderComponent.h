#pragma once

#include <SDL3/SDL.h>

const int TILE_SIZE = 32;

enum RenderLayer {
	BACKGROUND = 0,
	LEVEL = 1,
	CHARACTER = 2,
	FOREGROUND = 3
};
struct RenderComponent
{
	SDL_FRect srcRect { 0, 0, TILE_SIZE, TILE_SIZE };
	SDL_FRect destRect { 0.f, 0.f, TILE_SIZE, TILE_SIZE };
	SDL_Texture* texture;

	int layer = 0;
	RenderComponent()
	{

	}
	RenderComponent(SDL_Texture* tex, int inLayer) : texture(tex), layer(inLayer)
	{
	}
};