//#define DEBUG_DRAW_RECTANGLE
//#define FULLSCREEN
#define DRAW_MAP_COLLISIONS
#define DRAW_CHARACTER_COLLISIONS
#define DRAW_ENTITY_WORLDLOCATIONS
#define DRAW_MAP_PATHFINDING
#define PRINT_TO_CMD_ANIM_DATA
#define NDEBUG
//#define DEBUG_PRINT_INCOMING_VISIBLEANIMATIONS
//#define PAYLOAD_SORT
//#define VSYNC_ON
/* Project */
#include "GraphicsManager.h"
#include "GraphicsConstants.h"
#include "WorldData.h"
#include "TextManipulationFunctions.h"
#include "Collisions2D.h"
/* STL */
#include <type_traits>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <cassert>
/* Libraries */
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "fmt/core.h"


/* Loads textures into memory */
bool GraphicsManager::loadTexture(std::filesystem::path TextureFilePath, std::string TextureName)
{
	auto NameCompare = [&TextureName](const std::unique_ptr<TextureWrap>& tex) ->bool
	{;
		return TextureName == getNameFromFileName(tex->viewImageName());
	};

	if (std::find_if(TexturePack.begin(), TexturePack.end(), NameCompare) == TexturePack.end())
	{
		// Create Empty Assets
		Vector2D ImageDimension;
		SDL_Texture* SDLTextureLoading = nullptr;

		SDL_Surface* LoadingSurface = IMG_Load(TextureFilePath.string().c_str());
		if (!LoadingSurface)
		{
			printf("Unable to load image %s! SDL_image Error: %s\n", TextureFilePath.string().c_str(), IMG_GetError());
		}
		else
		{
			// Grab data
			ImageDimension.y = LoadingSurface->h;
			ImageDimension.x = LoadingSurface->w;
		}

		SDLTextureLoading = SDL_CreateTextureFromSurface(Renderer.get(), LoadingSurface);
		if (SDLTextureLoading == nullptr)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", TextureFilePath.string().c_str(), SDL_GetError());
		}

		// Free surface
		SDL_FreeSurface(LoadingSurface);

		// Put together the new TextureWrap and send it to the pack
		std::unique_ptr<SDL_Texture, SDLDelete> NewTexture = nullptr;
		std::unique_ptr<TextureWrap> NewTextureWrap{ new TextureWrap };
		NewTexture.reset(SDLTextureLoading);
		NewTextureWrap->giveTexture(std::move(NewTexture));
		NewTextureWrap->setImageName(TextureFilePath.filename().string());
		NewTextureWrap->setDimensions(ImageDimension);
		TexturePack.push_back(std::move(NewTextureWrap));
	}
	return true;
}

void GraphicsManager::drawClearRenderer()
{
	SDL_RenderClear(Renderer.get());
}

void GraphicsManager::drawFrame()
{	
	sortFrameVisibleAnimations();
	makeFrameAnimationLayers();
	CameraLocationIntX = static_cast<int>(CameraLocation.x)*ScalingFactor;
	CameraLocationIntY = static_cast<int>(CameraLocation.y)*ScalingFactor;

	/* Render background tiles */
	renderCellPack(CellPackDepth0to99);

	/* Render characters at ground level */

	renderCellPackCharacters(CellPackDepth100to199);

	/* Render foreground tiles */
	renderCellPack(CellPackDepth200to299);

	/* Render characters at sky level */
	renderCellPackCharacters(CellPackDepth300to399);

	showAnimData(0, 0, 0, 1, 1, 1, 1, 1);
//#ifdef DRAW_CHARACTER_COLLISIONS
//	renderCharacterCollisions();
//	renderEntityWorldLocations();
//#endif
//	/* Debug: Render Map Collisions */
#ifdef DRAW_MAP_COLLISIONS
renderMapCollisions();
#endif
#ifdef DRAW_MAP_PATHFINDING
renderMapPathfinding();
#endif
//#ifdef DRAW_ENTITY_WORLDLOCATIONS
//	renderEntityWorldLocations();
//#endif


	/* Reset characters packs*/
	CellPackDepth100to199.clear();
	CellPackDepth300to399.clear();
}

/* Final step of the Rendering process chain */
void GraphicsManager::sendRendererToScreen()
{
	SDL_RenderPresent(Renderer.get());
}

