#include "application.h"
#include <stdexcept>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


int main(int argc, char* argv[])
{
	try
	{
		Application app;
		app.run();
	}
	catch (std::exception& e)
	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	}

	return 0;
}
