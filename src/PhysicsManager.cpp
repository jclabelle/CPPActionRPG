/* Project */
#include "PhysicsManager.h"
#include "GeometryStructures.h"
#include "Collisions2D.h"
/* STL */
#include <cmath>
#include <optional>
#include <iostream>
#include <utility>

PhysicsManager::PhysicsManager()
{
}

PhysicsManager::PhysicsManager(CollisionType arg_CollisionType, Vector2D arg_WorldLocation, Vector2D arg_CollisionSize, Vector2D arg_MapLimits, double arg_Speed)
	: m_Collisions{ arg_CollisionType,Vector2D{ arg_WorldLocation.x - arg_CollisionSize.x / 2, arg_WorldLocation.y - arg_CollisionSize.y / 2}, arg_CollisionSize }
	, m_WorldLocation{ arg_WorldLocation }
	, m_AreaMapLimits{ arg_MapLimits }
	, m_Speed{arg_Speed}
{}

const Vector2D& PhysicsManager::v_WorldLocation() const
{
	return m_WorldLocation;
}

void PhysicsManager::refreshCollisions()
{
	m_Collisions.location = Vector2D{ m_WorldLocation.x - m_Collisions.size.x / 2, m_WorldLocation.y - m_Collisions.size.y / 2 };

}
const SquareCollision& PhysicsManager::v_Collisions() const
{
	return m_Collisions;
}

const Vector2D& PhysicsManager::v_Size() const
{
	return m_Collisions.size;
}

const CollisionType& PhysicsManager::v_CollisionsType() const
{
	return m_Collisions.type;
}

std::reference_wrapper<SquareCollision> PhysicsManager::get_CollisionsReference()
{
	
	 return std::ref(m_Collisions);
}

void PhysicsManager::set_AreaMapLimits(const Vector2D& arg_MapLimits)
{
	m_AreaMapLimits = arg_MapLimits;
}

void PhysicsManager::set_MoveType_X(const MovementType& arg_MoveType_X)
{
	m_MoveType_X = arg_MoveType_X;
}

void PhysicsManager::set_MoveType_Y(const MovementType& arg_MoveType_Y)
{
	m_MoveType_Y = arg_MoveType_Y;
}

void PhysicsManager::runFrameLogic(const MovementType& arg_MoveType_X, const MovementType& arg_MoveType_Y)
{
	m_MoveType_X = arg_MoveType_X;
	m_MoveType_Y = arg_MoveType_Y;
	refreshCollisions();
	auto FrameMovement = planMovement();
	move(FrameMovement);

}

void PhysicsManager::move(const Vector2D& arg_Movement)
{
		// Move the entity to the target destination
		m_WorldLocation = arg_Movement;

		// Check against area map limits. Upper left limit assumed to be 0,0
		// Adjust if the entity moves out of the provided limits
		if (arg_Movement.x < 0.f)
		{
			m_WorldLocation.x = 0.f;
		}

		if (arg_Movement.y  < m_Collisions.size.y * 3)
		{
			m_WorldLocation.y = m_Collisions.size.y *3;
		}

		if (arg_Movement.x > m_AreaMapLimits.x)
		{
			m_WorldLocation.x = m_AreaMapLimits.x;
		}

		if (arg_Movement.y  > m_AreaMapLimits.y)
		{
			m_WorldLocation.y = m_AreaMapLimits.y;
		}
}

void PhysicsManager::set_WorldLocation(const Vector2D& arg_WorldLocation)
{
	m_WorldLocation = arg_WorldLocation;
	refreshCollisions();
}

