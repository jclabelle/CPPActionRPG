#include "AI_CommandManager.h"
#include "GeometryConstants.h"
#include "Collisions2D.h"
#include <optional>


AI_CommandManager::AI_CommandManager(GameplayManager& arg_Gameplay)
	:m_Gameplay{&arg_Gameplay}
{
	init_Perception();
}

std::tuple<EntityEvent, EntityEvent> AI_CommandManager::run_FrameLogic(const Vector2D& arg_EntityWorldLoc, const FacingDirections& arg_SpriteFacing, bool arg_isActiveActionCompleted)
{
	EntityEvent tmp_MovementState{EventIdle};


	if (m_Perception)
	{

		auto ret_Perceived_Entities = m_Perception->run_FrameLogic(arg_EntityWorldLoc, arg_SpriteFacing);

		// Deal with an ongoing Collision Event against another Entity
		if (m_CurrentCollidingEntity_Collision != nullptr)
		{
			if (m_CollisionWithEntity == true)	// Register the direction at the time of the collision event
			{
				m_MovementAtCollisionEvent_EntityCollision = m_LastFrameMovementEvent.EventState;
				m_CollisionWithEntity = false;
				m_ClearanceBuffer_EntityCollision = const_CollisionClearance_Entity;
			}

			if (m_MovementAtCollisionEvent_EntityCollision == sMovingRight || m_MovementAtCollisionEvent_EntityCollision == sMovingLeft)	// If we were moving right or left when we collided  //m_Physics was m_WorldLocation.x = Shift_Left or Shift_Right;
			{
				if (m_Perception->v_OwnerWorldLocation().y > m_CurrentCollidingEntity_Collision->location.y + (m_CurrentCollidingEntity_Collision->size.y / 2))	// If our halfway point is below the halfway point of the colliding object
				{
					if (m_Perception->v_OwnerCollisions().location.y > m_CurrentCollidingEntity_Collision->location.y + m_CurrentCollidingEntity_Collision->size.y) // If we have moved down sufficiently to clear the colliding object
					{
						if (m_ClearanceBuffer_EntityCollision <= 0)
						{
							m_CurrentCollidingEntity_Collision = nullptr;
							//m_Path.clear();
							//m_PathProgress = 0;
						}
						else
						{
							--m_ClearanceBuffer_EntityCollision;
						}
					}

					tmp_MovementState = EventMoveDown;
					return{ tmp_MovementState, EventActionIdle };

				}
				else // If the halfway point is above the colliding object's halfway point
				{
					if (m_Perception->v_OwnerCollisions().location.y + m_Perception->v_OwnerCollisions().size.y < m_CurrentCollidingEntity_Collision->location.y) // If we have moved up sufficiently to clear the colliding object
					{
						if (m_ClearanceBuffer_EntityCollision <= 0)
						{
							m_CurrentCollidingEntity_Collision = nullptr;
							//m_Path.clear();
							//m_PathProgress = 0;
						}
						else
						{
							--m_ClearanceBuffer_EntityCollision;
						}
					}

					tmp_MovementState = EventMoveUp;
					return{ tmp_MovementState, EventActionIdle };

				}
			}

			if (m_MovementAtCollisionEvent_EntityCollision == sMovingDown || m_MovementAtCollisionEvent_EntityCollision == sMovingUp)	//m_WorldLocation.y = Shift_Up or Shift_Down;
			{
				if (m_Perception->v_OwnerWorldLocation().x > m_CurrentCollidingEntity_Collision->location.x + (m_CurrentCollidingEntity_Collision->size.x / 2))	// If our halfway point is below the halfway point of the colliding object
				{
					if (m_Perception->v_OwnerCollisions().location.x > m_CurrentCollidingEntity_Collision->location.x + m_CurrentCollidingEntity_Collision->size.x) // If we have moved down sufficiently to clear the colliding object
					{
						if (m_ClearanceBuffer_EntityCollision <= 0)
						{
							m_CurrentCollidingEntity_Collision = nullptr;
							//m_Path.clear();
							//m_PathProgress = 0;
						}
						else
						{
							--m_ClearanceBuffer_EntityCollision;
						}
					}

					tmp_MovementState = EventMoveRight;
					return{ tmp_MovementState, EventActionIdle };

				}
				else
				{
					if (m_Perception->v_OwnerCollisions().location.x + m_Perception->v_OwnerCollisions().size.x < m_CurrentCollidingEntity_Collision->location.x) // If we have moved up sufficiently to clear the colliding object
					{
						if (m_ClearanceBuffer_EntityCollision <= 0)
						{
							m_CurrentCollidingEntity_Collision = nullptr;
							//m_Path.clear();
							//m_PathProgress = 0;
						}
						else
						{
							--m_ClearanceBuffer_EntityCollision;
						}
					}

					tmp_MovementState = EventMoveLeft;
					return{ tmp_MovementState, EventActionIdle };

				}
			}
		}


		m_CurrentCollidingEntity_AI.clear();
			

		if (arg_isActiveActionCompleted == true)
		{
			if (ret_Perceived_Entities.size() > 0)
			{
				auto tmp_opt_NearestEnemy = find_NearestEnemy(ret_Perceived_Entities);


				if (tmp_opt_NearestEnemy)
				{
					const PerceptionData& tmp_NearestEnemy = tmp_opt_NearestEnemy.value();
						

					// If the target is not reachable walking in a straight line && the AI has no Path to follow
					if (tmp_NearestEnemy.m_Gameplay.m_Player_Current_Navmesh_Tile->m_ID != m_Current_Navmesh_Tile->m_ID && tmp_Path.size() == 0)
					{

						tmp_Path = request_Navigation_Path(m_Perception.value().v_OwnerWorldLocation(), tmp_NearestEnemy.m_WorldLocation, *m_Current_Navmesh_Tile, *tmp_NearestEnemy.m_Gameplay.m_Player_Current_Navmesh_Tile);
						auto ret_Distance_NextNode_Owner = make_RayTrace(tmp_Path[m_PathProgress].m_Location, m_Perception.value().v_OwnerWorldLocation());
						tmp_MovementState = find_MovementState_from_NormalRayTrace(ret_Distance_NextNode_Owner);
						m_debug_LineToNearestEnemy.second = tmp_Path[m_PathProgress].m_Location;
						m_debug_LineToNearestEnemy.first = m_Perception.value().v_OwnerWorldLocation();
						return{ tmp_MovementState, EventActionIdle };
						//std::cout << "AI_CommandManager::run_FrameLogic, UNREACHABLE ENEMY\n";
					}
					// If the target is not reachable && the AI has a path &&  we have not reached the last node && we have reached the next node
					else if (tmp_NearestEnemy.m_Gameplay.m_Player_Current_Navmesh_Tile->m_ID != m_Current_Navmesh_Tile->m_ID && tmp_Path.size() > 0 && m_PathProgress < tmp_Path.size() && make_RayTrace(tmp_Path[m_PathProgress].m_Location, m_Perception.value().v_OwnerWorldLocation()).LengthSquared() <= Minimum_Distance_To_AStarNode)
					{
						*&m_Current_Navmesh_Tile == *&tmp_Path[m_PathProgress].m_First_Tile_Ptr ? m_Current_Navmesh_Tile = *&tmp_Path[m_PathProgress].m_Second_Tile_Ptr : m_Current_Navmesh_Tile = *&tmp_Path[m_PathProgress].m_First_Tile_Ptr;

						++m_PathProgress;
					}
					// If the target is not reachable && the AI has a path && we have not reached the last node && we have not reached the next node
					else if (tmp_NearestEnemy.m_Gameplay.m_Player_Current_Navmesh_Tile->m_ID != m_Current_Navmesh_Tile->m_ID && tmp_Path.size() > 0 && m_PathProgress < tmp_Path.size() && make_RayTrace(tmp_Path[m_PathProgress].m_Location, m_Perception.value().v_OwnerWorldLocation()).LengthSquared() > Minimum_Distance_To_AStarNode)
					{
						// Move towards the next Node
						auto ret_Distance_NextNode_Owner = make_RayTrace(tmp_Path[m_PathProgress].m_Location, m_Perception.value().v_OwnerWorldLocation());
						tmp_MovementState = find_MovementState_from_NormalRayTrace(ret_Distance_NextNode_Owner);
						m_debug_LineToNearestEnemy.second = tmp_NearestEnemy.m_WorldLocation;
						m_debug_LineToNearestEnemy.first = m_Perception.value().v_OwnerWorldLocation();
						return{ tmp_MovementState, EventActionIdle };
					}
					// If the target is not reachable && the AI has a path && the AI is headed towards the last Node
					else if (tmp_NearestEnemy.m_Gameplay.m_Player_Current_Navmesh_Tile->m_ID != m_Current_Navmesh_Tile->m_ID && tmp_Path.size() > 0 && m_PathProgress >= tmp_Path.size())
					{
						// If the AI has not reached the last Node, keep moving towards the last Node
						if (make_RayTrace(tmp_Path.back().m_Location, m_Perception.value().v_OwnerWorldLocation()).LengthSquared() > Minimum_Distance_To_AStarNode)
						{
							auto ret_Distance_NextNode_Owner = make_RayTrace(tmp_Path.back().m_Location, m_Perception.value().v_OwnerWorldLocation());
							tmp_MovementState = find_MovementState_from_NormalRayTrace(ret_Distance_NextNode_Owner);
							m_debug_LineToNearestEnemy.second = tmp_NearestEnemy.m_WorldLocation;
							m_debug_LineToNearestEnemy.first = m_Perception.value().v_OwnerWorldLocation();
							return{ tmp_MovementState, EventActionIdle };

						}
						// If we have reached the last node, reset the path and progress, then return an idle state
						else if (make_RayTrace(tmp_Path.back().m_Location, m_Perception.value().v_OwnerWorldLocation()).LengthSquared() <= Minimum_Distance_To_AStarNode)
						{
							m_Path.clear();
							tmp_Path.clear();
							m_PathProgress = 0;

							m_debug_LineToNearestEnemy.second = tmp_NearestEnemy.m_WorldLocation;
							m_debug_LineToNearestEnemy.first = m_Perception.value().v_OwnerWorldLocation();
							return{ tmp_MovementState, EventActionIdle };
						}
					}


					auto ret_Distance_Enemy_Owner = make_RayTrace(tmp_NearestEnemy.m_WorldLocation, m_Perception.value().v_OwnerWorldLocation());

					// If the enemy is Reachable, reset the Path and progress
					if (tmp_NearestEnemy.m_Gameplay.m_Player_Current_Navmesh_Tile->m_ID == m_Current_Navmesh_Tile->m_ID)
					{
						m_Path.clear();
						tmp_Path.clear();
						m_PathProgress = 0;
					}

					// If the enemy is reachable but farther away than the limits of Close Quarters
					if (tmp_NearestEnemy.m_Gameplay.m_Player_Current_Navmesh_Tile->m_ID == m_Current_Navmesh_Tile->m_ID && ret_Distance_Enemy_Owner.Length() > m_CloseQuartersMaxDistance)
					{
						tmp_MovementState = find_MovementState_from_NormalRayTrace(ret_Distance_Enemy_Owner);
						m_debug_LineToNearestEnemy.second = tmp_NearestEnemy.m_WorldLocation;
						m_debug_LineToNearestEnemy.first = m_Perception.value().v_OwnerWorldLocation();
						return{ tmp_MovementState, EventActionIdle };
					}
					else if (tmp_NearestEnemy.m_Gameplay.m_Player_Current_Navmesh_Tile->m_ID == m_Current_Navmesh_Tile->m_ID && ret_Distance_Enemy_Owner.Length() <= m_CloseQuartersMaxDistance)
					{
						//std::cout << "AI_CommandManager::run_FrameLogic, AI reached it's target\n";
					}

				}
			}
		}
	}

	return std::make_tuple(tmp_MovementState, EventActionIdle);
}

