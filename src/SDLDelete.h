#pragma once
/* Libraries */
#include "SDL2/SDL.h"

/* unique_ptr Deleter for SDL Classes*/
struct SDLDelete
{
	void operator()(SDL_Surface* SDLPointer);
	void operator()(SDL_Texture* SDLPointer);
	void operator()(SDL_Renderer* SDLPointer);
	void operator()(SDL_Window* SDLPointer);
	void operator()(SDL_RWops* SDLPointer);
};