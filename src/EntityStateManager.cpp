// Project
#include "eFacingDirections.h"
#include "eEntityStateType.h"
#include "eMovementType.h"
#include "EntityStates.h"
#include "EntityStateManager.h"
#include "MoveTransitions.h"
#include "ActionTransitions.h"

// STL
#include <functional>
#include <tuple>
#include <utility>
#include <optional>
#include <iostream>
#include <variant>
#include <chrono>


EntityStateManager::EntityStateManager()
{}

void EntityStateManager::add_MovementEvent(const EntityEvent& new_event)
{
	/* Do nothing if the state has not changed */
	if (m_CurrentMovementState == new_event.EventState)
		return;

	// If this is a valid transition, update the CurrentMovementState and LatestMovementEvent
	if (m_LatestMovementEvent.checkValidTo(new_event) == true)
	{
		m_CurrentMovementState = new_event.EventState;
		m_LatestMovementEvent = new_event;
	}

	// Update the MostRecentMovementCommand
	m_MostRecentMovementCommand = new_event;
}

void EntityStateManager::add_ActionEvent(const EntityEvent& new_event)
{
	// The Command manager reports states each frame, prevent it from updating the CurrentActionState
	// unless it is set to Idle and the incoming Action is NOT Idle. Otherwise, Actions will fire off
	// 3-4 times pre key press as there can be multiple frames between the Press and Release events.
	if (m_CurrentActionState == sActionIdle && new_event.EventState.Action != StateType::IDLE)
	{
		m_MostRecentActionCommand = new_event;
	}
}

