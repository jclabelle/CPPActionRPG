#pragma once
#include "GraphicsStructures.h"
#include <string>

#include "SDL2/SDL.h"

SDL_Color makeSDLColorFromHex(std::string hex);

ColorRGBA makeRGBAColorFromHex(std::string hex);

