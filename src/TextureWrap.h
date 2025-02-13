#pragma once
/* Project */
#include "GeometryStructures.h"
#include "SDLDelete.h"
/* STL */
#include <cstdint>
#include <string>
#include <memory>
/* Libraries */
#include "SDL2/SDL.h"

// Wrapper around a SDL_Texture and various useful methods
class TextureWrap
{
public:

	// Constructor
	TextureWrap();

	// Accessors and viewers
	SDL_Texture* accessTexture();
	const SDL_Texture* viewTexture();
	const Vector2D& viewDimension();
	const std::string& viewImageName() const;
	void giveTexture(std::unique_ptr<SDL_Texture, SDLDelete> NewTexture);
	void setImageName(std::string ImageName);
	void setDimensions(Vector2D new_size);
	bool isValidImage();

	// Modes
	void setBlendMode(SDL_BlendMode blendmode);
	void setColorMode(Uint8 red, Uint8 green, Uint8 blue);
	void setAlphaMode(Uint8 alpha);
private:
	// The texture
	std::unique_ptr<SDL_Texture, SDLDelete> Texture{ nullptr };

	/* Name of the Source file on disk is used to identify
	which Texture should be rendered when the Graphics Manager's 
	drawRectangleOnRenderer is called by other parts of the engine */
	std::string ImageName{"NOT INITIALIZED"};

	// Dimensions
	Vector2D size{ 0,0 };
};