const bool AI_CommandManager::has_Perception() const
{
	return m_Perception ? true : false;
}

void AI_CommandManager::init_Perception()
{
	m_Perception = AI_PerceptionManager{};
}

AI_PerceptionManager& AI_CommandManager::Perception()
{
	return *m_Perception;
}

const std::pair<Vector2D, Vector2D>& AI_CommandManager::debug_LineToNearestEnemy()
{
	return m_debug_LineToNearestEnemy;
}

const std::vector<Vector2D>& AI_CommandManager::debug_CurrentPath()
{
	return m_debug_CurrentPath;
}

void AI_CommandManager::init_Pathfinding(AStarGraph& arg_pathing)
{
	m_Pathfinding = &arg_pathing;
}

void AI_CommandManager::register_CollisionWithMap(bool arg_b, SquareCollision& arg_square_collision)
{
	m_CollisionWithMap = arg_b;
	m_CurrentCollidingObject = &arg_square_collision;
}

void AI_CommandManager::register_CollisionWithEntity(bool arg_b, SquareCollision& arg_square_collision, AI_CommandManager& arg_ai)
{
	m_CollisionWithEntity = arg_b;
	m_CurrentCollidingEntity_Collision = &arg_square_collision;
	m_CurrentCollidingEntity_AI.push_back(&arg_ai);
}