void GraphicsManager::receiveFromCentralDispatch(std::any reqData, const eDataType& reqDataType)
{
	if (reqDataType == eDataType::AREA)
	{
		setNewActiveArea(*reqData._Cast<AreaData>());
	}

	if (reqDataType == eDataType::CHARACTER)
	{
		loadCharacterGraphics(*reqData._Cast<CharacterData>());
	}

	if (reqDataType == eDataType::TEXTURE)
	{
		auto path = *reqData._Cast<std::filesystem::path>();
		std::string name = path.string();
		name = getNameFromFileName(name);
		loadTexture(path, name);
	}

	if (reqDataType == eDataType::SPRITE)
	{
		auto data  = *reqData._Cast<SpriteData>();

		if (std::find_if(Sprites.begin(), Sprites.end(),
			[&data](const SpriteData& sd) 
			{
				return data.Name==sd.Name;
			}) == Sprites.end())
		{
			Sprites.push_back(data);
			pullFromCentralDispatch(eManagerType::GRAPHICS, eDataType::TEXTURE, getNameFromFileName(data.Texture));
		}
	}

	if (reqDataType == eDataType::FRAMECAMERALOCATION)
	{
		CameraLocation = *reqData._Cast< Vector2D >();
	}

	if (reqDataType == eDataType::FRAMEANIMATIONS)
	{
		VisibleAnimations = *reqData._Cast<std::vector<FrameVisibleAnimation>>();
#ifdef DEBUG_PRINT_INCOMING_VISIBLEANIMATIONS
		for (auto a : VisibleAnimations)
		{
			std::cout << a.SpriteName << ",";
		}
		std::cout << '\n';
#endif
	}
}

constexpr void GraphicsManager::sortFrameVisibleAnimations()
{	/* TODO: Try/Catch */
	std::sort(std::begin(VisibleAnimations), std::end(VisibleAnimations),
		[](FrameVisibleAnimation a, FrameVisibleAnimation b)
		{
			/* Compare ZDepths */
			if (a.ZDepth > b.ZDepth)
				return true;
			else if (a.ZDepth < b.ZDepth)
				return false;

			// TODO: Might require sorting based on the collision's location
			/* If ZDepths are equal, compare lateral location*/
			if (a.WorldLocation.y > b.WorldLocation.y)
				return false;
			else if (a.WorldLocation.y < b.WorldLocation.y)
				return true;
				//return true;

			// TODO: Might require sorting based on the collision's location
			/* if lateral locations are equal, compare horizontal location*/
			if (a.WorldLocation.x > b.WorldLocation.x)
				return true;
			else if (a.WorldLocation.x < b.WorldLocation.x)
				return false;

			return false;
		});
}

void GraphicsManager::makeFrameAnimationLayers()
{
	//if (VisibleAnimations.size() == 2)
		//std::cout << "TWO";
	for (auto& AnAnimation : VisibleAnimations)
	{
		/* Find the Animations's Sprite Data */
		SpriteData ActiveSprite;
		for (auto ASpriteData : Sprites)
		{
			if (ASpriteData.Name == AnAnimation.SpriteName)
			{
				ActiveSprite = ASpriteData;
				break;
			}
		}

		/* Get a pointer to the Sprite's texture */
		auto TextureName = ActiveSprite.Texture;
		auto TextureWrapItr = std::find_if(std::begin(TexturePack), std::end(TexturePack),
			[TextureName](const std::unique_ptr<TextureWrap>& tex)
			{
				std::string ImgName = tex->viewImageName();
				return (ImgName == TextureName);
			});
		auto TexturePointer = TextureWrapItr->get()->accessTexture();

		/* Make a new TextureCell*/
		TextureCell NewTextureCell;
		Cell NewAnimCell = ActiveSprite.Cells.at(AnAnimation.AnimationName).at(AnAnimation.AnimationFrameID);
		NewTextureCell.makeFromCell(NewAnimCell, TexturePointer);

		/* Make a new DrawCell and copy the TextureCell */
		DrawCell NewDrawCell;
		NewDrawCell.copyTextureCell(NewTextureCell);

		/* Add world location and zDepth */	// Changed WorldLocation for SpriteLocation
		Cell NewWorldCell{ static_cast<int>(AnAnimation.SpriteLocation.x), static_cast<int>(AnAnimation.SpriteLocation.y), NewAnimCell.w, NewAnimCell.h };
		NewDrawCell.copyCellToAreaLocs(NewWorldCell);
		NewDrawCell.zDepth = AnAnimation.ZDepth;
		NewDrawCell.ImageRotation = AnAnimation.ImageRotation;

		/* Finish the cell */
		NewDrawCell.makeSDLRects();
		NewDrawCell.applyScalingFactor();

		/* Add to appropriate pack */
		if (NewDrawCell.zDepth >= 100 && NewDrawCell.zDepth <= 199)
			CellPackDepth100to199.push_back(NewDrawCell);
		else if (NewDrawCell.zDepth >= 300 && NewDrawCell.zDepth <= 399)
			CellPackDepth300to399.push_back(NewDrawCell);
	}
}

eManagerType GraphicsManager::viewMyManagerType()
{
	return eManagerType(eManagerType::GRAPHICS);
}

