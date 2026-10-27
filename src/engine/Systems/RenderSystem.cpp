#include "RenderSystem.h"
#include <iostream>

#include "RenderSystem.h"
#include <iostream>

void RenderSystem::update(EntityManager& entityManager, GameState& gs, Entity player)
{
	auto* playerTransform = entityManager.GetComponent<TransformComponent>(player);

    //calculate viewport position
    if (playerTransform)
    {
        gs.mapViewport.x = (playerTransform->position.x + TILE_SIZE / 2) - gs.mapViewport.w / 2;
    }

    auto& renderComponents = entityManager.GetComponentMap<RenderComponent>();
    auto& transformComponents = entityManager.GetComponentMap<TransformComponent>();

    for (auto& [entity, renderComp] : renderComponents)
    {
        auto transformIt = transformComponents.find(entity);
        if (transformIt == transformComponents.end()) continue;

        auto& transform = transformIt->second;

        if (renderComp.texture == nullptr) continue;

        renderComp.destRect.x = transform.position.x - gs.mapViewport.x;
        renderComp.destRect.y = transform.position.y;

        SDL_FlipMode flipMode = (transform.direction == -1) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

        SDL_RenderTextureRotated(
            renderer,
            renderComp.texture,
            &renderComp.srcRect,
            &renderComp.destRect,
            0.f,
            nullptr,
            flipMode
        );


    }
}