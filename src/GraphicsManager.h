#pragma once
/* Project*/
#include "GraphicsRendererPayloads.h"
#include "TextureWrap.h"
#include "SDLDelete.h"
#include "WorldData.h"
#include "eManagerType.h"
#include "eDataType.h"
#include "SystemManagerBase.h"
/* STL */
#include <vector>
#include <memory>
#include <string>
#include <filesystem>
#include <any>
/* Libraries*/
#include "SDL2/SDL.h"

/* A Cell with a pointer to it's Texture */
struct TextureCell 
{
	SDL_Texture* TexturePtr{ nullptr };
	int x{ 0 };
	int y{ 0 };
	int w{ 0 };
	int h{ 0 };

	void makeFromCell(Cell new_cell, SDL_Texture* texture_ptr);
};

struct DrawCell
{
	SDL_Texture* TexturePtr{ nullptr };
	/* Texture loc and size */
	int tx{ 0 };
	int ty{ 0 };
	int tw{ 0 };
	int th{ 0 };

	/* Area loc */
	int ax{ 0 };
	int ay{ 0 };
	int aw{ 0 };
	int ah{ 0 };

	int zDepth{ -1 };		// Place in the rendering order
	int TilesetID{ -1 };	// ID of the tile on the tileset. -1 = blank

	double ImageRotation{ 0 };	// Used to rotate a cell when drawing

	SDL_Rect src{};	// Source on texture
	SDL_Rect dst{};	// Destination on screen

	void copyTextureCell(const TextureCell& new_texture_cell);
	void copyCellToAreaLocs(const Cell& new_cell);
	void makeSDLRects();
	void applyScalingFactor();
};

class GraphicsManager : public SystemManagerBase
{
public:

	/* Manager -> Manager Communications*/
	bool loadTexture(std::filesystem::path TextureFilePath, std::string TextureName);

	/* Init SDL */
	bool InitializeSDL();
	/* Draw */
	void drawClearRenderer();
	void drawFrame();
	void sendRendererToScreen();

	/* System Manager Base Virtual Inheritance */
	virtual void receiveFromCentralDispatch(std::any reqData, const eDataType& reqDataType);
	virtual eManagerType viewMyManagerType();
	virtual std::any searchForData(const eDataType& reqDataType, const std::string& reqDataName);

private:

	/* The textures currently loaded in memory */
	std::vector<std::unique_ptr<TextureWrap>> TexturePack;

	/* Active Area */
	AreaData ActiveAreaData;
	Vector2D CameraLocation{};
	int CameraLocationIntX{};
	int CameraLocationIntY{};
	Vector2D ScaledScreenSize{};

	/* Graphics Data */
	std::vector<SpriteData> Sprites;
	std::vector<TilesetData> Tilesets;
	std::vector<DrawCell> CellPackDepth0to99;		// Background
	std::vector<DrawCell> CellPackDepth200to299;	// Foreground
	std::vector<DrawCell> CellPackDepth100to199;	// Ground characters
	std::vector<DrawCell> CellPackDepth300to399;	// Rooftop/flying characters
	std::vector<FrameVisibleAnimation> VisibleAnimations;

	/* SDL Structs */
	std::unique_ptr<SDL_Renderer, SDLDelete> Renderer{ nullptr };
	std::unique_ptr<SDL_Window, SDLDelete> Window{ nullptr };

	/* Private methods */
	void setNewActiveArea(AreaData new_area);
	void loadCharacterGraphics(CharacterData new_player_character);
	std::map<std::string, std::vector<TextureCell>> makeTextureCells();
	void makeRenderLayers(std::map<std::string, std::vector<TextureCell>> texture_cells);
	void make_RenderLayers_MultiTileset(std::map<std::string, std::vector<TextureCell>> texture_cells);
	void renderCellPack(const std::vector<DrawCell>& Cellpack);
	void renderCellPackCharacters(const std::vector<DrawCell>& CellPack);
	void renderMapCollisions();
	void renderMapPathfinding();
	void renderCharacterCollisions();
	void renderEntityWorldLocations();
	void showBox(Vector2D arg_Location, Vector2D arg_Size, Uint8 arg_Red, Uint8 arg_Green, Uint8 arg_Blue, Uint8 arg_Alpha, bool arg_FillBox=0);
	void showLine(Vector2D arg_Begin, Vector2D arg_End, Uint8 arg_Red, Uint8 arg_Green, Uint8 arg_Blue, Uint8 arg_Alpha);
	constexpr void sortFrameVisibleAnimations();
	void makeFrameAnimationLayers(); 
	void showAnimData(bool arg_Print
		, bool arg_WorldLocation
		, bool arg_CollisionLocation
		, bool arg_SpriteLocation
		, bool arg_FillWorldLocation
		, bool arg_SightCollisions
		, bool arg_SightNearestEnemy
		, bool arg_EntityPathfinding);
};