#pragma warning (disable: 4100)
std::any GraphicsManager::searchForData(const eDataType& reqDataType, const std::string& reqDataName)
{
	return std::any(0);
}

void GraphicsManager::setNewActiveArea(AreaData new_area_data)
{
	ActiveAreaData = new_area_data;
	
	/* Reset Data */
	/* TODO: Don't reload assets that are still needed 
	TODO: Changing area won't reload the Active Character's graphics data */
	Sprites.clear();
	Tilesets.clear();
	TexturePack.clear();

	/* Load data for the Area's tilesets */
	for (auto TilesetName : ActiveAreaData.FirstTileID_TilesetName)
	{
		std::any NewData = pullFromCentralDispatch(eManagerType::GRAPHICS, eDataType::TILESET, TilesetName.second);
		TilesetData NewTilesetData = *NewData._Cast< TilesetData >();
		Tilesets.push_back(NewTilesetData);
	}

	/* Load data and textures for the Area's characters */
	for (auto CharacterName : ActiveAreaData.Character_SpawnPoint)
	{
		// Load the Sprite
		std::any NewData = pullFromCentralDispatch(eManagerType::GRAPHICS, eDataType::CHARACTER, CharacterName.first);
		auto NewCharacterData = *NewData._Cast< CharacterData >()  ;
		std::string SpriteName = NewCharacterData.SpriteName;
		std::any NewSpriteData = pullFromCentralDispatch(eManagerType::GRAPHICS, eDataType::SPRITE, SpriteName);
		SpriteData NewSprite = *NewSpriteData._Cast< SpriteData >()  ;
		Sprites.push_back(NewSprite);

		// Load the Texture
		auto searchTextureName = NewSprite.Texture;
		searchTextureName = getNameFromFileName(searchTextureName);
		NewData = pullFromCentralDispatch(eManagerType::GRAPHICS, eDataType::TEXTURE, searchTextureName);
		auto NewTextureData = *NewData._Cast< std::filesystem::path>();
		std::string TextureName = NewTextureData.string();
		TextureName = getNameFromFileName(TextureName);
		loadTexture(NewTextureData, TextureName);
	}

	// Load Area Textures
	for (auto TilesetD : Tilesets)
	{
		std::any NewData = pullFromCentralDispatch(eManagerType::GRAPHICS, eDataType::TEXTURE, TilesetD.Texture);
		auto NewTextureData = *NewData._Cast< std::filesystem::path>();
		std::string TextureName = NewTextureData.string();
		TextureName = getNameFromFileName(TextureName);
		loadTexture(NewTextureData, TextureName);
	}

	// build the Tilemaps Cells 
	auto TilesetTextureCells = makeTextureCells();
	makeRenderLayers(TilesetTextureCells);
}

void GraphicsManager::loadCharacterGraphics(CharacterData new_player_character)
{
	// Load the Character's Sprite Data
	std::string SpriteName = new_player_character.SpriteName;
	std::any NewSpriteData = pullFromCentralDispatch(eManagerType::GRAPHICS, eDataType::SPRITE, SpriteName);
	SpriteData NewSprite = *NewSpriteData._Cast< SpriteData >();
	Sprites.push_back(NewSprite);

	// Load the Sprite's Texture
	auto searchTextureName = NewSprite.Texture;
	searchTextureName = getNameFromFileName(searchTextureName);
	std::any NewData = pullFromCentralDispatch(eManagerType::GRAPHICS, eDataType::TEXTURE, searchTextureName);
	auto NewTextureData = *NewData._Cast< std::filesystem::path>();
	std::string TextureName = NewTextureData.string();
	TextureName = getNameFromFileName(TextureName);
	loadTexture(NewTextureData, TextureName);
}

std::map<std::string, std::vector<TextureCell>> GraphicsManager::makeTextureCells()
{
	std::map<std::string,std::vector<TextureCell>> TilesetsTextureCells;

	for (auto Tileset : Tilesets)
	{
		std::vector<TextureCell> TextureCellV;
		/* Get a pointer to the Tileset's texture */
		auto TextureName = Tileset.Texture;
		auto TextureWrapItr = std::find_if(std::begin(TexturePack), std::end(TexturePack),
			[TextureName](const std::unique_ptr<TextureWrap>& tex)
			{ 
				std::string ImgName = tex->viewImageName();
				ImgName = getNameFromFileName(ImgName);
				return (ImgName == TextureName); 
			});
		auto TexturePointer = TextureWrapItr->get()->accessTexture();

		/* Build TextureCells for the Tileset */
		for (const auto& CellRef : Tileset.Tiles)
		{
			TextureCell NewTextureCell;
			NewTextureCell.makeFromCell(CellRef, TexturePointer);
			TextureCellV.push_back(NewTextureCell);
		}

		TilesetsTextureCells.emplace(TextureName, TextureCellV);
	}
	return TilesetsTextureCells;
}



