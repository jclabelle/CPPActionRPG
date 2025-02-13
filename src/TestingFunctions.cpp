#include "TestingFunctions.h"

#include <iostream>
#include <iomanip>
#include "SDL2/SDL.h"


// Print an approximation of the FPS to the console
void printFPS()
{
	static float frame_count=1;
	std::cout << std::setw(2) << frame_count / (SDL_GetTicks() / 1000.f) << '\n';
	std::cout << "Current tick: " << SDL_GetTicks() << '\n';
	++frame_count;
}

