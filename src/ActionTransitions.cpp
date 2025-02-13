//Project
#include "ActionTransitions.h"
#include "MoveTransitions.h"
#include "eFacingDirections.h"
#include "eMovementType.h"
#include "EntityStates.h"
//STL
#include <tuple>
#include <variant>
#include <optional>


constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> ActionOne(const FacingDirections& arg_CurrentActionFacing)
{
	/* Set the action, prevent movement and prevent changes in Rotation */
	StateType ActionState = StateType::ATTACK_BASIC;
	MovementType MoveType_X = MovementType::DECELERATION_LEFT;
	MovementType MoveType_Y = MovementType::DECELERATION_UP;
	FacingDirections SpriteFacing = arg_CurrentActionFacing;
#ifdef DEBUG_DRAW_CONTROLS
	std::cout << "ACTION ONE" << '\n';
#endif
	return std::make_tuple(MoveType_X, MoveType_Y, SpriteFacing, ActionState);

}

constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> ActionTwo(const FacingDirections& arg_CurrentActionFacing)
{
	StateType ActionState = StateType::ACTION_SPECIAL_TWO;
	MovementType MoveType_X = MovementType::DECELERATION_LEFT;
	MovementType MoveType_Y = MovementType::DECELERATION_UP;
	FacingDirections SpriteFacing = arg_CurrentActionFacing;
#ifdef DEBUG_DRAW_CONTROLS
	std::cout << "ACTION ONE" << '\n';
#endif
	return std::make_tuple(MoveType_X, MoveType_Y, SpriteFacing, ActionState);

}

constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> ActionThree(const FacingDirections& arg_CurrentActionFacing)
{
	StateType ActionState = StateType::ACTION_SPECIAL_THREE;
	MovementType MoveType_X = MovementType::DECELERATION_LEFT;
	MovementType MoveType_Y = MovementType::DECELERATION_UP;
	FacingDirections SpriteFacing = arg_CurrentActionFacing;
#ifdef DEBUG_DRAW_CONTROLS
	std::cout << "ACTION ONE" << '\n';
#endif
	return std::make_tuple(MoveType_X, MoveType_Y, SpriteFacing, ActionState);

}

constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> ActionFour(const FacingDirections& arg_CurrentActionFacing)
{
	StateType ActionState = StateType::ACTION_SPECIAL_FOUR;
	MovementType MoveType_X = MovementType::DECELERATION_LEFT;
	MovementType MoveType_Y = MovementType::DECELERATION_UP;
	FacingDirections SpriteFacing = arg_CurrentActionFacing;
#ifdef DEBUG_DRAW_CONTROLS
	std::cout << "ACTION ONE" << '\n';
#endif
	return std::make_tuple(MoveType_X, MoveType_Y, SpriteFacing, ActionState);

}

//auto ActionFailedMatch()
////std::tuple<EntityState, EntityEvent> ActionIdle()
//{
//	EntityState CurrentActionState = EventActionIdle.EventState;
//	EntityEvent LatestActionEvent = EventActionIdle;
//	std::tuple<EntityState, EntityEvent> FailedMatch = std::make_tuple(CurrentActionState, LatestActionEvent);
//	return FailedMatch;
//}