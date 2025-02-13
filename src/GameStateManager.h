#pragma once

#include "Entity.h"
#include "Thing.h"
#include "Area.h"

#include "GraphicsRendererPayloads.h"
#include "CommandManager.h"
#include "eDataType.h"
#include "eManagerType.h"
#include "SystemManagerBase.h"
#include "WorldData.h"
#include "classes_Pathfinding.h"
#include "functions_Pathfinding.h"

#include <utility>
#include <optional>
#include <memory>
#include <iostream>
#include <functional>

/* Runs the game logic */

class GameStateManager : public SystemManagerBase
{
public:
	class GameStateManager();
	GameStateManager(GameStateManager&&) noexcept;

	/* Make and Set */
	Entity makeCharacter(CharacterData new_character_data, std::optional<Vector2D> arg_MapLimits = std::nullopt, std::optional<Vector2D> arg_SpawnPoint = std::nullopt);
	Entity makeEntity(EntityData new_entity_data, std::optional<Vector2D> arg_MapLimits = std::nullopt, std::optional<Vector2D> arg_SpawnPoint = std::nullopt);
	std::unique_ptr<Area> makeArea(AreaData new_area_data);
	ActionGenerator makeActionGenerator(const std::string& ActionName);
	void setPlayerCharacter(Entity NewPlayerCharacter, std::optional<Area*> arg_Area = std::nullopt, std::optional<Entity*> arg_Camera = std::nullopt);
	void setActiveArea(std::unique_ptr<Area> new_area);

	/* Manager <-> Manager */
	std::function<void(std::string name, eDataType data)> requestDataFromDataManager;
	virtual void receiveFromCentralDispatch(std::any inbound_data, const eDataType& inbound_data_type);
	virtual eManagerType viewMyManagerType();
	virtual std::any searchForData(const eDataType& reqDataType, const std::string& reqDataName);

	/* View and Access  */
	CommandManager* accessCommandManager();

	/* Process logic for the frame on the active map: 
	entities, characters*/
	//void runFrameLogic();
	void run_FrameLogic_V2(const std::tuple<EntityEvent, EntityEvent>& arg_CommandsMovementAction);

	void sendVisibleAnimations(double LagFrame=0);

	/* Test Functions */
	void testPrintMainCharacterID() { std::cout << PlayerCharacter.operator*().GlobalID; };

private:
	// Game State
	//std::unique_ptr<Area> ActiveArea{ std::make_unique<Area>() };
	std::unique_ptr<Area> ActiveArea{ nullptr };
	std::unique_ptr<Entity> PlayerCharacter{ nullptr };
	CharacterData PlayerCharacterData;
	bool m_Navmesh_recreated{ false };

	// Commands
	std::unique_ptr<CommandManager> Commander{std::make_unique<CommandManager>()};
};