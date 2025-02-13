#pragma once
// Project
#include "eFacingDirections.h"
#include "eEntityStateType.h"
#include "eMovementType.h"
#include "EntityStates.h"
#include "MoveTransitions.h"
#include "ActionTransitions.h"
// STL
#include <map>
#include <tuple>
#include <utility>
#include <functional>
#include <optional>
#include <chrono>
#include <variant>

class EntityStateManager
{
public:
	EntityStateManager();
	void add_MovementEvent(const EntityEvent& new_event);
	void add_ActionEvent(const EntityEvent& new_event);

	std::optional<std::tuple<MovementType, MovementType, FacingDirections, StateType>> select_FrameActionState(const EntityState& arg_CurrentActionState, const FacingDirections& arg_CurrentActionFacing);
	
	auto select_FrameMovementState(const MovementType& arg_MoveType_X, const MovementType& arg_MoveType_Y, const EntityState& arg_CurrentMovementState, const FacingDirections& arg_SpriteFacing);

	std::tuple<std::optional<EntityEvent>, FacingDirections, MovementType, MovementType, StateType> run_FrameLogic(bool arg_isActiveActionCompleted, const std::tuple<EntityEvent, EntityEvent>& arg_CommandsMovementAction);
	// Viewers
	const MovementType& v_Moving_State_X();
	const MovementType& v_Moving_State_Y();
	const FacingDirections& v_Sprite_Facing() const;

	//FacingDirections& v_SpriteFacing() { return m_SpriteFacing; }
	std::optional<EntityEvent> v_m_MostRecentActionCommand_EventState_Action() {return m_MostRecentActionCommand;}
	void set_m_LatestActionEvent_Duration(const std::chrono::steady_clock::duration& d) { m_LatestActionEvent_Duration = d; }
	void set_m_CurrentActionState(const StateType& arg_CurrentActionState) { m_CurrentActionState.Action = arg_CurrentActionState; }
	const StateType& v_ActionState() { return m_ActionState; }

private:

	FacingDirections m_SpriteFacing{ FacingDirections::DOWN };
	FacingDirections m_CurrentActionFacing{ FacingDirections::DOWN };
	StateType m_ActionState{ StateType::IDLE };
	MovementType m_MovingState_X{ MovementType::STATIONARY };
	MovementType m_MovingState_Y{ MovementType::STATIONARY };
	EntityState m_CurrentMovementState{ sIdle };
	EntityState m_CurrentActionState{ sIdle };
	EntityEvent m_LatestMovementEvent{ EventIdle };
	EntityEvent m_LatestActionEvent{ EventIdle };
	std::chrono::steady_clock::time_point m_LatestActionEvent_Start{};
	std::chrono::steady_clock::duration m_LatestActionEvent_Duration{};
	std::chrono::steady_clock::duration m_SpriteAnimDuration{};


	// Most recent event from Commands
	std::optional<EntityEvent> m_MostRecentMovementCommand{ std::nullopt };
	std::optional<EntityEvent> m_MostRecentActionCommand{ std::nullopt };

};

