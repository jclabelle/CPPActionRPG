//#define DEBUG_DRAW_CONTROLS
//#define DEBUG_DRAW_COLLISIONTEXT
#define DEBUG_DEACTIVATE_AI_MOVEMENT
/* Project */
#include "Entity.h"
#include "Chronometer.h"
#include "WorldData.h"
#include "eFacingDirections.h"
#include "eEntityStateType.h"
#include "eMovementType.h"
#include "EntityStates.h"
#include "Collisions2D.h"
#include "GameplayManager.h"
/* STL */
#include <optional>
#include <iostream>
#include <cmath>
#include <ranges>

// Constructor
Entity::Entity()
{
	m_State = EntityStateManager{};
}

Entity::Entity(EntityData new_entity_data)
	: m_Name{ new_entity_data.Name }
{
	m_State = EntityStateManager{};
}

Entity::Entity(EntityData arg_EntityData, PhysicsManager arg_PhysicsManager)
	:m_Name{arg_EntityData.Name}
	, m_Physics{arg_PhysicsManager}
{
	m_State = EntityStateManager{};
}

Entity::Entity(CharacterData new_character_data, SpriteData new_sprite_data)
	: m_Name{ new_character_data.Name }
	, m_Sprite{ new_sprite_data }
	, m_Data{ new_character_data }
{
	m_State = EntityStateManager{};
}

// Viewers
const std::string& Entity::v_Name() const { return m_Name; }

Navmesh_Tile* Entity::get_CurrentNavmesh_Tile()
{
	return (m_AI.has_value()) ? m_AI->Current_Navmesh_Tile() : m_Gameplay.m_Player_Current_Navmesh_Tile;
}

Navmesh_Tile* Entity::get_CurrentNavmesh_Tile_Player_Ptr()
{
	return m_Gameplay.m_Player_Current_Navmesh_Tile;
}

const EntityStateManager& Entity::v_State() const
{
	return m_State.value();
}

const Vector2D& Entity::v_WorldLocation() const { return m_Physics.v_WorldLocation();}

Vector2D Entity::v_SpriteLocation() const
{
	return Vector2D{ m_Physics.v_WorldLocation().x - m_Sprite->viewCollisionOffset().x - (m_Physics.v_Size().x / 2)
	,m_Physics.v_WorldLocation().y - m_Sprite->viewCollisionOffset().y - (m_Physics.v_Size().y / 2) };
}

const Vector2D& Entity::v_SpriteCollisionSize() { return m_Sprite->viewCollisionSize(); }

const SquareCollision& Entity::v_Collision() const{ return m_Physics.v_Collisions(); }

bool Entity::has_Collisions() const
{
	if (m_Physics.v_CollisionsType() != CollisionType::IGNORE && m_Physics.v_CollisionsType() != CollisionType::MAX_COLLISIONS)
			return true;

	return false;
}

const bool Entity::is_Visible() const
{
	return m_Sprite ? true : false;
}

AI_CommandManager& Entity::AI()
{
	return *m_AI;
}

const AI_CommandManager& Entity::v_AI() const
{
	return *m_AI;
}

void Entity::initAI()
{
	m_AI = AI_CommandManager(m_Gameplay);

	m_AI->Perception().set_OwnerCollisions(m_Physics.get_CollisionsReference());
}

bool Entity::has_AI()
{
	return m_AI.has_value() ? true : false;
}

std::optional<std::vector<std::reference_wrapper<GameplayEffect>>> Entity::list_VisibleEffects()
{
	auto EffectVisible = [](const GameplayEffect& e) { return e.isVisible(); };

	std::optional<std::vector<std::reference_wrapper<GameplayEffect>>> ret_VisibleEffects;

	for (GameplayEffect& for_Effect : a_GameplayManager().accessOwnedEffects() | std::views::filter(EffectVisible))
	{
		std::reference_wrapper< GameplayEffect> ref = std::ref(for_Effect);
		ret_VisibleEffects->push_back(ref);
	}

	return ret_VisibleEffects;
}

std::reference_wrapper<PhysicsManager> Entity::Physics()
{
	return std::reference_wrapper<PhysicsManager>{m_Physics};
}

bool Entity::is_Colliding(const SquareCollision& arg_Collider)
{
	return m_Physics.is_Colliding(arg_Collider, m_State->v_Moving_State_X(), m_State->v_Moving_State_Y());
}

const std::optional<Sprite>& Entity::v_Sprite()
{
	return *&m_Sprite;
}

const Vector2D& Entity::v_SpriteSize()
{
	return *&m_Sprite->viewSize();
}

std::optional<Sprite>& Entity::a_Sprite()
{
	return *&m_Sprite;
}

int Entity::a_ZDepth()
{
	return zDepth;
}

GameplayManager& Entity::a_GameplayManager()
{
	return m_Gameplay;
}

const GameplayManager& Entity::v_Gameplay() const
{
	return m_Gameplay;
}

