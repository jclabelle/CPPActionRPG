#pragma once
/* Project */
#include "SystemManagerBase.h"
#include "AssetFilePaths.h"
#include "eDataType.h"
#include "WorldData.h"
/* STL */
#include <filesystem>
/* Libraries*/
#include "nlohmann/json.hpp"

// Owns WorldData and responds to data requests from other managers 
class DataManager : public SystemManagerBase
{
public:

	/* System Manager Base Inheritance */
	virtual std::any searchForData(const eDataType& reqDataType, const std::string& reqDataName);
	virtual eManagerType viewMyManagerType();
	virtual void receiveFromCentralDispatch(std::any reqData, const eDataType& reqDataType);

	/* Data Loading */
	void loadWorldData(const std::filesystem::path& world_file_system_path = TestWorld);

	/* Load Save Game */
	void loadSaveGame(const std::filesystem::path& save_game_path = TestSaveGame);

	/* Tools*/
	void PrintToCmdLine();

private:
	WorldData WorldData;
};