// Detect collisions and correct location if necessary 
bool PhysicsManager::is_Colliding(const SquareCollision & arg_Collider, const MovementType& arg_MoveType_X, const MovementType& arg_MoveType_Y)
{
	double SpaceBuffer = 0.00000000001;	// Buffer prevent rounding errors which can cause "teleportation"
	m_Collisions.location = Vector2D{ m_WorldLocation.x - m_Collisions.size.x / 2, m_WorldLocation.y - m_Collisions.size.y / 2 };

	if (m_Collisions.location.x < arg_Collider.location.x + arg_Collider.size.x
		&& m_Collisions.location.x + m_Collisions.size.x > arg_Collider.location.x
		&& m_Collisions.location.y < arg_Collider.location.y + arg_Collider.size.y
		&& m_Collisions.location.y + m_Collisions.size.y > arg_Collider.location.y)
	{
		if(arg_Collider.viewType() == CollisionType::PHYSICAL)
		{
			double Shift_Left = arg_Collider.location.x - m_Collisions.size.x / 2 - SpaceBuffer;
			double Shift_Up = arg_Collider.location.y - m_Collisions.size.y/2 - SpaceBuffer;
			double Shift_Right = arg_Collider.location.x + arg_Collider.size.x + m_Collisions.size.x / 2 + SpaceBuffer;
			double Shift_Down = arg_Collider.location.y + arg_Collider.size.y + m_Collisions.size.y / 2 + SpaceBuffer;

			if (m_FrameMove.Degrees() == 90)
			{
				m_WorldLocation.x = Shift_Left;
				refreshCollisions();
			}
			
			if (m_FrameMove.Degrees() == 0)
			{
				m_WorldLocation.y = Shift_Up;
				refreshCollisions();
			}

			if (m_FrameMove.Degrees() == -90)
			{
				m_WorldLocation.x = Shift_Right;
				refreshCollisions();
			}

			if (m_FrameMove.Degrees() == 180)
			{
				m_WorldLocation.y = Shift_Down;
				refreshCollisions();
			}

			

			/* Check distance of x/y of opposite corners*/
			if (arg_MoveType_X == MovementType::ACCELERATION_RIGHT && arg_MoveType_Y == MovementType::ACCELERATION_DOWN)
			{
				// Bottom/Right corner of this VS Top/Left corner of AnEntity 
				double AbsX = std::abs(arg_Collider.location.x - (m_Collisions.location.x + m_Collisions.size.x));
				double AbsY = std::abs(arg_Collider.location.y - (m_Collisions.location.y + m_Collisions.size.y));
				if (AbsX >= AbsY)
				{
					m_WorldLocation.y = Shift_Up;
					refreshCollisions();
				}
				else if (AbsX < AbsY)
				{
					m_WorldLocation.x = Shift_Left;
					refreshCollisions();
				}
			}
			if (arg_MoveType_X == MovementType::ACCELERATION_LEFT && arg_MoveType_Y == MovementType::ACCELERATION_DOWN)
			{
				/* Bottom/Left corner of this VS Top/Right corner of AnEntity */

				double AbsX = std::abs((arg_Collider.location.x + arg_Collider.size.x) - m_Collisions.location.x);
				double AbsY = std::abs(arg_Collider.location.y - (m_Collisions.location.y + m_Collisions.size.y));
				if (AbsX >= AbsY)
				{
					m_WorldLocation.y = Shift_Up;
					refreshCollisions();
				}
				else if (AbsX < AbsY)
				{
					m_WorldLocation.x = Shift_Right;
					refreshCollisions();
				}
			}
			if (arg_MoveType_X == MovementType::ACCELERATION_RIGHT && arg_MoveType_Y == MovementType::ACCELERATION_UP)
			{
				/* Top/Right corner of this VS Bottom/Left corner of AnEntity */

				double AbsX = std::abs(arg_Collider.location.x - (m_Collisions.location.x + m_Collisions.size.x));
				double AbsY = std::abs((arg_Collider.location.y + arg_Collider.size.y) - m_Collisions.location.y);
				if (AbsX >= AbsY)
				{
					m_WorldLocation.y = Shift_Down;
					refreshCollisions();
				}
				else if (AbsX < AbsY)
				{
					m_WorldLocation.x = Shift_Left;
					refreshCollisions();
				}
			}
			if (arg_MoveType_X == MovementType::ACCELERATION_LEFT && arg_MoveType_Y == MovementType::ACCELERATION_UP)
			{
				/* Top/Left corner of this VS Bottom/Right corner of AnEntity */

				double AbsX = std::abs((arg_Collider.location.x + arg_Collider.size.x) - m_Collisions.location.x);
				double AbsY = std::abs((arg_Collider.location.y + arg_Collider.size.y) - m_Collisions.location.y);
				if (AbsX >= AbsY)
				{
					m_WorldLocation.y = Shift_Down;
					refreshCollisions();
				}
				else if (AbsX < AbsY)
				{
					m_WorldLocation.x = Shift_Right;
					refreshCollisions();
				}
			}
		}
		return true;
	}
	return false;
}


Vector2D PhysicsManager::planMovement()
{
	Vector2D Movement{ 0, 0 };
	double Speed = m_Speed;
	Vector2D FrameDestination;
	Vector2D StartingDestination = m_WorldLocation;

	/* DECELERATION*/
	if (m_MoveType_Y == MovementType::DECELERATION_UP)
	{
		Movement.y = 0;
	}

	if (m_MoveType_Y == MovementType::DECELERATION_DOWN)
	{
		Movement.y = 0;
	}

	if (m_MoveType_X == MovementType::DECELERATION_LEFT)
	{
		Movement.x = 0;
	}

	if (m_MoveType_X == MovementType::DECELERATION_RIGHT)
	{
		Movement.x = 0;
	}

	/* ACCELERATION */

	//TODO :Reduce Speed if x and y if player is accelerating on both axis

	if (m_MoveType_Y == MovementType::ACCELERATION_UP)
	{
		Movement.y = Speed * -1;
	}

	if (m_MoveType_Y == MovementType::ACCELERATION_DOWN)
	{
		Movement.y = Speed;
	}

	if (m_MoveType_X == MovementType::ACCELERATION_LEFT)
	{
		Movement.x = Speed * -1;
	}

	if (m_MoveType_X == MovementType::ACCELERATION_RIGHT)
	{
		Movement.x = Speed;
	}

	FrameDestination = Movement + StartingDestination;
	m_FrameMove = Movement;
	return FrameDestination;
}
