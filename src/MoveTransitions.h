#pragma once
// Project
#include "EntityStates.h"
#include "eMovementType.h"
#include "eFacingDirections.h"
// STL
#include <functional>
#include <tuple>
#include <vector>
#include <utility>


// Movement functions
 
using MovementStateData = std::tuple<MovementType, MovementType, FacingDirections, StateType>;
constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> f_moveUp(const MovementType& arg_MoveType_X, const MovementType& arg_MoveType_Y);
constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> f_moveDown(const MovementType& arg_MoveType_X, const MovementType& arg_MoveType_Y);
constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> f_moveLeft(const MovementType& arg_MoveType_X, const MovementType& arg_MoveType_Y);
constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> f_moveRight(const MovementType& arg_MoveType_X, const MovementType& arg_MoveType_Y);
constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> f_moveUpLeft(const MovementType& arg_MoveType_X, const MovementType& arg_MoveType_Y, const FacingDirections& arg_SpriteFacing);
constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> f_moveUpRight(const MovementType& arg_MoveType_X, const MovementType& arg_MoveType_Y, const FacingDirections& arg_SpriteFacing);
constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> f_moveDownLeft(const MovementType& arg_MoveType_X, const MovementType& arg_MoveType_Y, const FacingDirections& arg_SpriteFacing);
constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> f_moveDownRight(const MovementType& arg_MoveType_X, const MovementType& arg_MoveType_Y, const FacingDirections& arg_SpriteFacing);
constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> f_moveIdle(const FacingDirections& arg_CurrentActionFacing);

using Move_Idle = std::function<std::tuple<MovementType, MovementType, FacingDirections, StateType>(const FacingDirections&)>;
using CardinalFacings = std::function<std::tuple<MovementType, MovementType, FacingDirections, StateType>(const MovementType&, const MovementType&)>;
using InterCardinalFacings = std::function<std::tuple<MovementType, MovementType, FacingDirections, StateType>(const MovementType&, const MovementType&, const FacingDirections&)>;
const std::pair<EntityState, Move_Idle> p_MoveIdle{ sIdle , f_moveIdle };
const std::pair<EntityState, CardinalFacings> p_MoveUp{ sMovingUp, f_moveUp };
const std::pair<EntityState, CardinalFacings> p_MoveDown{ sMovingDown, f_moveDown };
const std::pair<EntityState, CardinalFacings> p_MoveLeft{ sMovingLeft, f_moveLeft };
const std::pair<EntityState, CardinalFacings> p_MoveRight{ sMovingRight, f_moveRight };
const std::pair<EntityState, InterCardinalFacings> p_MoveUpLeft{ sMovingUpLeft, f_moveUpLeft };
const std::pair<EntityState, InterCardinalFacings> p_MoveUpRight{ sMovingUpRight, f_moveUpRight };
const std::pair<EntityState, InterCardinalFacings> p_MoveDownLeft{ sMovingDownLeft, f_moveDownLeft };
const std::pair<EntityState, InterCardinalFacings> p_MoveDownRight{sMovingDownRight , f_moveDownRight };
const std::vector<std::pair<EntityState, CardinalFacings>> CardinalTransitions{ p_MoveUp, p_MoveDown, p_MoveLeft, p_MoveRight };
const std::vector<std::pair<EntityState, InterCardinalFacings>> InterCardinalTransitions{ p_MoveUpLeft, p_MoveUpRight, p_MoveDownLeft, p_MoveDownRight };
