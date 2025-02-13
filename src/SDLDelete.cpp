/* Project */
#include "SDLDelete.h"
/* Libraries */
#include "SDL2/SDL.h"

/* Deleter passed to unique_ptr for proper free/destroy/close SDL operations */

void SDLDelete::operator()(SDL_Surface* SDLPointer)
{
	if (SDLPointer)
		SDL_FreeSurface(SDLPointer);
}
void SDLDelete::operator()(SDL_Texture* SDLPointer)
{
	if (SDLPointer)
		SDL_DestroyTexture(SDLPointer);
}
void SDLDelete::operator()(SDL_Renderer* SDLPointer)
{
	if (SDLPointer)
		SDL_DestroyRenderer(SDLPointer);
}
void SDLDelete::operator()(SDL_Window* SDLPointer)
{
	if (SDLPointer) SDL_DestroyWindow(SDLPointer);
}
void SDLDelete::operator()(SDL_RWops* SDLPointer)
{
	if (SDLPointer) SDL_RWclose(SDLPointer);
};