void GraphicsManager::makeRenderLayers(std::map<std::string, std::vector<TextureCell>> texture_cells)
{
	auto AreaSize = ActiveAreaData.Size;
	//auto AreaCellCount = static_cast<int>(AreaSize.x * AreaSize.y);
	auto AreaColumns = AreaSize.x;

	struct TilesetLimits
	{
		std::string m_TilesetName;
		int m_FirstID;
		int m_LastID;
		Vector2Di m_TileSize;
		std::string m_TextureName;
		
	};
	//auto rr = ActiveAreaData.FirstTileID_TilesetName.value_comp();
	
	//auto lam_Compare = [](const std::pair<const int, std::string>& a, const std::pair<const int, std::string>& b)
	//{
	//	return a.first < b.first;
	//};


	std::vector<TilesetLimits> tmp_Limits;
	// Figure out the the upper and lower TileID bounds for each tileset used on this map
	for (auto iter = ActiveAreaData.FirstTileID_TilesetName.begin(); iter != ActiveAreaData.FirstTileID_TilesetName.end(); ++iter)
	{
		// Use this until we reach the last element
		if (iter != std::prev(ActiveAreaData.FirstTileID_TilesetName.end()))
		{
			tmp_Limits.push_back(TilesetLimits(iter->second, iter->first, (std::next(iter)->first) - 1));
		}
		else
		{
			tmp_Limits.push_back(TilesetLimits(iter->second, iter->first, std::numeric_limits<int>::max() ));
		}
	}

	// Find and set the tile size for each tileset used on this map
	for (auto& for_TilesetLimit : tmp_Limits)
	{
		const auto& tmp_itr_Tileset = std::find_if(std::begin(Tilesets), std::end(Tilesets), [&for_TilesetLimit](const TilesetData& TData) {return TData.Name == for_TilesetLimit.m_TilesetName; });
		for_TilesetLimit.m_TileSize = tmp_itr_Tileset->TileSize;
		for_TilesetLimit.m_TextureName;
	}


	// Build Cell Packs for Background and Foreground
	int ColumnCount{ 0 };
	int RowCount{ 0 };
	for (auto aLayout : ActiveAreaData.ZDepth_Layout)	// The Layout for a given ZDepth
	{
		ColumnCount = 0;
		RowCount = 0;
		for (auto TilesetID : aLayout.second)	// Iterate through each Tile of the Layout
		{
			// Skip blank tiles
			if (TilesetID == 0)
			{
				// Increment and get another TilesetID
				ColumnCount++;
				if (ColumnCount >= AreaColumns)
				{
					ColumnCount = 0;
					RowCount++;
				}
				continue;
			}
			
			// Give the DrawCell it's zDepth and TilesetID
			DrawCell NewDrawCell;
			NewDrawCell.zDepth = aLayout.first;
			//NewDrawCell.TilesetID = TilesetID;




			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//TODO: This will never work with >1 tileset

			
			///* Make a reverse iterator */
			//std::map<int, std::string>::reverse_iterator ritr;
			//int TilesetCount = ActiveAreaData.FirstTileID_TilesetName.size();

			// Temp containers for data from the tileset
			//size_t  tmp_LimitsIndex{ 0 };
			Vector2Di TileSize{0,0};
			//std::string TexName{};
			std::string TilesetName{};


			// Use the limits to find the texture
			for (const auto& for_Limits : tmp_Limits)
			{
				if (for_Limits.m_FirstID <= TilesetID && TilesetID <= for_Limits.m_LastID)
				{
					NewDrawCell.TilesetID = TilesetID - for_Limits.m_FirstID;
					//TileSize = for_Limits.m_TileSize;
					//TexName = for_Limits.m_TextureName;
					//TilesetName = for_Limits.m_TilesetName;
					NewDrawCell.copyTextureCell(texture_cells.at(for_Limits.m_TilesetName)[NewDrawCell.TilesetID]);

					//Cell NewCell;
					//NewCell.x = ColumnCount * for_Limits.m_TileSize.x;
					//NewCell.y = RowCount * for_Limits.m_TileSize.y;
					//NewCell.w = for_Limits.m_TileSize.x;
					//NewCell.h = for_Limits.m_TileSize.y;

					/* Copy the Cell, build the SDL Rectangles and copy to vector */
					NewDrawCell.copyCellToAreaLocs(Cell(ColumnCount* for_Limits.m_TileSize.x, RowCount* for_Limits.m_TileSize.y, for_Limits.m_TileSize.x, for_Limits.m_TileSize.y));

					//NewDrawCell.copyCellToAreaLocs(NewCell);
					NewDrawCell.makeSDLRects();
					NewDrawCell.applyScalingFactor();

					/* Is this a background tile with a properly set zDepth?...*/
					if (NewDrawCell.zDepth >= BackgroundTilesDepthMin && NewDrawCell.zDepth <= BackgroundTilesDepthMax && NewDrawCell.zDepth >= 0)
					{
						CellPackDepth0to99.push_back(NewDrawCell);
					}
					/* ... or a foreground tile with a properly set zDepth? */
					else if (NewDrawCell.zDepth >= ForegroundTilesDepthMin && NewDrawCell.zDepth <= ForegroundTilesDepthMax && NewDrawCell.zDepth >= 0)
					{
						CellPackDepth200to299.push_back(NewDrawCell);
					}
				}
			}

			
			/* Iterate through the map backwards*/
			//for (ritr = ActiveAreaData.FirstTileID_TilesetName.rbegin(); ritr != ActiveAreaData.FirstTileID_TilesetName.rend(); ritr++)
			//{

			//	// __PROBLEM__ The search will always grab the last tileset and use it to set the TileSize.
			//	/* Search the Tilesets to find the Tilesize.*/ 
			//	auto TsetItr = std::find_if(std::begin(Tilesets), std::end(Tilesets), [&ritr](const TilesetData& TData) {return TData.Name == ritr->second; });
			//	auto TileSize = TsetItr->TileSize;


				/* ...in case there is more than one tileset, find the original TileID */
				//if (NewDrawCell.TilesetID >= ritr->first && ritr->first !=0)
				//{
				//	int OriginalID = TilesetID - (AreaCellCount * (TilesetCount - 1));
				//	NewDrawCell.TilesetID = OriginalID;
				//}



				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




				// __PROBLEM__ : This will always give the last tileset texture // first (reverse) iterator
				/* Copy Texture Cell data if not blank. Leave TilsetID at -1 if blank. */
				//auto TexName = ritr->second;

				

				//TilesetCount--;

				/* Figure out world location */
				
			//}

			ColumnCount++;
			if (ColumnCount >= AreaColumns)
			{
				ColumnCount = 0;
				RowCount++;
			}
		}
	}
}