void AI_CommandManager::save_LastFrameMovementEvent(const EntityEvent& arg_move_event)
{
	m_LastFrameMovementEvent = arg_move_event;
}

void AI_CommandManager::request_StopMoving(bool b)
{
	m_StopMovingRequest = b;
}

void AI_CommandManager::set_Current_Navmesh_Tile(Navmesh_Tile& arg_navmesh_tile)
{
	m_Current_Navmesh_Tile = &arg_navmesh_tile;
}

Navmesh_Tile* AI_CommandManager::Current_Navmesh_Tile()
{
	return m_Current_Navmesh_Tile;
}

std::optional<PerceptionData> AI_CommandManager::find_NearestEnemy(const std::map<uint64_t, PerceptionData>& arg_PerceivedPop)
{
	uint64_t tmp_Nearest_ID;
	double tmp_Closest_Distance = std::numeric_limits<double>::max();
	for (const auto& for_Entity : arg_PerceivedPop)
	{
		// Only consider the player. TODO: Change this when faction system is done.
		if (for_Entity.second.m_Faction == EntityFaction::PLAYER)
		{
			auto ret_RayVector2D = make_RayTrace(for_Entity.second.m_WorldLocation, m_Perception.value().v_OwnerWorldLocation());
			if (ret_RayVector2D.LengthSquared() < tmp_Closest_Distance)// && std::count(, for_Entity.second.m_Faction.end(), ) TODO: add check for aggressivess
			{
				tmp_Closest_Distance = ret_RayVector2D.LengthSquared();
				tmp_Nearest_ID = for_Entity.first;
			}
		}
	}

	// If we found a target
	if(tmp_Closest_Distance < std::numeric_limits<double>::max())
	{
	auto tmp_Iterator_Nearest_Enemy = arg_PerceivedPop.find(tmp_Nearest_ID);
	return tmp_Iterator_Nearest_Enemy._Ptr->_Myval.second;
	}

	// If we did not
	return std::nullopt;
}