auto EntityStateManager::select_FrameMovementState(const MovementType& arg_MoveType_X
	, const MovementType& arg_MoveType_Y, const EntityState& arg_CurrentMovementState
	,const FacingDirections& arg_SpriteFacing)
{
	// Function to compare the arg_CurrentMovementState against Cardinal directions
	auto Cardinal = [&arg_CurrentMovementState](const std::pair<EntityState, CardinalFacings>& s) -> bool
	{
		if (s.first.Action == arg_CurrentMovementState.Action && s.first.Facing == arg_CurrentMovementState.Facing)
		{
			return true;
		}
		return false;
	};

	// Function to compare the arg_CurrentMovementState against InterCardinal directions
	auto InterCardinal = [&arg_CurrentMovementState](const std::pair<EntityState, InterCardinalFacings>& s) -> bool
	{
		if (s.first.Action == arg_CurrentMovementState.Action && s.first.Facing == arg_CurrentMovementState.Facing)
		{
			return true;
		}
		return false;
	};

	// Check if we're going Up/Down/Left/Right
	if (auto if_MovePair = std::find_if(CardinalTransitions.begin(), CardinalTransitions.end(), Cardinal); if_MovePair != CardinalTransitions.end())
	{
		auto MoveData = if_MovePair->second(arg_MoveType_X, arg_MoveType_Y);
		return MoveData;
	}

	// Check if we're going UpLeft/UpRight/DownLeft/DownRight
	if (auto if_MovePair = std::find_if(InterCardinalTransitions.begin(), InterCardinalTransitions.end(), InterCardinal); if_MovePair != InterCardinalTransitions.end())
	{
		auto MoveData = if_MovePair->second(arg_MoveType_X, arg_MoveType_Y, arg_SpriteFacing);

		return MoveData;
	}

	// Check if we're going Idle
	if (arg_CurrentMovementState == p_MoveIdle.first)
	{
		auto MoveData = p_MoveIdle.second(arg_SpriteFacing);	//That or Current Action Facing
		return MoveData;
	}
	std::cout << "WARNING, FAILED TO RESOLVE FRAME_MOVEMENT_STATE\n";	
	return std::tuple<MovementType, MovementType, FacingDirections, StateType>();
}
std::tuple<std::optional<EntityEvent>, FacingDirections, MovementType, MovementType, StateType> EntityStateManager::run_FrameLogic(
	 bool arg_isActiveActionCompleted, const std::tuple<EntityEvent, EntityEvent>& arg_CommandsMovementAction)
{
	const auto& [tmp_MovementCommand, tmp_ActionCommand] = arg_CommandsMovementAction;
	add_MovementEvent(tmp_MovementCommand);
	add_ActionEvent(tmp_ActionCommand);

	// If the Entity's Gameplay manager reports the current action is completed,
	// that means our current action state is Idle
	if (arg_isActiveActionCompleted)
	{
		m_CurrentActionState.Action = StateType::IDLE;
	//	std::cout << "Gameplay reports IDLE\n";
	}

	// If a Command was accepted this frame
	if (m_MostRecentActionCommand)
	{	
		m_CurrentActionState = m_MostRecentActionCommand->EventState;
		m_LatestActionEvent = *m_MostRecentActionCommand;
		m_LatestActionEvent_Start = std::chrono::steady_clock::now();
		m_CurrentActionFacing = m_SpriteFacing;	// Saves the rotation when the event starts
		//std::cout<<"Planning An Action\n";	
	}

	// Resolve the effect of any add_MovementEvent(const EntityEvent& new_event) received from the Command Manager prior to the GameState firing
	// Returns: std::tuple<MovementType, MovementType, FacingDirections, StateType>
	const auto& [ret_MovingState_X_FromMove, ret_MovingState_Y_FromMove, ret_SpriteFacing_FromMove, ret_ActionState_FromMove] = select_FrameMovementState(m_MovingState_X, m_MovingState_Y, m_CurrentMovementState, m_SpriteFacing);
	m_MovingState_X = ret_MovingState_X_FromMove;
	m_MovingState_Y = ret_MovingState_Y_FromMove;
	m_SpriteFacing = ret_SpriteFacing_FromMove;
	m_ActionState = ret_ActionState_FromMove;


	// Resolve the effect of any add_ActionEvent received from the Command Manager this frame
	const auto& ret_opt_FrameActionState = select_FrameActionState(m_CurrentActionState, m_CurrentActionFacing);

	// If we did not find a Action Transition Match in FrameActionState, run the Idle transition
	if (!ret_opt_FrameActionState)
	{
		m_CurrentActionState = EventActionIdle.EventState;
		m_LatestActionEvent = EventActionIdle;
		//std::cout << "NO Action State Match Found, we are Idle\n";
	}

	// If we found a valid Action Transition match, update the relevant data
	if (ret_opt_FrameActionState)
	{
		const auto& [ret_MovingState_X_FromAction, ret_MovingState_Y_FromAction, ret_SpriteFacing_FromAction, ret_ActionState_FromAction] = *ret_opt_FrameActionState;
		m_MovingState_X = ret_MovingState_X_FromAction;
		m_MovingState_Y = ret_MovingState_Y_FromAction;
		m_SpriteFacing = ret_SpriteFacing_FromAction;
		m_ActionState = ret_ActionState_FromAction;
		//std::cout << "Action Match Found\n";
	}

	// Pack up the return data of the function
	std::tuple<std::optional<EntityEvent>, FacingDirections, MovementType, MovementType, StateType> tmp_FrameLogicState = std::make_tuple(m_MostRecentActionCommand, m_SpriteFacing, m_MovingState_X, m_MovingState_Y, m_ActionState);

	// Wipe the latest data received from Command
	m_MostRecentMovementCommand.reset();
	m_MostRecentActionCommand.reset();

	return tmp_FrameLogicState;
	
}

const MovementType& EntityStateManager::v_Moving_State_X()
{
	return m_MovingState_X;
}

const MovementType& EntityStateManager::v_Moving_State_Y()
{
	return m_MovingState_Y;
}

std::optional<std::tuple<MovementType, MovementType, FacingDirections, StateType>> EntityStateManager::select_FrameActionState(const EntityState& arg_CurrentActionState
	, const FacingDirections& arg_CurrentActionFacing)
{	// Search for a valid action transition with arg_CurrentActionState
	auto ActionSwitchSearch = [&arg_CurrentActionState](const std::pair<EntityState, ActionSwitch>& s) -> bool
	{
		if (s.first.Action == arg_CurrentActionState.Action && s.first.Facing == arg_CurrentActionState.Facing)
		{
			return true;
		}
		return false;
	};

	//Search the ActionTransitions for a match with arg_CurrentActionState 
	if (auto if_ActionPair = std::find_if(ActionTransitions.begin(), ActionTransitions.end(), ActionSwitchSearch); if_ActionPair != ActionTransitions.end())
	{	// If we find a match, run the transition then return the CurrentActionState, LatestActionEvent
		std::tuple<MovementType, MovementType, FacingDirections, StateType> ActionData = if_ActionPair->second(arg_CurrentActionFacing);
		return ActionData;
	}

	// If we cannot find a match, return std::nullopt so we know to run the Idle transition
	return std::nullopt;
}

const FacingDirections& EntityStateManager::v_Sprite_Facing() const
{
	return m_SpriteFacing;
}