//TODO: Remove this if the new RenderLayers works as expected
void GraphicsManager::make_RenderLayers_MultiTileset(std::map<std::string, std::vector<TextureCell>> texture_cells)
{
	auto AreaSize = ActiveAreaData.Size;
	auto AreaCellCount = static_cast<int>(AreaSize.x * AreaSize.y);
	auto AreaColumns = AreaSize.x;

	// Build Cell Packs for Background and Foreground
	int ColumnCount{ 0 };
	int RowCount{ 0 };
	for (auto aLayout : ActiveAreaData.ZDepth_Layout)	// The Layout for a given ZDepth
	{
		ColumnCount = 0;
		RowCount = 0;
		for (auto TilesetID : aLayout.second)	// Iterate through each Tile of the Layout
		{
			// Give the DrawCell it's zDepth and TilesetID
			DrawCell NewDrawCell;
			NewDrawCell.zDepth = aLayout.first;
			NewDrawCell.TilesetID = TilesetID;

			// If this is on the first Tileset

			/* Make a reverse iterator */
			std::map<int, std::string>::reverse_iterator ritr;
			int TilesetCount = ActiveAreaData.FirstTileID_TilesetName.size();

			/* Iterate through the map backwards*/
			for (ritr = ActiveAreaData.FirstTileID_TilesetName.rbegin(); ritr != ActiveAreaData.FirstTileID_TilesetName.rend(); ritr++)
			{
				/* Search the Tilesets to find the Tilesize.*/
				auto TsetItr = std::find_if(std::begin(Tilesets), std::end(Tilesets), [&ritr](const TilesetData& TData) {return TData.Name == ritr->second; });
				auto TileSize = TsetItr->TileSize;
				/* ...in case there is more than one tileset, find the original TileID */
				if (NewDrawCell.TilesetID >= ritr->first && ritr->first != 0)
				{
					int OriginalID = TilesetID - (AreaCellCount * (TilesetCount - 1));
					NewDrawCell.TilesetID = OriginalID;
				}

				/* Copy Texture Cell data if not blank. Leave TilsetID at -1 if blank. */
				auto TexName = ritr->second;
				if (NewDrawCell.TilesetID > 0)
				{
					NewDrawCell.copyTextureCell(texture_cells.at(ritr->second)[NewDrawCell.TilesetID - 1]);
				}

				TilesetCount--;

				/* Figure out world location */
				Cell NewCell;
				NewCell.x = ColumnCount * TileSize.x;
				NewCell.y = RowCount * TileSize.y;
				NewCell.w = TileSize.x;
				NewCell.h = TileSize.y;

				/* Copy the Cell, build the SDL Rectangles and copy to vector */
				NewDrawCell.copyCellToAreaLocs(NewCell);
				NewDrawCell.makeSDLRects();
				NewDrawCell.applyScalingFactor();

				/* Is this a background tile with a properly set zDepth?...*/
				if (NewDrawCell.zDepth >= BackgroundTilesDepthMin && NewDrawCell.zDepth <= BackgroundTilesDepthMax && NewDrawCell.zDepth >= 0)
				{
					CellPackDepth0to99.push_back(NewDrawCell);
				}
				/* ... or a foreground tile with a properly set zDepth? */
				else if (NewDrawCell.zDepth >= ForegroundTilesDepthMin && NewDrawCell.zDepth <= ForegroundTilesDepthMax && NewDrawCell.zDepth >= 0)
				{
					CellPackDepth200to299.push_back(NewDrawCell);
				}
			}

			ColumnCount++;
			if (ColumnCount >= AreaColumns)
			{
				ColumnCount = 0;
				RowCount++;
			}
		}
	}
}



