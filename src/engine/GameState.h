#pragma once

#include "GameObject.h"
#include "SDL3/SDL.h"
#include "SDLState.h"
#include "SDL3_mixer/SDL_mixer.h"
#include <array>


const size_t LAYER_IDX_LEVEL = 0;
const size_t LAYER_IDX_CHARACTERS = 1;


struct GameState
{
    std::array < std::vector<GameObject>, 2> layers;
    std::vector<GameObject> backgroundTiles;
    std::vector<GameObject> foregroundTiles;
    std::vector<GameObject> bullets;
    int playerIndex;
    SDL_FRect mapViewport;
    float bg2Scroll, bg3Scroll, bg4Scroll;
    bool debugMode;

    GameState()
    {

    }

    GameState(const SDLState& state)
    {
        playerIndex = -1;
        mapViewport = SDL_FRect{
            .x = 0, .y = 0,
            .w = static_cast<float>(state.logW), .h = static_cast<float>(state.logH)
        };
        bg2Scroll = bg3Scroll = bg4Scroll = 0.f;
        debugMode = false;
    }

    GameObject& player() {
        return layers[LAYER_IDX_CHARACTERS][playerIndex];
    }
};