const EntityEvent& find_MovementState_from_NormalRayTrace(const Vector2D& arg_normal_ray_trace)
{
	auto tmp_RayTrace_Degrees = arg_normal_ray_trace.Normalized().Degrees();

	if (Degrees_Up >= tmp_RayTrace_Degrees && tmp_RayTrace_Degrees >= Midpoint_Degrees_Up_UpRight)
		return EventMoveUp;
	else if (Midpoint_Degrees_Up_UpRight >= tmp_RayTrace_Degrees && tmp_RayTrace_Degrees >= Midpoint_Degrees_UpRight_Right)
		return EventMoveUpRight;
	else if (Midpoint_Degrees_UpRight_Right >= tmp_RayTrace_Degrees && tmp_RayTrace_Degrees >= Midpoint_Degrees_Right_DownRight)
		return EventMoveRight;
	else if (Midpoint_Degrees_Right_DownRight >= tmp_RayTrace_Degrees && tmp_RayTrace_Degrees >= Midpoint_Degrees_DownRight_Down)
		return EventMoveDownRight;
	else if (Midpoint_Degrees_DownRight_Down >= tmp_RayTrace_Degrees && tmp_RayTrace_Degrees >= Midpoint_Degrees_Down_DownLeft)
		return EventMoveDown;
	else if (Midpoint_Degrees_Down_DownLeft >= tmp_RayTrace_Degrees && tmp_RayTrace_Degrees >= Midpoint_Degrees_DownLeft_Left)
		return EventMoveDownLeft;
	else if (Midpoint_Degrees_DownLeft_Left >= tmp_RayTrace_Degrees && tmp_RayTrace_Degrees >= Midpoint_Degrees_Left_UpLeft)
		return EventMoveLeft;
	else if (Midpoint_Degrees_Left_UpLeft >= tmp_RayTrace_Degrees && tmp_RayTrace_Degrees >= Midpoint_Degrees_UpLeft_Up)
		return EventMoveUpLeft;
	else if (Midpoint_Degrees_UpLeft_Up >= tmp_RayTrace_Degrees && tmp_RayTrace_Degrees >= Degrees_Up_ForceNegative)
		return EventMoveUp;
	else
		return EventMoveUp;
}
