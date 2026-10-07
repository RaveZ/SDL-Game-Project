#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

using namespace std;

struct SDLState
{
	SDL_Window* window;
    SDL_Renderer* renderer;
    
    int width, height, logW, logH;
};

void cleanup(SDLState& state);
bool Initialize(SDLState& state);

int main(int argc, char* argv[])
{
    SDLState state; 
    state.width = 1600;
	state.height = 900;
	state.logW = 640;
	state.logH = 320;
    
    if(!Initialize(state))
    {
        return 1;
	}

    //load the assets
	SDL_Texture* idleTexture = IMG_LoadTexture(state.renderer, "data/idle.png");
	SDL_SetTextureScaleMode(idleTexture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST);

    //setup game data
	const bool* keys = SDL_GetKeyboardState(nullptr);
    float playerX = 150.f;
    const float floor = state.logH;

    uint64_t prevTime = SDL_GetTicks();

    //start the game loop
    bool running = true;
    while (running)
    {
        uint64_t nowTime = SDL_GetTicks();
        float deltaTime = (nowTime - prevTime) / 1000.f; // convert to seconds 
        SDL_Event event{0};
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

        //handle movement
        float moveAmount = 0;
        if(keys[SDL_SCANCODE_A])
        {
            moveAmount += -75.f;
		}
        else if(keys[SDL_SCANCODE_D])
        {
            moveAmount += 75.f;
        }
		playerX += moveAmount * deltaTime;


        //perform drawing commands
		SDL_SetRenderDrawColor(state.renderer, 1, 1, 1, 1);
		SDL_RenderClear(state.renderer);

        const float spriteSize = 32.f;
        SDL_FRect src{
			.x = 0,
            .y = 0,
			.w = spriteSize,
            .h = spriteSize
        };

        SDL_FRect dst{
            .x = playerX,
            .y = floor - spriteSize,
            .w = spriteSize,
            .h = spriteSize
        };


		SDL_RenderTexture(state.renderer, idleTexture, &src, &dst);

        //swap buffer and present
		SDL_RenderPresent(state.renderer);

        prevTime = nowTime ;
    }


	SDL_DestroyTexture(idleTexture);
    cleanup(state);
    return 0;
}

bool Initialize(SDLState& state)
{
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
        cleanup(state);
        initSuccess = false;
    }

    //create the renderer
    state.renderer = SDL_CreateRenderer(state.window, nullptr);
    if (!state.renderer)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating renderer", nullptr);
        cleanup(state);
        initSuccess = false;
    }

    //configure presentation
    SDL_SetRenderLogicalPresentation(state.renderer, state.logW, state.logH, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	return initSuccess; 
}

void cleanup(SDLState &state)
{
	SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
}