/* SDL Initialisation: Window, Renderer, IMG, Audio, Mixer(disabled), Fonts(disabled) */
bool GraphicsManager::InitializeSDL()
{
	// Initialization flag
	bool success = true;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// Create window
		Window.reset(std::move(SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			PhysicalScreenWidth, PhysicalScreenHeight, SDL_WINDOW_SHOWN)));
		if (Window == nullptr)
		{
			printf("Window could not be read! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			// Create renderer for window 
#ifdef VSYNC_ON
			Renderer.reset(std::move(SDL_CreateRenderer(Window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)));
			if (Renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
#endif
#ifndef VSYNC_ON
			Renderer.reset(std::move(SDL_CreateRenderer(Window.get(), -1, SDL_RENDERER_ACCELERATED)));
			if (Renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}

#endif
			else
			{
				// Initialize renderer color
				SDL_SetRenderDrawColor(Renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);

				// Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				/*
				// Initialize SDL_mixer
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}

				// Initialize SDL_tff

				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}*/
			}
		}
	}
#ifdef FULLSCREEN
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
	SDL_SetWindowFullscreen(Window.get(), SDL_WINDOW_FULLSCREEN);
#endif
	//SDL_RenderSetScale(Renderer.get(), 1, 1);
	return success;
}

// Used for Background and Foreground
void GraphicsManager::renderCellPack(const std::vector<DrawCell>& CellPack)
{
	Vector2D Scaled { ScaledScreenWidth, ScaledScreenHeight };
	for (auto aCell : CellPack)
	{
		if (aCell.TilesetID > -1 && aCell.TexturePtr!=nullptr)
		{
			/* Frame camera offset */
			aCell.dst.x -= CameraLocationIntX;
			aCell.dst.y -= CameraLocationIntY;

			/* Only render visible tiles */
			if (checkCollisionAxisAligned(CameraLocation, Scaled, Vector2D( aCell.ax, aCell.ay ), Vector2D( aCell.aw, aCell.ah )) == true)
				SDL_RenderCopyEx(Renderer.get(), aCell.TexturePtr, &aCell.src, &aCell.dst, 0, nullptr, SDL_FLIP_NONE);
		//SDL_Delay(25);
		//SDL_RenderPresent(Renderer.get());
		}
	}
}

// Used for Frame Visible Animations
void GraphicsManager::renderCellPackCharacters(const std::vector<DrawCell>& CellPack)
{
	for (auto aCell : CellPack)
	{
		if (aCell.TexturePtr != nullptr)
		{
			/* Frame Camera Offset */
			aCell.dst.x -= CameraLocationIntX;
			aCell.dst.y -= CameraLocationIntY;
			SDL_RenderCopyEx(Renderer.get(), aCell.TexturePtr, &aCell.src, &aCell.dst, aCell.ImageRotation, nullptr, SDL_FLIP_NONE);
			//SDL_Delay(25);
			//SDL_RenderPresent(Renderer.get());
		}
	}
}

// Utility for testing
void GraphicsManager::renderMapCollisions()
{
	for (const auto &CollisionBox : ActiveAreaData.CollisionMap)
	{
		SDL_SetRenderDrawColor(Renderer.get(), 255, 0, 0, 255);
		SDL_Rect Box{ static_cast<int>(CollisionBox.location.x), static_cast<int>(CollisionBox.location.y), 
			static_cast<int>(CollisionBox.size.x), static_cast<int>(CollisionBox.size.y)};
		if  constexpr (ScalingFactor != 1)
		{
			Box.x *= ScalingFactor;
			Box.y *= ScalingFactor;
			Box.w *= ScalingFactor;
			Box.h *= ScalingFactor;
		}

		Box.x -= CameraLocationIntX;
		Box.y -= CameraLocationIntY;

		SDL_RenderDrawRect(Renderer.get(), &Box);
	}
}

