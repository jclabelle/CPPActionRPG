//#define DEBUG_GRAPHICS_PAYLOADS
#pragma warning (disable: 4100)
#define NDEBUG
/* Project */
#include "GameStateManager.h"
#include "TestingFunctions.h"
#include "AssetFilePaths.h"
#include "WorldData.h"
#include "classes_Pathfinding.h"
#include "functions_Pathfinding.h"
/* STL */
#include <string>
#include <utility>
#include <memory>
#include <cassert>
#include <iostream>

GameStateManager::GameStateManager() { std::cout << "Game State Default Constructors"; }
GameStateManager::GameStateManager(GameStateManager&&) noexcept { std::cout << "Game State Move Constructors"; }

 CommandManager* GameStateManager::accessCommandManager()
 {
	 return Commander.get();
 }

 void GameStateManager::run_FrameLogic_V2(const std::tuple<EntityEvent, EntityEvent>& arg_CommandsMovementAction)
 {
	 auto& tmp_Tile_Ref = find_Current_Navmesh_Tile(PlayerCharacter.get()->v_Collision(), ActiveArea->Navmesh_().m_Tiles);
	 PlayerCharacter.get()->m_Gameplay.m_Player_Current_Navmesh_Tile = &tmp_Tile_Ref;

	 for ( auto& f_nav : ActiveArea->Navmesh_().m_Tiles)
	 {
		 for ( auto& f_navv : f_nav.m_Edges)
		 {
			 auto reff = traverse_Navmesh_Edge(f_nav, *f_navv);
		 }
	 }


	 if (m_Navmesh_recreated == false)
	 {
		 m_Navmesh_recreated = true;

		 for (auto& for_Entity : ActiveArea.get()->Entity_Population())
		 {
			 Navmesh_Tile& tmp_Current_Tile = find_Current_Navmesh_Tile(for_Entity.v_Collision(), ActiveArea->Navmesh_().m_Tiles);
			 for_Entity.AI().set_Current_Navmesh_Tile(tmp_Current_Tile);
		 }
	 }

	 auto ret_FullPop = ActiveArea->run_FrameLogic_V2(arg_CommandsMovementAction);
	 ActiveArea->run_FrameCleanup(ret_FullPop);
 }

 void GameStateManager::sendVisibleAnimations(double LagFrame)
 {
	 auto Camera = ActiveArea->make_Camera(*PlayerCharacter);

	 std::any FrameAnimations = ActiveArea->list_VisibleAnimations(Camera);
	 pushToCentralDispatch(eManagerType::GRAPHICS, eDataType::FRAMEANIMATIONS, FrameAnimations);

	 std::any CameraLocation = Camera.location;
	 pushToCentralDispatch(eManagerType::GRAPHICS, eDataType::FRAMECAMERALOCATION, CameraLocation);
 }

//Assigns a new Active Area, updates Map Limits and the Area's Player reference
void GameStateManager::setActiveArea(std::unique_ptr<Area> new_area)
{
	/* Assign the new area */
	ActiveArea.swap(new_area);
	ActiveArea->init_AI();
	ActiveArea->init_Pathfinding();

	/* Update the Player's Map Limits, save a reference for the Map and then find the player's Navmesh Tile */
	ActiveArea->setPlayerCharacter(PlayerCharacter.get());
	 auto& tmp_Tile_Ref = find_Current_Navmesh_Tile(PlayerCharacter.get()->v_Collision(), ActiveArea->Navmesh_().m_Tiles);
	 PlayerCharacter.get()->m_Gameplay.m_Player_Current_Navmesh_Tile = &tmp_Tile_Ref;
	
	// Find each Entity's current Navmesh_Tile.
	for (auto& for_Entity : ActiveArea.get()->Entity_Population())
	{
		Navmesh_Tile& tmp_Current_Tile = find_Current_Navmesh_Tile(for_Entity.v_Collision(), ActiveArea->Navmesh_().m_Tiles);
		for_Entity.AI().set_Current_Navmesh_Tile(tmp_Current_Tile);
	}
}


ActionGenerator GameStateManager::makeActionGenerator(const std::string& ActionName)
{
	// Get the Actions's Data from Central
	std::any ActionAny = pullFromCentralDispatch(eManagerType::DATA, eDataType::ACTION, ActionName);
	auto ActData = std::any_cast<ActionData>(ActionAny);
	std::any GameAny = pullFromCentralDispatch(eManagerType::DATA, eDataType::GAMEPLAYEFFECT, ActData.Name);
	auto GPlayData = std::any_cast<GameplayEffectData>(GameAny);
	std::any NewVfxAny = pullFromCentralDispatch(eManagerType::DATA, eDataType::VISUALEFFECT, GPlayData.VisualEffectName);
	std::any NewNavAny = pullFromCentralDispatch(eManagerType::DATA, eDataType::NAVIGATIONSYSTEM,GPlayData.NavigationSystemName );
	auto VfxData = std::any_cast<VisualEffectData>(NewVfxAny);
	auto NewNavData = std::any_cast<NavigationSystemData>(NewNavAny);

	// Load VFX Sprite data
	std::vector<SpriteData> SpriteArray;
	for (const auto& SpriteName : VfxData.SpriteNames)
	{
		std::any SpriteAny = pullFromCentralDispatch(eManagerType::DATA, eDataType::SPRITE, SpriteName);
		SpriteArray.push_back(std::any_cast<SpriteData>(SpriteAny));
	}

	// Load Navigation
	std::vector<NavigationPathData> PathArray;
	for (const auto& PathName : NewNavData.PathNames)
	{
		std::any PathAny = pullFromCentralDispatch(eManagerType::DATA, eDataType::NAVIGATIONPATH, PathName);
		PathArray.push_back(std::any_cast<NavigationPathData>(PathAny));
	}

	return ActionGenerator(ActData, GPlayData, NewNavData, VfxData, SpriteArray, PathArray);
}

