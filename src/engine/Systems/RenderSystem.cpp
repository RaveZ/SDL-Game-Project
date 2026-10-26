#include "RenderSystem.h"
#include <iostream>

void RenderSystem::update(EntityManager& entityManager)
{
	auto& renderComponents = entityManager.GetComponentMap<RenderComponent>();
	auto& transformComponents = entityManager.GetComponentMap<TransformComponent>();

    for (auto& [entity, renderComponent] : renderComponents)
    {
		auto transformIt = transformComponents.find(entity);

        if (transformIt == transformComponents.end())
            continue;


        auto& transform = transformIt->second;

        renderComponent.destRect.x = transform.position.x;
        renderComponent.destRect.y = transform.position.y;

        SDL_FlipMode flipMode = transformIt->second.direction == -1 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;


        SDL_RenderTextureRotated(renderer, renderComponent.texture, &renderComponent.srcRect, &renderComponent.destRect, 0.f, nullptr, flipMode);

    }
}