void GraphicsManager::renderMapPathfinding()
{
	for (const auto& for_Pathfinding_Row : ActiveAreaData.m_GeneratedPathfindingNodes)
	{
		for(const auto& for_Node : for_Pathfinding_Row)
		{
			
			SDL_SetRenderDrawColor(Renderer.get(), 255, 0, 0, 255);
			SDL_Rect Box{ static_cast<int>(for_Node.x), static_cast<int>(for_Node.y),
				static_cast<int>(5), static_cast<int>(5) };
			if  constexpr (ScalingFactor != 1)
			{
				Box.x *= ScalingFactor;
				Box.y *= ScalingFactor;
				Box.w *= ScalingFactor;
				Box.h *= ScalingFactor;
			}

			Box.x -= CameraLocationIntX;
			Box.y -= CameraLocationIntY;

			SDL_RenderDrawRect(Renderer.get(), &Box);
		}
	}
}

// Utility for testing
void GraphicsManager::renderCharacterCollisions()
{
	for (const auto& Character : VisibleAnimations)
	{
		SDL_SetRenderDrawColor(Renderer.get(), 0, 0, 255, 255);

		SDL_Rect Box{ static_cast<int>(Character.CollisionLocation.x)
			, static_cast<int>(Character.CollisionLocation.y)
			, static_cast<int>(Character.Size.x)
			, static_cast<int>(Character.Size.y) };

		if  constexpr (ScalingFactor != 1)
		{
			Box.x *= ScalingFactor;
			Box.y *= ScalingFactor;
			Box.w *= ScalingFactor;
			Box.h *= ScalingFactor;
		}

		Box.x -= CameraLocationIntX;
		Box.y -= CameraLocationIntY;

		SDL_RenderDrawRect(Renderer.get(), &Box);
	}
}

// Utility for testing
void GraphicsManager::renderEntityWorldLocations()
{
	for (const auto& Character : VisibleAnimations)
	{
		SDL_SetRenderDrawColor(Renderer.get(), 0, 255, 0, 255);

		SDL_Rect Box{ static_cast<int>(Character.WorldLocation.x)
			, static_cast<int>(Character.WorldLocation.y)
			, static_cast<int>(10)
			, static_cast<int>(10) };

		if  constexpr (ScalingFactor != 1)
		{
			Box.x *= ScalingFactor;
			Box.y *= ScalingFactor;
			Box.w *= ScalingFactor;
			Box.h *= ScalingFactor;
		}

		Box.x -= CameraLocationIntX;
		Box.y -= CameraLocationIntY;

		SDL_RenderDrawRect(Renderer.get(), &Box);
	}
}

void GraphicsManager::showBox(Vector2D arg_Location, Vector2D arg_Size, Uint8 arg_Red, Uint8 arg_Green, Uint8 arg_Blue, Uint8 arg_Alpha, bool arg_FillBox)
{

	SDL_SetRenderDrawColor(Renderer.get(), arg_Red, arg_Green, arg_Blue, arg_Alpha);

	SDL_Rect Box{ static_cast<int>(arg_Location.x)
		, static_cast<int>(arg_Location.y)
		, static_cast<int>(arg_Size.x)
		, static_cast<int>(arg_Size.y) };

	if  constexpr (ScalingFactor != 1)
	{
		Box.x *= ScalingFactor;
		Box.y *= ScalingFactor;
		Box.w *= ScalingFactor;
		Box.h *= ScalingFactor;
	}

	Box.x -= CameraLocationIntX;
	Box.y -= CameraLocationIntY;

	if (arg_FillBox == 0)
		SDL_RenderDrawRect(Renderer.get(), &Box);
	else
		SDL_RenderFillRect(Renderer.get(), &Box);
}

void GraphicsManager::showLine(Vector2D arg_Begin, Vector2D arg_End, Uint8 arg_Red, Uint8 arg_Green, Uint8 arg_Blue, Uint8 arg_Alpha)
{

	SDL_SetRenderDrawColor(Renderer.get(), arg_Red, arg_Green, arg_Blue, arg_Alpha);

	auto Begin_X = static_cast<int>(arg_Begin.x);
	auto Begin_Y = static_cast<int>(arg_Begin.y);
	auto End_X = static_cast<int>(arg_End.x);
	auto End_Y = static_cast<int>(arg_End.y);

	if  constexpr (ScalingFactor != 1)
	{
		Begin_X *= ScalingFactor;
		Begin_Y *= ScalingFactor;
		End_X *= ScalingFactor;
		End_Y *= ScalingFactor;
	}

	Begin_X -= CameraLocationIntX;
	Begin_Y -= CameraLocationIntY;
	End_X -= CameraLocationIntX;
	End_Y -= CameraLocationIntY;

	SDL_RenderDrawLine(Renderer.get(), Begin_X, Begin_Y, End_X, End_Y);
}


