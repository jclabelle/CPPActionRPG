#pragma once
/* Project */
#include "eEntityStateType.h"
#include "eFacingDirections.h"
#include "eMovementType.h"
/* STL */
#include <vector>
//#include <tuple>
//#include <variant>
//#include <functional>

struct EntityState
{
	EntityState(StateType new_action, FacingDirections new_facing)
	:Action{new_action}, Facing{new_facing} {}
	
	StateType Action;
	FacingDirections Facing;

	bool operator==(const EntityState& b);
};

bool operator==(const EntityState& a, const EntityState& b);



struct EntityEvent
{
	/* State triggered by event, list of states it can transition to */
	EntityEvent(const EntityState state, const std::initializer_list<EntityState> valid_to)
		: EventState{ state }, ValidTo{ valid_to } {}
	EntityState EventState;
	std::vector<EntityState> ValidTo;	// List of valid states that can replace the EventState

	bool checkValidTo(const EntityEvent& new_event) const;	// Check if an Event has a State that can replace this Event's State
};

// States are combinations of Types and Facing(faked rotation)
// Cardinal Movement States
 const EntityState sMovingUp{ StateType::WALK, FacingDirections::UP};
 const EntityState sMovingDown{ StateType::WALK, FacingDirections::DOWN };
 const EntityState sMovingLeft{ StateType::WALK, FacingDirections::LEFT };
 const EntityState sMovingRight{ StateType::WALK, FacingDirections::RIGHT };
 // InterCardinal Movement States
 const EntityState sMovingUpLeft{ StateType::WALK, FacingDirections::UPLEFT };
 const EntityState sMovingUpRight{ StateType::WALK, FacingDirections::UPRIGHT };
 const EntityState sMovingDownLeft{ StateType::WALK, FacingDirections::DOWNLEFT };
 const EntityState sMovingDownRight{ StateType::WALK, FacingDirections::DOWNRIGHT };
 // Idle Movement State
 const EntityState sIdle{ StateType::IDLE, FacingDirections::CURRENT };

 // Action States
 const EntityState sActionIdle{ StateType::IDLE, FacingDirections::CURRENT };
 const EntityState sAction{ StateType::ATTACK_BASIC, FacingDirections::CURRENT };
 const EntityState sActionOne{ StateType::ATTACK_BASIC, FacingDirections::CURRENT };
 const EntityState sActionTwo{ StateType::ACTION_SPECIAL_ONE, FacingDirections::CURRENT };
 const EntityState sActionThree{ StateType::ACTION_SPECIAL_TWO, FacingDirections::CURRENT };
 const EntityState sActionFour{ StateType::ACTION_SPECIAL_THREE, FacingDirections::CURRENT };

 // Events represent a single State, and the others states that since state is allowed to transition to
 // Cardinal
 const EntityEvent EventMoveUp{ sMovingUp, {sIdle, sAction, sMovingUp, sMovingDown, sMovingLeft, sMovingRight, sMovingUpLeft, sMovingUpRight, sMovingDownLeft, sMovingDownRight } };
 const EntityEvent EventMoveDown{ sMovingDown, {sIdle, sAction,  sMovingUp, sMovingDown, sMovingLeft, sMovingRight, sMovingUpLeft, sMovingUpRight, sMovingDownLeft, sMovingDownRight } };
 const EntityEvent EventMoveLeft{ sMovingLeft, {sIdle, sAction,  sMovingUp, sMovingDown, sMovingLeft, sMovingRight, sMovingUpLeft, sMovingUpRight, sMovingDownLeft, sMovingDownRight } };
 const EntityEvent EventMoveRight{ sMovingRight, {sIdle, sAction,  sMovingUp, sMovingDown, sMovingLeft, sMovingRight, sMovingUpLeft, sMovingUpRight, sMovingDownLeft, sMovingDownRight} };
 // InterCardinal
 const EntityEvent EventMoveUpLeft{ sMovingUpLeft, {sIdle, sAction,  sMovingUp, sMovingDown, sMovingLeft, sMovingRight, sMovingUpLeft, sMovingUpRight, sMovingDownLeft, sMovingDownRight } };
 const EntityEvent EventMoveUpRight{ sMovingUpRight, {sIdle, sAction,  sMovingUp, sMovingDown, sMovingLeft, sMovingRight, sMovingUpLeft, sMovingUpRight, sMovingDownLeft, sMovingDownRight } };
 const EntityEvent EventMoveDownLeft{ sMovingDownLeft, {sIdle, sAction,  sMovingUp, sMovingDown, sMovingLeft, sMovingRight, sMovingUpLeft, sMovingUpRight, sMovingDownLeft, sMovingDownRight } };
 const EntityEvent EventMoveDownRight{ sMovingDownRight, {sIdle, sAction,  sMovingUp, sMovingDown, sMovingLeft, sMovingRight, sMovingUpLeft, sMovingUpRight, sMovingDownLeft, sMovingDownRight } };
 // Move Idle
 const EntityEvent EventIdle{ sIdle, {sAction, sMovingUp, sMovingDown, sMovingLeft, sMovingRight, sMovingUpLeft, sMovingUpRight, sMovingDownLeft, sMovingDownRight } };
 // Actions
 const EntityEvent EventActionIdle{ sIdle, {sActionOne, sActionTwo, sActionThree, sActionFour} };
 const EntityEvent EventActionOne{ sActionOne, {sIdle, sActionOne, sActionTwo, sActionThree, sActionFour } };
 const EntityEvent EventActionTwo{ sActionTwo, {sIdle, sActionOne, sActionTwo, sActionThree, sActionFour } };
 const EntityEvent EventActionThree{ sActionThree, {sIdle, sActionOne, sActionTwo, sActionThree, sActionFour } };
 const EntityEvent EventActionFour{ sActionFour, {sIdle, sActionOne, sActionTwo, sActionThree, sActionFour } };


 