std::unique_ptr<Area> GameStateManager::makeArea(AreaData new_area_data)
{
	std::vector<Entity> NewEntityPopulation;

	for (auto NewCharacterNameSpawn : new_area_data.Character_SpawnPoint)
	{
		/* Create Character */
		auto NewCharacterData = pullFromCentralDispatch(eManagerType::DATA, eDataType::CHARACTER, NewCharacterNameSpawn.first);
		auto NewCharacter = makeCharacter(*NewCharacterData._Cast<CharacterData>(), new_area_data.WorldSize, NewCharacterNameSpawn.second);

		/* Add to population*/
		NewEntityPopulation.push_back(NewCharacter);
	}

	for (auto NewEntityNameSpawn : new_area_data.Entity_SpawnPoint)
	{
		/* Create Entity */
		auto NewEntityData = pullFromCentralDispatch(eManagerType::DATA, eDataType::ENTITY, NewEntityNameSpawn.first);
		Entity NewEntity = makeEntity(*NewEntityData._Cast<EntityData>(), new_area_data.WorldSize, NewEntityNameSpawn.second);

		/* Add to population*/
		NewEntityPopulation.push_back(NewEntity);
	}

	return std::make_unique<Area>(new_area_data, NewEntityPopulation );
}

/* Create a new Character Entity from a Data pack */
Entity GameStateManager::makeCharacter(CharacterData new_character_data, std::optional<Vector2D> arg_MapLimits, std::optional<Vector2D> arg_SpawnPoint)
{
	std::any NewSpriteData = pullFromCentralDispatch(eManagerType::DATA, eDataType::SPRITE, new_character_data.SpriteName);
	Entity NewEntity{ new_character_data, *NewSpriteData._Cast<SpriteData>() };

	if(arg_MapLimits && arg_SpawnPoint)
	NewEntity.init_Physics(new_character_data.m_CollisionType, *arg_SpawnPoint, NewSpriteData._Cast<SpriteData>()->CollisionSize, *arg_MapLimits);

	// Populate Action Generators for the Character.
	const auto& SprData = *NewSpriteData._Cast<SpriteData>();
	for (const auto& ActionName : new_character_data.AvailableActions)
	{
		auto ActionGen = makeActionGenerator(ActionName);
		if (SprData.SpriteOffsets)
		{
			if (SprData.SpriteOffsets->count(ActionGen.viewGameplayEffect()) > 0)
			{
				ActionGen.setOffsetData(SprData.SpriteOffsets->at(ActionGen.viewGameplayEffect()));
			}
			else
			{
				std::cout << "No key found in Sprite Offsets of " << SprData.Name << " in Action Generator:  " << ActionGen.viewName() << '\n';
			}
		}
		NewEntity.a_GameplayManager().addGenerator(ActionGen);
	}
	return NewEntity;
}


Entity GameStateManager::makeEntity(EntityData new_entity_data, std::optional<Vector2D> arg_MapLimits, std::optional<Vector2D> arg_SpawnPoint)
{
	Entity NewEntity{ new_entity_data, PhysicsManager{CollisionType::IGNORE, *arg_SpawnPoint, Vector2D{2.0,2.0 }, *arg_MapLimits} };
	return NewEntity;
}

void GameStateManager::setPlayerCharacter(Entity NewPlayerCharacter, std::optional<Area*> arg_Area, std::optional<Entity*> arg_Camera)
{
	/* Assign the new Character */
	NewPlayerCharacter.set_AuthorityType(AuthorityType::PLAYER);
	PlayerCharacter = std::make_unique<Entity>(NewPlayerCharacter);
	std::cout << "Player Character Global ID: " << PlayerCharacter->GlobalID << '\n';
	PlayerCharacter.get()->a_GameplayManager().Stats().set_Faction(EntityFaction::PLAYER);
	/* Update the Player Character references on the Commander and GameCamera */

	// Set the character is a valid Area is present
	if(arg_Area)
		arg_Area.value()->setPlayerCharacter(PlayerCharacter.get());

}


eManagerType GameStateManager::viewMyManagerType()
{
	return eManagerType::GAMESTATE;
}

std::any GameStateManager::searchForData(const eDataType& reqDataType, const std::string& reqDataName)
{
	return std::any();
}

/* 
Manage incoming Data.
*/
void GameStateManager::receiveFromCentralDispatch(std::any inbound_data, const eDataType& inbound_data_type)
{
	if (inbound_data_type == eDataType::AREA)
	{
		/* Make and Set the new Active Area */
		auto NewArea = makeArea(*inbound_data._Cast<AreaData>());

		setActiveArea(std::move(NewArea));

		for (auto& f_nav : ActiveArea->Navmesh_().m_Tiles)
		{
			for (auto& f_navv : f_nav.m_Edges)
			{
				auto reff = traverse_Navmesh_Edge(f_nav, *f_navv);
			}
		}
	}

	if (inbound_data_type == eDataType::PLAYERCHARACTER)
	{
		/* Make and Set new Player Character */
		PlayerCharacterData = *inbound_data._Cast<CharacterData>();
		Entity NewPlayerCharacter = makeCharacter(*inbound_data._Cast<CharacterData>());
		setPlayerCharacter(NewPlayerCharacter);
	}
}


