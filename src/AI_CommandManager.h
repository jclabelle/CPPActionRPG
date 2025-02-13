#pragma once

// Project
#include "EntityStates.h"
#include "GameplayManager.h"
#include "PhysicsManager.h"
#include "eFacingDirections.h"
#include "eEntityStateType.h"
#include "eMovementType.h"
#include "AI_PerceptionManager.h"
#include "AStar.h"
#include "AIConstants.h"
#include "functions_Pathfinding.h"

// STL
#include <optional>

struct AI_CommandManager
{
	AI_CommandManager(GameplayManager& arg_Gameplay);
	std::tuple<EntityEvent, EntityEvent> run_FrameLogic(const Vector2D& arg_EntityWorldLoc, const FacingDirections& arg_SpriteFacing, bool arg_isActiveActionCompleted);
	const bool has_Perception() const;
	void init_Perception();
	AI_PerceptionManager& Perception();
	const std::pair<Vector2D, Vector2D>& debug_LineToNearestEnemy();
	const std::vector<Vector2D>& debug_CurrentPath();
	void init_Pathfinding(AStarGraph& arg_pathing);
	void register_CollisionWithMap(bool arg_b, SquareCollision& arg_square_collision);
	void register_CollisionWithEntity(bool arg_b, SquareCollision& arg_square_collision, AI_CommandManager& arg_ai);
	void save_LastFrameMovementEvent(const EntityEvent& arg_move_event);
	void request_StopMoving(bool b);
	void set_Current_Navmesh_Tile(Navmesh_Tile& arg_navmesh_tile);
	Navmesh_Tile* Current_Navmesh_Tile();

	

	std::optional<AI_PerceptionManager> m_Perception;
	std::vector<std::pair<Vector2D, Vector2D>> m_debug_Sighted_LineStart_LineEnd;
	std::pair<Vector2D, Vector2D> m_debug_LineToNearestEnemy;
	std::vector<Vector2D> m_debug_CurrentPath;
	std::optional<PerceptionData> find_NearestEnemy(const std::map<uint64_t, PerceptionData>& PerceivedPop);
	GameplayManager* m_Gameplay;

	// Pathfinding
	AStarGraph* m_Pathfinding;
	std::deque<std::reference_wrapper<AStarNode>> m_Path;
	size_t m_PathProgress{ 0 };
	double m_CloseQuartersMaxDistance{20}; // The max distance between the AI and a target for close quarter actions such as melee combat.
	std::chrono::steady_clock::time_point m_LatestPathRefresh{ std::chrono::steady_clock::now() };
	std::chrono::steady_clock::duration m_PathRefreshFrequency{ std::chrono::seconds(1) };

	// Pathfinding_AStar_SSFA_Steering
	Navmesh_Tile* m_Current_Navmesh_Tile{ nullptr };
	std::vector<Navmesh_Waypoint> tmp_Path;

	// Manage Collisions with Map
	bool m_CollisionWithMap{ false };
	SquareCollision* m_CurrentCollidingObject{nullptr};
	EntityEvent m_LastFrameMovementEvent{ EventIdle };
	EntityState m_MovementAtCollisionEvent{ sIdle };
	int m_ClearanceBuffer{ const_CollisionClearance_Map };	// # of frames the AI will keep walking away from an obstance
	
	// Manage Collisions with Entities
	bool m_CollisionWithEntity{ false };
	SquareCollision* m_CurrentCollidingEntity_Collision{ nullptr };
	std::vector<AI_CommandManager*> m_CurrentCollidingEntity_AI;
	EntityEvent m_LastFrameMovementEvent_EntityCollision{ EventIdle };
	EntityState m_MovementAtCollisionEvent_EntityCollision{ sIdle };
	int m_ClearanceBuffer_EntityCollision{ const_CollisionClearance_Entity };	// # of frames the AI will keep walking away from an obstance
	bool m_StopMovingRequest{ false };
};

const EntityEvent& find_MovementState_from_NormalRayTrace(const Vector2D& arg_normal_ray_trace);
