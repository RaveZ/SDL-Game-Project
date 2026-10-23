#pragma once

#include <SDL3/SDL.h>


struct RenderComponent
{
	SDL_FRect srcRect;
	SDL_FRect destRect;
	SDL_Texture* texture;

	RenderComponent(SDL_Texture* tex, SDL_FRect src, SDL_FRect dest)
		: texture(tex), srcRect(src), destRect(dest) 
	{

	}
};