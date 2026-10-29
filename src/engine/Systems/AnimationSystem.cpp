#include "AnimationSystem.h"

void AnimationSystem::update(EntityManager& entityManager, float deltaTime)
{
    auto& animMap = entityManager.GetComponentMap<AnimationComponent>();

    for (auto& [entity, anim] : animMap)
    {
        if (anim.frameCount <= 1 || anim.isFinished) continue;

        // Calculate how long ONE frame should last
        float frameDuration = anim.totalDuration / anim.frameCount;

        // Advance the timer
        anim.timer += deltaTime;

        // Check if it's time to advance to the next frame
        if (anim.timer >= frameDuration)
        {
            anim.timer -= frameDuration;
            anim.currentFrame++;

            // Handle the end of the animation
            if (anim.currentFrame >= anim.frameCount)
            {
                if (anim.isLooping)
                {
                    anim.currentFrame = 0;
                }
                else
                {
                    anim.currentFrame = anim.frameCount - 1;
                    anim.isFinished = true;
                }
            }
        }
    }
}