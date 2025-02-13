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
#include <variant>
#include <optional>


constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> ActionOne(const FacingDirections& arg_CurrentActionFacing);
constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> ActionTwo(const FacingDirections& arg_CurrentActionFacing);
constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> ActionThree(const FacingDirections& arg_CurrentActionFacing);
constexpr std::tuple<MovementType, MovementType, FacingDirections, StateType> ActionFour(const FacingDirections& arg_CurrentActionFacing);


using ActionSwitch = std::function<std::tuple<MovementType, MovementType, FacingDirections, StateType>(const FacingDirections&)>;
using ActionVariant = std::variant<std::tuple<EntityState, EntityEvent>, std::tuple<MovementType, MovementType, FacingDirections, StateType>>;
using PlanAction = std::optional<std::tuple<EntityState, FacingDirections>>;
const std::pair<EntityState, ActionSwitch> p_ActionOne{ sActionOne , ActionOne };
const std::pair<EntityState, ActionSwitch> p_ActionTwo{ sActionTwo , ActionTwo };
const std::pair<EntityState, ActionSwitch> p_ActionThree{ sActionThree , ActionThree };
const std::pair<EntityState, ActionSwitch> p_ActionFour{ sActionFour , ActionFour };
const std::vector<std::pair<EntityState, ActionSwitch>> ActionTransitions{ p_ActionOne, p_ActionTwo, p_ActionThree, p_ActionFour };


