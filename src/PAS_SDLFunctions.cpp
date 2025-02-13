#include "PAS_SDLFunctions.h"

#include <cstdint>
#include <sstream>
#include <string>

#include "SDL2/SDL.h"


SDL_Color makeSDLColorFromHex(std::string hex)
{
	// Lops off the # if needed
	if (hex.starts_with('#'))
		std::erase(hex, '#');

	std::stringstream sr{ hex.substr(0, 2) };
	std::stringstream sg{ hex.substr(2, 2) };
	std::stringstream sb{ hex.substr(4, 2) };
	Uint8 r;
	Uint8 g;
	Uint8 b;
	sr >> std::hex >> r;
	sg >> std::hex >> g;
	sb >> std::hex >> b;
	SDL_Color color{ r, g, b };
	return color;
}

ColorRGBA makeRGBAColorFromHex(std::string hex)
{
	{
		// Lops off the # if needed
		if (hex.starts_with('#'))
			std::erase(hex, '#');

		std::stringstream sr{ hex.substr(0, 2) };
		std::stringstream sg{ hex.substr(2, 2) };
		std::stringstream sb{ hex.substr(4, 2) };
		Uint8 r;
		Uint8 g;
		Uint8 b;
		sr >> std::hex >> r;
		sg >> std::hex >> g;
		sb >> std::hex >> b;
		ColorRGBA color{ r, g, b };
		return color;
	}
}
