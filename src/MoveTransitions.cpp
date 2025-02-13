//Project
#include "MoveTransitions.h"
#include "eFacingDirections.h"
#include "eMovementType.h"
#include "EntityStates.h"
//STL
#include <tuple>

// Movement Transition Functions
constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> f_moveUp(const MovementType& arg_MoveType_X, const MovementType& arg_MoveType_Y)
{
	MovementType MoveType_X;
	MovementType MoveType_Y;

	if (arg_MoveType_X == MovementType::ACCELERATION_LEFT)
	{
		MoveType_X = MovementType::DECELERATION_LEFT;
	}
	else if (arg_MoveType_X == MovementType::ACCELERATION_RIGHT)
	{
		MoveType_X = MovementType::DECELERATION_RIGHT;
	}
	else
	{
		MoveType_X = arg_MoveType_X;
	}

	FacingDirections SpriteFacing = FacingDirections::UP;
	StateType ActionState = StateType::WALK;
	if (arg_MoveType_Y != MovementType::ACCELERATION_UP)
	{
		MoveType_Y = MovementType::ACCELERATION_UP;
	}
	else
	{
		MoveType_Y = arg_MoveType_Y;
	}
#ifdef DEBUG_DRAW_CONTROLS
	std::cout << "UP" << '\n';
#endif
	return std::make_tuple(MoveType_X, MoveType_Y, SpriteFacing, ActionState);
}

constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> f_moveDown(const MovementType& arg_MoveType_X
	, const MovementType& arg_MoveType_Y)
{
	MovementType MoveType_X;
	MovementType MoveType_Y;
	if (arg_MoveType_X == MovementType::ACCELERATION_LEFT)
	{
		MoveType_X = MovementType::DECELERATION_LEFT;
	}
	else if (arg_MoveType_X == MovementType::ACCELERATION_RIGHT)
	{
		MoveType_X = MovementType::DECELERATION_RIGHT;
	}
	else
	{
		MoveType_X = arg_MoveType_X;
	}

	FacingDirections SpriteFacing = FacingDirections::DOWN;
	StateType ActionState = StateType::WALK;
	if (arg_MoveType_Y != MovementType::ACCELERATION_DOWN)
	{
		MoveType_Y = MovementType::ACCELERATION_DOWN;
	}
	else
	{
		MoveType_Y = arg_MoveType_Y;
	}
#ifdef DEBUG_DRAW_CONTROLS
	std::cout << "DOWN" << '\n';
#endif
	return std::make_tuple(MoveType_X, MoveType_Y, SpriteFacing, ActionState);

}

constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> f_moveLeft(const MovementType& arg_MoveType_X
	, const MovementType& arg_MoveType_Y)
{
	MovementType MoveType_X;
	MovementType MoveType_Y;
	if (arg_MoveType_Y == MovementType::ACCELERATION_UP)
	{
		MoveType_Y = MovementType::DECELERATION_UP;
	}
	else if (arg_MoveType_Y == MovementType::ACCELERATION_DOWN)
	{
		MoveType_Y = MovementType::DECELERATION_DOWN;
	}
	else
	{
		MoveType_Y = arg_MoveType_Y;
	}

	FacingDirections SpriteFacing = FacingDirections::LEFT;
	StateType ActionState = StateType::WALK;
	if (arg_MoveType_X != MovementType::ACCELERATION_LEFT)
	{
		MoveType_X = MovementType::ACCELERATION_LEFT;
	}
	else
	{
		MoveType_X = arg_MoveType_X;
	}
#ifdef DEBUG_DRAW_CONTROLS
	std::cout << "LEFT" << '\n';
#endif
	return std::make_tuple(MoveType_X, MoveType_Y, SpriteFacing, ActionState);
}

constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> f_moveRight(const MovementType& arg_MoveType_X
	, const MovementType& arg_MoveType_Y)
{

	MovementType MoveType_Y;
	MovementType MoveType_X;
	if (arg_MoveType_Y == MovementType::ACCELERATION_UP)
	{
		MoveType_Y = MovementType::DECELERATION_UP;
	}
	else if (arg_MoveType_Y == MovementType::ACCELERATION_DOWN)
	{
		MoveType_Y = MovementType::DECELERATION_DOWN;
	}
	else
	{
		MoveType_Y = arg_MoveType_Y;
	}

	FacingDirections SpriteFacing = FacingDirections::RIGHT;
	StateType ActionState = StateType::WALK;
	if (arg_MoveType_X != MovementType::ACCELERATION_RIGHT)
	{
		MoveType_X = MovementType::ACCELERATION_RIGHT;
	}
	else
	{
		MoveType_X = arg_MoveType_X;
	}
#ifdef DEBUG_DRAW_CONTROLS
	std::cout << "RIGHT" << '\n';
#endif
	return std::make_tuple(MoveType_X, MoveType_Y, SpriteFacing, ActionState);
}

constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> f_moveUpLeft(const MovementType& arg_MoveType_X
	, const MovementType& arg_MoveType_Y
	, const FacingDirections& arg_SpriteFacing)
{
	MovementType MoveType_X;
	MovementType MoveType_Y;
	FacingDirections SpriteFacing;

	if (arg_SpriteFacing != FacingDirections::UP && arg_SpriteFacing != FacingDirections::LEFT)
	{
		SpriteFacing = FacingDirections::LEFT;
	}
	else
	{
		SpriteFacing = arg_SpriteFacing;
	}

	StateType ActionState = StateType::WALK;

	if (arg_MoveType_Y != MovementType::ACCELERATION_UP)
	{
		MoveType_Y = MovementType::ACCELERATION_UP;
	}
	else
	{
		MoveType_Y = arg_MoveType_Y;
	}

	if (arg_MoveType_X != MovementType::ACCELERATION_LEFT)
	{
		MoveType_X = MovementType::ACCELERATION_LEFT;
	}
	else
	{
		MoveType_X = arg_MoveType_X;
	}
#ifdef DEBUG_DRAW_CONTROLS
	std::cout << "UPLEFT" << '\n';
#endif
	return std::make_tuple(MoveType_X, MoveType_Y, SpriteFacing, ActionState);

}

constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> f_moveUpRight(const MovementType& arg_MoveType_X
	, const MovementType& arg_MoveType_Y
	, const FacingDirections& arg_SpriteFacing)
{
	MovementType MoveType_X;
	MovementType MoveType_Y;
	FacingDirections SpriteFacing;

	if (arg_SpriteFacing != FacingDirections::UP && arg_SpriteFacing != FacingDirections::RIGHT)
	{
		SpriteFacing = FacingDirections::RIGHT;
	}
	else
	{
		SpriteFacing = arg_SpriteFacing;
	}

	StateType ActionState = StateType::WALK;

	if (arg_MoveType_Y != MovementType::ACCELERATION_UP)
	{
		MoveType_Y = MovementType::ACCELERATION_UP;
	}
	else
	{
		MoveType_Y = arg_MoveType_Y;
	}

	if (arg_MoveType_X != MovementType::ACCELERATION_RIGHT)
	{
		MoveType_X = MovementType::ACCELERATION_RIGHT;
	}
	else
	{
		MoveType_X = arg_MoveType_X;
	}

#ifdef DEBUG_DRAW_CONTROLS
	std::cout << "UPRIGHT" << '\n';
#endif
	return std::make_tuple(MoveType_X, MoveType_Y, SpriteFacing, ActionState);

}

constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> f_moveDownLeft(const MovementType& arg_MoveType_X
	, const MovementType& arg_MoveType_Y
	, const FacingDirections& arg_SpriteFacing)
{
	MovementType MoveType_X;
	MovementType MoveType_Y;
	FacingDirections SpriteFacing;

	if (arg_SpriteFacing != FacingDirections::DOWN && arg_SpriteFacing != FacingDirections::LEFT)
	{
		SpriteFacing = FacingDirections::LEFT;
	}
	else
	{
		SpriteFacing = arg_SpriteFacing;
	}

	StateType ActionState = StateType::WALK;

	if (arg_MoveType_Y != MovementType::ACCELERATION_DOWN)
	{
		MoveType_Y = MovementType::ACCELERATION_DOWN;
	}
	else
	{
		MoveType_Y = arg_MoveType_Y;
	}

	if (arg_MoveType_X != MovementType::ACCELERATION_LEFT)
	{
		MoveType_X = MovementType::ACCELERATION_LEFT;
	}
	else
	{
		MoveType_X = arg_MoveType_X;
	}

#ifdef DEBUG_DRAW_CONTROLS
	std::cout << "DOWNLEFT" << '\n';
#endif
	return std::make_tuple(MoveType_X, MoveType_Y, SpriteFacing, ActionState);

}

constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> f_moveDownRight(const MovementType& arg_MoveType_X
	, const MovementType& arg_MoveType_Y
	, const FacingDirections& arg_SpriteFacing)
{
	MovementType MoveType_X;
	MovementType MoveType_Y;
	FacingDirections SpriteFacing;

	if (arg_SpriteFacing != FacingDirections::DOWN && arg_SpriteFacing != FacingDirections::RIGHT)
	{
		SpriteFacing = FacingDirections::RIGHT;
	}
	else
	{
		SpriteFacing = arg_SpriteFacing;
	}
	StateType ActionState = StateType::WALK;

	if (arg_MoveType_Y != MovementType::ACCELERATION_DOWN)
	{
		MoveType_Y = MovementType::ACCELERATION_DOWN;
	}
	else
	{
		MoveType_Y = arg_MoveType_Y;
	}

	if (arg_MoveType_X != MovementType::ACCELERATION_RIGHT)
	{
		MoveType_X = MovementType::ACCELERATION_RIGHT;
	}
	else
	{
		MoveType_X = arg_MoveType_X;
	}
#ifdef DEBUG_DRAW_CONTROLS
	std::cout << "UPLEFT" << '\n';
#endif
	return std::make_tuple(MoveType_X, MoveType_Y, SpriteFacing, ActionState);

}

constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> f_moveIdle(const FacingDirections& arg_SpriteFacing)
{
	StateType ActionState = StateType::IDLE;
	MovementType MoveType_X = MovementType::DECELERATION_LEFT;
	MovementType MoveType_Y = MovementType::DECELERATION_RIGHT;
	FacingDirections SpriteFacing = arg_SpriteFacing;

	return std::make_tuple(MoveType_X, MoveType_Y, SpriteFacing, ActionState);

}