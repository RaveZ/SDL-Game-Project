#include "application.h"
#include "engine/Components/CollisionComponent.h"
#include "engine/Components/RenderComponent.h"
#include "engine/Components/InputComponent.h"
#include "engine/Components/TransformComponent.h"


const int Application::SCREEN_WIDTH = 1200;
const int Application::SCREEN_HEIGHT = 700;
const int Application::HALF_WIDTH = SCREEN_WIDTH / 2;
const int Application::HALF_HEIGHT = SCREEN_HEIGHT / 2;
const int Application::LOGICAL_WIDTH = 640;
const int Application::LOGICAL_HEIGHT = 320;

const int MAP_ROWS = 5;
const int MAP_COLS = 50;

void drawParalaxBackground(SDL_Renderer* renderer, SDL_Texture* texture, float xVelocity, float& scrollPos, float scrollFactor, float deltaTime);

Application::Application()
{
}

bool Application::Initialize()
{
    state = SDLState();
    state.width = SCREEN_WIDTH;
    state.height = SCREEN_HEIGHT;
    state.logW = LOGICAL_WIDTH;
    state.logH = LOGICAL_HEIGHT;

   
    bool initSuccess = true;
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing SDL3", nullptr);
        initSuccess = false;
    }

    //create window
    state.window = SDL_CreateWindow("SDL3 Demo", state.width, state.height, SDL_WINDOW_RESIZABLE);

    if (!state.window)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating window", nullptr);
        cleanup();
        initSuccess = false;
    }

    //create the renderer
    state.renderer = SDL_CreateRenderer(state.window, nullptr);
    if (!state.renderer)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating renderer", state.window);
        cleanup();
        initSuccess = false;
    }
    SDL_SetRenderVSync(state.renderer, true);


    //configure presentation
    SDL_SetRenderLogicalPresentation(state.renderer, state.logW, state.logH, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    //create mixer device
    MIX_Init();
    state.mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!state.mixer)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating mixer device", state.window);
        cleanup();
        initSuccess = false;
    }

    //setup entity manager
    entityManager = std::make_unique<EntityManager>();

    //setup systems
	renderSystem = std::make_unique<RenderSystem>(state.renderer);

    return initSuccess;
}


int Application::run()
{
    if (!Initialize())
    {
        return 1;
    }

    //load the assets
    res.load(state);


    //setup game data
    gs = GameState(state);
    createTiles();
    uint64_t prevTime = SDL_GetTicks();


    //play main music
    MIX_Track* track = MIX_CreateTrack(state.mixer);
    MIX_SetTrackAudio(track, res.musicMain);
    MIX_SetTrackGain(track, 0.33f);
    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
    MIX_PlayTrack(track, props);

    //start the game loop
    bool running = true;
    while (running)
    {
        uint64_t nowTime = SDL_GetTicks();
        float deltaTime = (nowTime - prevTime) / 1000.f; // convert to seconds 
        SDL_Event event{ 0 };
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                {
                    running = false;
                    break;
                }
                case SDL_EVENT_WINDOW_RESIZED:
                {
                    state.width = event.window.data1;
                    state.height = event.window.data2;
                    break;
                }
            }
            
        }

        update(deltaTime);
       
        prevTime = nowTime;
    }

    res.unload();
    cleanup();
    return 0;

}

void Application::update(float deltaTime)
{ 
    //calculate viewport position
    gs.mapViewport.x = (gs.player().position.x + TILE_SIZE / 2) - gs.mapViewport.w / 2;

    //perform drawing commands
    SDL_SetRenderDrawColor(state.renderer, 50, 50, 50, 1);
    SDL_RenderClear(state.renderer);

    //draw background images
    SDL_RenderTexture(state.renderer, res.texBg1, nullptr, nullptr);
    drawParalaxBackground(state.renderer, res.texBg4, gs.player().velocity.x, gs.bg2Scroll, 0.075f, deltaTime);
    drawParalaxBackground(state.renderer, res.texBg3, gs.player().velocity.x, gs.bg2Scroll, 0.15f, deltaTime);
    drawParalaxBackground(state.renderer, res.texBg2, gs.player().velocity.x, gs.bg2Scroll, 0.3f, deltaTime);


    //update systems
    renderSystem->update(*entityManager);


    //swap buffer and present
    SDL_RenderPresent(state.renderer);
}




