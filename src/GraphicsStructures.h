#pragma once
/* STL */
#include <cstdint>

/*
Only dependency must remain <cstdint>
*/

/* New color structure to  fix recursion issues with SDL_Color */
struct ColorRGBA
{
	ColorRGBA();
	ColorRGBA(uint8_t r, uint8_t g, uint8_t b);
	uint8_t r{ 0 };
	uint8_t g{ 0 };
	uint8_t b{ 0 };
	uint8_t a{ 255 };
};