void printAnimData(std::string arg_Name, Vector2D arg_Location, Vector2D arg_Size)
{
	fmt::print("{}: Location (x{} y{}), Size (x{} y{}) \n", arg_Name, arg_Location.x, arg_Location.y, arg_Size.x, arg_Size.y);
}

void GraphicsManager::showAnimData(bool arg_Print
	, bool arg_WorldLocation
	, bool arg_CollisionLocation
	, bool arg_SpriteLocation
	, bool arg_FillWorldLocation
	, bool arg_SightCollisions
	, bool arg_SightNearestEnemy
	, bool arg_EntityPathfinding)
{
	for (const auto& for_Animation : VisibleAnimations)
	{
		if (arg_Print == true)
			std::cout << "GraphicsManager::showAnimData\n";

		if (arg_SpriteLocation == true)
		{
			SpriteData activesprite;
			for (auto for_spritedata : Sprites)
			{
				if (for_spritedata.Name == for_Animation.SpriteName)
				{
					activesprite = for_spritedata;
					break;
				}
			}
			showBox(for_Animation.SpriteLocation, activesprite.CellSize, 255, 0, 0, 255);

			if (arg_Print == true)
				printAnimData(std::string("Sprite"), for_Animation.SpriteLocation, activesprite.CellSize);
		}

			if (arg_CollisionLocation == true)
			{
				showBox(for_Animation.CollisionLocation, for_Animation.Size, 0, 255, 0, 255);

				if (arg_Print == true)
					printAnimData(std::string("Collision"), for_Animation.CollisionLocation, for_Animation.Size);

			}

			if (arg_WorldLocation == true)
			{
				double BoxSize{ 6 };
				Vector2D LocBox{ BoxSize, BoxSize };
				Vector2D BoxSpawn{ for_Animation.WorldLocation.x - BoxSize / 2, for_Animation.WorldLocation.y - BoxSize / 2, };
				
				showBox(BoxSpawn, LocBox, 0, 0, 255, 255, arg_FillWorldLocation);

				if (arg_Print == true)
					printAnimData(std::string("WorldLocation"), for_Animation.WorldLocation, Vector2D{ 1,1 });
			}

			if (arg_SightCollisions == true)
			{
				showBox(for_Animation.SightCollisions_WorldLocation, for_Animation.SightCollisions_Size, 125, 125, 125, 255);

				if (arg_Print == true)
					printAnimData(std::string("Sight"), for_Animation.SightCollisions_WorldLocation, for_Animation.SightCollisions_Size);
			}

			if (arg_SightNearestEnemy == true)
			{
				showLine(for_Animation.m_debug_LineToNearestEnemy.first
					, for_Animation.m_debug_LineToNearestEnemy.second
					, 255, 255, 255, 255);

				if (arg_Print == true)
					printAnimData(std::string("Nearest Enemy"), for_Animation.m_debug_LineToNearestEnemy.first, for_Animation.m_debug_LineToNearestEnemy.second);

			}

			if (arg_EntityPathfinding == true)
			{
				if (for_Animation.m_debug_CurrentPath.size())
				{
					for (const auto& for_AStarNodeVisual : for_Animation.m_debug_CurrentPath)
					{
						showBox(for_AStarNodeVisual, Vector2D{ 10,10 }, 75, 75, 75, 255, 1);
					}

				}
			}
		}
	}

void TextureCell::makeFromCell(Cell new_cell, SDL_Texture* texture_ptr)
{
	x = new_cell.x;
	y = new_cell.y;
	w = new_cell.w;
	h = new_cell.h;
	TexturePtr = texture_ptr;
}

void DrawCell::copyTextureCell(const TextureCell& new_texture_cell)
{
	TexturePtr = new_texture_cell.TexturePtr;
	tx = new_texture_cell.x;
	ty = new_texture_cell.y;
	tw = new_texture_cell.w;
	th = new_texture_cell.h;
}

void DrawCell::copyCellToAreaLocs(const Cell& new_cell)
{
	ax = new_cell.x;
	ay = new_cell.y;
	aw = new_cell.w;
	ah = new_cell.h;
}

void DrawCell::makeSDLRects()
{
	src.x = tx;
	src.y = ty;
	src.w = tw;
	src.h = th;

	dst.x = ax;
	dst.y = ay;
	dst.w = aw;
	dst.h = ah;

}

void DrawCell::applyScalingFactor()
{
	if constexpr (ScalingFactor != 1)
	{
		dst.x *= ScalingFactor;
		dst.y *= ScalingFactor;
		dst.w *= ScalingFactor;
		dst.h *= ScalingFactor;
	}
}
