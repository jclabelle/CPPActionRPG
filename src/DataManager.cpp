/* Project files */
#include "DataManager.h"
#include "AssetFilePaths.h"
#include "PAS_SDLFunctions.h"
#include "PAS_JsonUtilities.h"
#include "TextManipulationFunctions.h"
#include "WorldData.h"
#include "eManagerType.h"
#include "eDataType.h"

/* STL */
#include <string>
#include <vector>
#include <fstream>
#include <set>
/* Libraries */
#include "nlohmann/json.hpp"


/* SystemManagerBase Inheritance */
eManagerType DataManager::viewMyManagerType()
{
	return eManagerType::DATA;
}

void DataManager::receiveFromCentralDispatch(std::any reqData, const eDataType& reqDataType)
{
	reqData = reqDataType;
}

std::any DataManager::searchForData(const eDataType& reqDataType, const std::string& reqDataName)
{
	std::any returnData{};

	if (reqDataType == eDataType::ACTION)
	{
		returnData = WorldData.ActionName_Action.at(reqDataName);
		return returnData;
	}

	if (reqDataType == eDataType::AREA)
	{
		returnData = WorldData.AreaName_Area.at(reqDataName);
		return returnData;
	}

	if (reqDataType == eDataType::CHARACTER)
	{
		returnData = WorldData.CharacterName_Character.at(reqDataName);
		return returnData;
	}

	if (reqDataType == eDataType::ENTITY)
	{
		returnData = WorldData.EntityName_Entity.at(reqDataName);
		return returnData;
	}

	if (reqDataType == eDataType::GAMEPLAYEFFECT)
	{
		returnData = WorldData.GameplayEffectName_GameplayEffect.at(reqDataName);
		return returnData;
	}

	if (reqDataType == eDataType::NAVIGATIONPATH)
	{
		returnData = WorldData.NavigationPathName_NavigationPath.at(reqDataName);
		return returnData;
	}

	if (reqDataType == eDataType::NAVIGATIONSYSTEM)
	{
		returnData = WorldData.NavigationSystemName_NavigationSystem.at(reqDataName);
		return returnData;
	}

	if (reqDataType == eDataType::SPRITE)
	{
		returnData = WorldData.SpriteName_Sprite.at(reqDataName);
		return returnData;
	}

	if (reqDataType == eDataType::TILESET)
	{
		returnData = WorldData.TilesetName_Tileset.at(reqDataName);
		return returnData;
	}

	if (reqDataType == eDataType::TEXTURE)
	{

		returnData = WorldData.Texture_Filepath.at(reqDataName);
		return returnData;
	}

	if (reqDataType == eDataType::VISUALEFFECT)
	{

		returnData = WorldData.VisualEffectName_VisualEffect.at(reqDataName);
		return returnData;
	}

	return returnData;
}

/*
Retrieves the following information from the Save Game:
- The Area of the savepoint
- The Player Character 
*/
void DataManager::loadSaveGame(const std::filesystem::path& save_game_path)
{
	/* Load the save game into a json object */
	nlohmann::json jSave = loadJson(save_game_path);

	/* Send the PlayerCharacter to the game state */
	auto PlayerCharacterName = jSave.at("playercharacter").get<std::string>();
	auto any_CharacterData = searchForData(eDataType::CHARACTER, PlayerCharacterName);
	pushToCentralDispatch(eManagerType::GAMESTATE, eDataType::PLAYERCHARACTER, any_CharacterData);
	pushToCentralDispatch(eManagerType::GRAPHICS, eDataType::CHARACTER, any_CharacterData);
	
	/* Send the AreaData of the SavePoint to the game state */
	auto LoadingAreaName = jSave.at("area").get<std::string>();
	std::any any_AreaData = searchForData(eDataType::AREA, LoadingAreaName);
	pushToCentralDispatch(eManagerType::GAMESTATE, eDataType::AREA, any_AreaData);
	pushToCentralDispatch(eManagerType::GRAPHICS, eDataType::AREA, any_AreaData);

	// Send all Sprites to the Graphics Manager	
	for(const auto& forName : WorldData.SpriteName_Sprite)
	pushToCentralDispatch(eManagerType::GRAPHICS, eDataType::SPRITE, forName.second);



}



void DataManager::loadWorldData(const std::filesystem::path& world_file_system_path)
{
	/* Loads all custom datatype : areas+maps, characters, entities, tilesets, sprites,
	* visual effects, navigation systems & paths, gameplay effects and actions.
	 Does not load image assets. Images are loaded by the Graphics Manager's SDL2 functions */
	nlohmann::json jWorld = loadJson(world_file_system_path);
	WorldData.from_json(jWorld, WorldData);
}

void DataManager::PrintToCmdLine()
{
	WorldData.PrintToCmdLine();
}

