#include "TextureWrap.h"

#include <cstdio>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

TextureWrap::TextureWrap()
{
	Texture = nullptr;
}

SDL_Texture* TextureWrap::accessTexture()
{
	return Texture.get();
}

const SDL_Texture* TextureWrap::viewTexture()
{
	return Texture.get();
}

const Vector2D& TextureWrap::viewDimension()
{
	return size;
}

const std::string& TextureWrap::viewImageName() const
{
	return ImageName;
}

void TextureWrap::giveTexture(std::unique_ptr<SDL_Texture, SDLDelete> NewTexture)
{
	Texture.swap(NewTexture);
}

void TextureWrap::setImageName(std::string NewImageName)
{
	ImageName = NewImageName;
}

void TextureWrap::setDimensions(Vector2D new_size)
{
	size = new_size;
}

bool TextureWrap::isValidImage()
{
	if (Texture == nullptr)
		return false;
	else
		return true;
}
