#pragma once
#include "Collisions2D.h"
#include "GeometryStructures.h"
#include "eCollisionType.h"
#include "eMovementType.h"


struct PhysicsManager
{
	PhysicsManager();
	PhysicsManager(CollisionType arg_CollisionType, Vector2D arg_WorldLocation, Vector2D arg_CollisionSize, Vector2D arg_MapLimits, double arg_Speed = 2);

	const Vector2D& v_WorldLocation() const;
	const SquareCollision& v_Collisions() const;
	const Vector2D& v_Size() const;
	const CollisionType& v_CollisionsType() const; 
	std::reference_wrapper<SquareCollision> get_CollisionsReference();

	void set_AreaMapLimits(const Vector2D& arg_MapLimits);
	void set_MoveType_X(const MovementType& arg_MoveType_X);
	void set_MoveType_Y(const MovementType& arg_MoveType_Y);
	void set_WorldLocation(const Vector2D& arg_WorldLocation);
	bool is_Colliding(const SquareCollision& arg_Collider, const MovementType& arg_MoveType_X, const MovementType& arg_MoveType_Y);

	void runFrameLogic(const MovementType& arg_MoveType_X, const MovementType& arg_MoveType_Y);

	void move(const Vector2D& arg_Movement);

	void setFrameMove(Vector2D v) { m_FrameMove = v; }

	void set_Speed(double arg_Speed) { m_Speed = arg_Speed; }


	// Physics Data
	SquareCollision m_Collisions{ CollisionType::IGNORE, Vector2D{0,0}, Vector2D{0,0} };
	Vector2D m_WorldLocation{0,0};
	Vector2D m_AreaMapLimits{ 1024, 1024 };

	// Frame movement data, refreshed each time movement occurs
	MovementType m_MoveType_X{MovementType::MOVEMENT_MAX};
	MovementType m_MoveType_Y{ MovementType::MOVEMENT_MAX };

	// Destination of this frame and Movement speed
	Vector2D m_FrameMove{0,0};
	double m_Speed{ 2 };

	void refreshCollisions();
	Vector2D planMovement();


};