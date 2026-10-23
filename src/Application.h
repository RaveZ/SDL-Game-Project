#pragma once

#include "engine/GameState.h"
#include "engine/SDLState.h"
#include "resources.h"
#include "engine/EntityManager.h"

#ifndef APPLICATION_H
#define APPLICATION_H



class Application
{
public:
    static const int SCREEN_WIDTH;
    static const int SCREEN_HEIGHT;
    static const int HALF_WIDTH;
    static const int HALF_HEIGHT;
	static const int LOGICAL_WIDTH;
	static const int LOGICAL_HEIGHT;

    Application();

    int run();
    bool Initialize();
    void cleanup();
    void createTiles();

private:
    GameState gs;
    SDLState state;
    Resources res;

	std::unique_ptr<EntityManager> entityManager;
};


#endif // APPLICATION_H
