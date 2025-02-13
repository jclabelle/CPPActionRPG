#pragma once
/* Project */
#include "Action.h"
#include "ActionGenerator.h"
#include "EntityStates.h"
#include "WorldData.h"
#include "GameStats.h"
#include "classes_Pathfinding.h"
/* STL */
#include <memory>
#include <functional>



struct GameplayManager
{
	GameplayManager();
	bool isActiveActionCompleted();
	void startNewAction(const EntityState& new_state
		,const uint64_t creator_id
		, const Vector2D& creator_location
		, const std::chrono::steady_clock::duration& action_duration
		, const FacingDirections& owner_facing);
	void run_FrameCleanup();
	void runFrameLogic();
	void addGenerator(const ActionGenerator& new_action_generator) ;
	void setActionOffsets(const std::map<GameEffect, ActionOffsets>& new_action_offsets);
	std::vector<GameplayEffect>& accessOwnedEffects();	// Returns pointers to OwnedEffects
	GameStats& Stats();
	const GameStats& v_Stats() const;
	
	/* Actions */
	ActionGenerator m_GeneratorActionSpecialOne;
	ActionGenerator m_GeneratorActionSpecialTwo;
	ActionGenerator m_GeneratorActionSpecialThree;
	ActionGenerator m_GeneratorActionBasicAttack;
	ActionGenerator m_GeneratorActionIdle;
	Action ActiveAction;
	std::map<std::string, ActionGenerator> AvailableActions;
	std::map<GameEffect, ActionOffsets> EntityOffsets;

	void moveEffectToOwnedList();

	/* Owner Active Effects */
	std::vector<GameplayEffect> OwnedEffects;	// Effects caused/triggered by this Entity

	// GameStats
	GameStats m_Stats{};

	// Nav
	Navmesh_Tile* m_Player_Current_Navmesh_Tile{ nullptr };



};