#pragma once
/* Project */
#include "GeometryStructures.h"
#include "eCollisionType.h"
#include <utility>


bool collision_axis_align(Cell first, Cell second);
bool checkCollisionAxisAligned(const Vector2D& first_location, const Vector2D& first_size, const Vector2D& second_location, const Vector2D& second_size);
Vector2D checkCollisionDuringMovement(Vector2D first_origin, Vector2D first_location, Vector2D first_size, Vector2D second_location, Vector2D second_size);

struct SquareCollision
{
	SquareCollision();
	SquareCollision(CollisionType new_type);
	SquareCollision(CollisionType new_type, Vector2D new_location, Vector2D new_size);
	Vector2D size{ 0,0 };
	Vector2D location{ 0,0 };	// == top left corner
	CollisionType type{ CollisionType::PHYSICAL };
	void PrintToCmdLine() const;
	const CollisionType& viewType() const;
	SquareCollision to_World(const Vector2D& arg_WorldLocation);

	Vector2D view_CenterLocation();
	Point2D calc_TopLeftCorner() const;
	Point2D calc_TopRightCorner() const;
	Point2D calc_BottomLeftCorner() const;
	Point2D calc_BottomRightCorner() const;
	Vector2D calc_TopSide() const;
	Vector2D calc_BottomSide() const;
	Vector2D calc_LeftSide() const;
	Vector2D calc_RightSide() const;

	//bool is_Colliding(const SquareCollision& Collider) const;
};


bool detect_Collisions(const SquareCollision& first, const SquareCollision& second);

bool detect_LineBoxCollision(const Vector2D& arg_box_top_left, const Vector2D& arg_box_bottom_right, const Vector2D& arg_line_start, const Vector2D& arg_line_end);

std::pair<double, double> ratio_Line_Overlap(double arg_axis_one_min, double arg_axis_one_max, double arg_axis_two_min, double arg_axis_two_max);
