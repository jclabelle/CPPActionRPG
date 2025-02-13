#pragma once
/* Project*/
#include "Thing.h"
#include "TestingFunctions.h"
#include "GeometryStructures.h"
#include "WorldData.h"
#include "eFacingDirections.h"
#include "eMovementType.h"
#include "eEntityStateType.h"
#include "eAuthorityType.h"
#include "EntityStates.h"
#include "Chronometer.h"
#include "Collisions2D.h"
#include "GamePlayManager.h"
#include "Sprite.h"
#include "PhysicsManager.h"
#include "EntityStateManager.h"
#include "AI_CommandManager.h"

/* STL */
#include <memory>
#include <functional>
#include <optional>

/*
Base class for interactions within the context of the game world.
*/

struct Entity : public Thing
{

	Entity();
	Entity(EntityData new_entity_data);
	Entity(EntityData arg_EntityData, PhysicsManager arg_PhysicsManager);
	Entity(CharacterData new_character_data, SpriteData new_sprite_data);

	/* View methods*/
	const std::string& v_Name() const;
	Navmesh_Tile* get_CurrentNavmesh_Tile();

	// State
	const EntityStateManager& v_State() const;
	
	// Gameplay Manager
	std::optional<std::vector<std::reference_wrapper<GameplayEffect>>> list_VisibleEffects();
	GameplayManager& a_GameplayManager();
	const GameplayManager& v_Gameplay() const;
	void add_GameplayEffect(GameplayEffect& new_effect);

	// Physics
	const Vector2D& v_WorldLocation() const;
	const SquareCollision& v_Collision() const;
	bool has_Collisions() const;
	std::reference_wrapper<PhysicsManager> Physics();
	bool is_Colliding(const SquareCollision& arg_Collider);
	void init_Physics(CollisionType arg_CollisionType, Vector2D arg_WorldLocation, Vector2D arg_CollisionSize, Vector2D arg_MapLimits);

	// Sprite
	 const std::optional<Sprite>& v_Sprite();
	 const Vector2D& v_SpriteSize();
	 std::optional<Sprite>& a_Sprite();
	 Vector2D v_SpriteLocation() const;
	 const Vector2D& v_SpriteCollisionSize();
	 int a_ZDepth();
	 const bool is_Visible() const;

	 // AI
	 AI_CommandManager& AI();
	 const AI_CommandManager& v_AI() const;
	 void initAI();
	 bool has_AI();



	/* Test/Debug */
	void PrintToCmdLine();

	/* Assignment */
	void set_AreaMapLimits(const Vector2D& new_limits);
	void set_AuthorityType(const AuthorityType& arg_Authority);


	/* Add */
	void add_MovementEvent(const EntityEvent& new_event);
	void add_ActionEvent(const EntityEvent& new_event);

	/* Frame actions */
	//void run_FrameLogic() override;
	void run_FrameLogic_V2(const std::tuple<EntityEvent, EntityEvent>& arg_CommandsMovementAction);
	void run_FrameCleanup();

	/* GENERAL */
	std::string m_Name{ "DefaultEntityName" };
	int zDepth{ 100 };	// Default zDepth for characters
	AuthorityType m_ControllerType{ AuthorityType::AI };

/* PHYSICS, SIZE, MOVEMENT */
	
	/* From: EntityData or CharacterData */
	//double m_MoveSpeed{ 2 };
	std::optional<CharacterData> m_Data;

	/* Managers */
	PhysicsManager m_Physics;
	std::optional<EntityStateManager> m_State{};
	GameplayManager m_Gameplay{};
	std::optional<Sprite> m_Sprite{ std::nullopt };
	std::optional<AI_CommandManager> m_AI{std::nullopt};

	
	StateType ActionState{ StateType::IDLE };


	// Player Specific
	Navmesh_Tile* get_CurrentNavmesh_Tile_Player_Ptr();



};