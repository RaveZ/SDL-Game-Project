#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "engine/SDLState.h"

#include "engine/GameState.h"
#include "engine/Animation.h"

struct TileSetTextures
{
	int firstGid;
	std::vector<SDL_Texture*> textures;
};

struct Resources
{
	static Resources& get()
	{
		static Resources instance;
		return instance;
	}

	//animations
	const int ANIM_PLAYER_IDLE = 0;
	const int ANIM_PLAYER_RUN = 1;
	const int ANIM_PLAYER_SLIDE = 2;
	const int ANIM_PLAYER_SHOOT = 3;
	const int ANIM_PLAYER_SLIDE_SHOOT = 4;
	std::vector<Animation> playerAnims;

	const int ANIM_BULLET_MOVING = 0;
	const int ANIM_BULLET_HIT = 1;
	std::vector<Animation> bulletAnims;

	std::vector<Animation> enemyAnims;
	const int ANIM_ENEMY = 0;
	const int ANIM_ENEMY_HIT = 1;
	const int ANIM_ENEMY_DIE = 2;

	//textures
	std::vector<SDL_Texture*> textures;
	SDL_Texture* texIdle, * texRun, * texSlide,
		* texBrick, * texGrass, * texGround, * texPanel,
		* texBg1, * texBg2, * texBg3, * texBg4, * texBullet, * texBulletHit,
		* texShoot, * texRunShoot, * texSlideShoot,
		* texEnemy, * texEnemyHit, * texEnemyDie;

	//Sound
	std::vector<MIX_Audio*> audioList;
	MIX_Audio* audioShoot, * audioShootHit, * audioEnemyHit,
		* musicMain;

	SDL_Texture* loadTexture(SDL_Renderer* renderer, const std::string& filePath);

	MIX_Audio* loadAudio(MIX_Mixer* mixer, const std::string& filePath);

	void load(SDLState& state);

	void unload();
};