void Application::cleanup()
{
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
}


void Application::createTiles()
{
    /*
    *   1 - Ground
    *   2 - Panel
    *   3 - Enemy
    *   4 - Player
    *   5 - Grass
    *   6 - Brick
    */
    short map[MAP_ROWS][MAP_COLS] = {
        0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 3, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 4, 2, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

    };

    short foreground[MAP_ROWS][MAP_COLS] = {
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    };

    short background[MAP_ROWS][MAP_COLS] = {
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 6, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    };


    const auto loadMap = [this](short layer[MAP_ROWS][MAP_COLS])
        {
            const auto createObject = [this](int r, int c, SDL_Texture* tex, ObjectType type)
                {
                    GameObject o;
                    o.type = type;
                    o.position = glm::vec2(c * TILE_SIZE, state.logH - (MAP_ROWS - r) * TILE_SIZE);
                    o.texture = tex;
                    o.collider = {
                        .x = 0, .y = 0,
                        .w = TILE_SIZE, .h = TILE_SIZE
                    };
                    return o;
                };

            for (int r = 0; r < MAP_ROWS; r++)
            {
                for (int c = 0; c < MAP_COLS; c++)
                {
                    switch (layer[r][c])
                    {
                    case 1: //ground
                    {
                        GameObject o = createObject(r, c, res.texGround, ObjectType::level);
                        gs.layers[LAYER_IDX_LEVEL].push_back(o);
                        break;
                    }
                    case 2: //panel
                    {
                        GameObject o = createObject(r, c, res.texPanel, ObjectType::level);
                        gs.layers[LAYER_IDX_LEVEL].push_back(o);
                        break;
                    }
                    case 3: //enemy
                    {
                        GameObject o = createObject(r, c, res.texEnemy, ObjectType::enemy);
                        o.data.enemy = EnemyData();
                        o.currentAnimation = res.ANIM_ENEMY;
                        o.animations = res.enemyAnims;
                        o.collider = {
                            .x = 10, .y = 4,
                            .w = 12, .h = 28
                        };
                        o.maxSpeedX = 15;
                        o.dynamic = true;
                        gs.layers[LAYER_IDX_CHARACTERS].push_back(o);
                        break;
                    }
                    case 4: //player
                    {
                        //create our player
						Entity player = entityManager->createEntity();
						entityManager->AddComponent(player, InputComponent());
						entityManager->AddComponent(player, TransformComponent(c * TILE_SIZE, state.logH - (MAP_ROWS - r) * TILE_SIZE));
						entityManager->AddComponent(player, RenderComponent(res.texIdle));
						entityManager->AddComponent(player, CollisionComponent(SDL_FRect { .x = 11, .y = 6, .w = 10, .h = 26 }));

                        gs.playerIndex = gs.layers[LAYER_IDX_CHARACTERS].size() - 1;
                        break;
                    }
                    case 5: //grass
                    {
                        GameObject o = createObject(r, c, res.texGrass, ObjectType::level);
                        gs.foregroundTiles.push_back(o);
                        break;
                    }
                    case 6: //brick
                    {
                        GameObject o = createObject(r, c, res.texBrick, ObjectType::level);
                        gs.backgroundTiles.push_back(o);
                        break;
                    }
                    }
                }
            }
        };

    loadMap(map);
    loadMap(foreground);
    loadMap(background);

    assert(gs.playerIndex != -1);
}


void drawParalaxBackground(SDL_Renderer* renderer, SDL_Texture* texture, float xVelocity, float& scrollPos, float scrollFactor, float deltaTime)
{
    scrollPos -= xVelocity * scrollFactor * deltaTime;

    if (scrollPos <= -texture->w)
    {
        scrollPos = 0;
    }

    SDL_FRect dst{
        .x = scrollPos, .y = 30,
        .w = texture->w * 2.0f, .h = static_cast<float>(texture->h)
    };

    SDL_RenderTextureTiled(renderer, texture, nullptr, 1, &dst);
}