void Entity::PrintToCmdLine()
{}

void Entity::set_AreaMapLimits(const Vector2D& arg_MapLimits)
{
	m_Physics.set_AreaMapLimits(arg_MapLimits);
}

void Entity::run_FrameLogic_V2(const std::tuple<EntityEvent, EntityEvent>& arg_CommandsMovementAction)
{
	if (m_ControllerType == AuthorityType::PLAYER)
	{

		auto [ret_opt_MostRecentActionCommand, ret_SpriteFacing, ret_MovingState_X, ret_MovingState_Y, ret_ActionState] = m_State->run_FrameLogic(m_Gameplay.isActiveActionCompleted(), arg_CommandsMovementAction);

		// If we have a Sprite and the most recent action command has a value
		if (m_Sprite && ret_opt_MostRecentActionCommand.has_value())
		{
			m_State->set_m_LatestActionEvent_Duration(m_Sprite->viewFacing_ActionDuration(ret_SpriteFacing, ret_opt_MostRecentActionCommand->EventState.Action));
		}

		if (ret_opt_MostRecentActionCommand.has_value())
		{
			m_Gameplay.startNewAction(EntityState{ ret_opt_MostRecentActionCommand->EventState.Action, ret_SpriteFacing }, GlobalID, v_WorldLocation(), m_Sprite->viewFacing_ActionDuration(ret_SpriteFacing, ret_opt_MostRecentActionCommand->EventState.Action), ret_SpriteFacing);
		}

		m_Gameplay.runFrameLogic();

		m_Physics.runFrameLogic(ret_MovingState_X, ret_MovingState_Y);

		if (m_Sprite.has_value())
		{
			m_Sprite->setFacing_Action(ret_SpriteFacing, ret_ActionState);
		}
	}

	if (m_ControllerType == AuthorityType::AI)
	{

		const auto& ret_CommandsMovementAction = m_AI->run_FrameLogic(m_Physics.v_WorldLocation(), m_State->v_Sprite_Facing(), m_Gameplay.isActiveActionCompleted());
	    m_AI->save_LastFrameMovementEvent( ret_CommandsMovementAction._Myfirst._Val);

		auto [ret_opt_MostRecentActionCommand, ret_SpriteFacing, ret_MovingState_X, ret_MovingState_Y, ret_ActionState] = m_State->run_FrameLogic(m_Gameplay.isActiveActionCompleted(), ret_CommandsMovementAction);

		// If we have a Sprite and the most recent action command has a value
		if (m_Sprite && ret_opt_MostRecentActionCommand.has_value())
		{
			m_State->set_m_LatestActionEvent_Duration(m_Sprite->viewFacing_ActionDuration(ret_SpriteFacing, ret_opt_MostRecentActionCommand->EventState.Action));
		}

		if (ret_opt_MostRecentActionCommand.has_value())
		{
			m_Gameplay.startNewAction(EntityState{ ret_opt_MostRecentActionCommand->EventState.Action, ret_SpriteFacing }, GlobalID, v_WorldLocation(), m_Sprite->viewFacing_ActionDuration(ret_SpriteFacing, ret_opt_MostRecentActionCommand->EventState.Action), ret_SpriteFacing);
		}

		m_Gameplay.runFrameLogic();


		m_Physics.set_Speed(1);
		m_Physics.runFrameLogic(ret_MovingState_X, ret_MovingState_Y);
		

		if (m_Sprite.has_value())
		{
			m_Sprite->setFacing_Action(m_State->v_Sprite_Facing(), ret_ActionState);
		}
	}
}


void Entity::run_FrameCleanup()
{
	m_Gameplay.run_FrameCleanup();
}

void Entity::set_AuthorityType(const AuthorityType& arg_Authority)
{
	m_ControllerType = arg_Authority;
}

void Entity::init_Physics(CollisionType arg_CollisionType, Vector2D arg_WorldLocation, Vector2D arg_CollisionSize, Vector2D arg_MapLimits)
{
	m_Physics = PhysicsManager{ arg_CollisionType, arg_WorldLocation, arg_CollisionSize, arg_MapLimits };
}

void Entity::add_MovementEvent(const EntityEvent& new_event)
{
	m_State->add_MovementEvent(new_event);
}

void Entity::add_ActionEvent(const EntityEvent& new_event)
{
	m_State->add_ActionEvent(new_event);
}

void Entity::add_GameplayEffect(GameplayEffect &new_effect)
{
	if (new_effect.viewLifeTimeIndex() == 0)
	{
		std::cout << "CHRONO GAMEPLAY EFFECT" << '\n';
	}

	if (new_effect.viewLifeTimeIndex() == 1)
	{
		std::cout << "CHARGE GAMEPLAY EFFECT" << '\n';
	}

	if (new_effect.viewLifeTimeIndex() == 2)
	{
		std::cout << "INSTANT GAMEPLAY EFFECT" << '\n';
	}
}
