/* Project */
#include "GeometryStructures.h"
#include "Collisions2D.h"
/* STL */
#include <iostream>
#include <utility>
/* Libraries */
#include "fmt/core.h"



bool collision_axis_align(Cell first, Cell second)
{
	if (first.x > (second.x + second.w))
		return false;

	if ((first.x + first.w) < second.x)
		return false;

	if (first.y > (second.y + second.h))
		return false;

	if ((first.y + first.h) < second.y)
		return false;

	return true;
}

bool detect_Collisions(const SquareCollision& first, const SquareCollision& second)
{
	if (first.location.x > (second.location.x + second.size.x))
		return false;

	if ((first.location.x + first.size.x) < second.location.x)
		return false;

	if (first.location.y > (second.location.y + second.size.y))
		return false;

	if ((first.location.y + first.size.y) < second.location.y)
		return false;

	return true;
}

bool checkCollisionAxisAligned(const Vector2D& first_location, const Vector2D& first_size, const Vector2D& second_location, const Vector2D& second_size)
{
	if (first_location.x > (second_location.x + second_size.x))
		return false;

	if ((first_location.x + first_size.x) < second_location.x)
		return false;

	if (first_location.y > (second_location.y + second_size.y))
		return false;

	if ((first_location.y + first_size.y) < second_location.y)
		return false;

	return true;
}

Vector2D checkCollisionDuringMovement(Vector2D first_origin, Vector2D first_destination, Vector2D first_size, Vector2D second_location, Vector2D second_size)
{
	// TODO: Redo this with real Vector math. Not robust.
	Vector2D FinalLocation = first_origin;
	double margin = 0.001; // force a small separation between objects

	if (first_destination.x < second_location.x + second_size.x && first_destination.x + first_size.x > second_location.x
		&& first_destination.y < second_location.y + second_size.y && first_destination.y + first_size.y > second_location.y)
	{
		/* Moving right: right side of first at origin is left of Left side of second: */
		if (first_origin.x + first_size.x < second_location.x)
			/* find the x location of first by substracting its size from the x location of second */
			FinalLocation.x = second_location.x - first_size.x - margin;

		/* Moving left: left side of first at origin is right of right side of second: */
		if (first_origin.x > second_location.x + second_size.x)
			/* the x location of first is the right side of second */
			FinalLocation.x = second_location.x + first_size.x + margin;

		/* Moving down: bottom side of first at origin is above top side of second: */
		if (first_origin.y + first_size.y < second_location.y)
			/* find the y location of first by substracting its size from the y location of second */
			FinalLocation.y = second_location.y - first_size.y - margin;

		/* Moving up: top side of first at origin is below the bottom side of second: */
		if (first_origin.y > second_location.y + second_size.y)
			/* the y location of first is the bottom side of second */
			FinalLocation.y = second_location.y + first_size.y + margin;
	}
	else
	{
		// No collision: return the destination
		return first_destination;
	}
	return FinalLocation;
}

SquareCollision::SquareCollision()
{
}

SquareCollision::SquareCollision(CollisionType new_collision_type)
	:type{new_collision_type}
{}

SquareCollision::SquareCollision(CollisionType new_type, Vector2D new_location, Vector2D new_size)
	:type{new_type}, location{new_location}, size{new_size}
{}



void SquareCollision::PrintToCmdLine() const
{
	std::cout << fmt::format("Collision detected: [x{}, y{}], [w{}, h{}] \n", location.x, location.y, size.x, size.y);

}

const CollisionType& SquareCollision::viewType() const
{
	return type;
}

SquareCollision SquareCollision::to_World(const Vector2D& arg_WorldLocation)
{
	return SquareCollision{type, {convert_Vec2LocalToWorld(location, arg_WorldLocation)}, {size} };
}

Vector2D SquareCollision::view_CenterLocation()
{
	return Vector2D(location.x + size.x/2, location.y + size.y/2 );
}

Point2D SquareCollision::calc_TopLeftCorner() const
{
	return Point2D(location.x, location.y);
}

Point2D SquareCollision::calc_TopRightCorner() const
{
	return Point2D(location.x + size.x, location.y);
}

Point2D SquareCollision::calc_BottomLeftCorner() const
{
	return Point2D(location.x, location.y + size.y);
}

Point2D SquareCollision::calc_BottomRightCorner() const
{
	return Point2D(location.x + size.x, location.y + size.y);
}

Vector2D SquareCollision::calc_TopSide() const
{
	return Vector2D(calc_TopRightCorner() - calc_TopLeftCorner());
}

Vector2D SquareCollision::calc_BottomSide() const
{
	return Vector2D(calc_BottomRightCorner() - calc_BottomLeftCorner());
}

Vector2D SquareCollision::calc_LeftSide() const
{
	return Vector2D(calc_BottomLeftCorner() - calc_TopLeftCorner());
}

Vector2D SquareCollision::calc_RightSide() const
{
	return Vector2D(calc_BottomRightCorner() - calc_TopRightCorner());
}

// Returns the start and end points where line_a overlaps line_b on their axis,
// expressed as start/end ratios of b.
std::pair<double, double> ratio_Line_Overlap(double arg_line_a_min, double arg_line_a_max, double arg_line_b_min, double arg_line_b_max) 
{
	// Find at what fraction of line b the overlap with line a starts
	double tmp_line_b_fraction_of_start = (arg_line_a_min - arg_line_b_min) / (arg_line_b_max - arg_line_b_min);

	// Find at what fraction of line b the overlap with line a ends
	double tmp_line_b_fraction_of_end = (arg_line_a_max - arg_line_b_min) / (arg_line_b_max - arg_line_b_min);

	// verify that start is the lowest value and end the largest. If not, swap them.
	if (tmp_line_b_fraction_of_end < tmp_line_b_fraction_of_start)
		std::swap(tmp_line_b_fraction_of_end, tmp_line_b_fraction_of_start);

	return std::pair{ tmp_line_b_fraction_of_start, tmp_line_b_fraction_of_end };
}

bool detect_LineBoxCollision(const Vector2D& arg_box_top_left, const Vector2D& arg_box_bottom_right, const Vector2D& arg_line_start, const Vector2D& arg_line_end) 
{
	auto [ret_Overlap_Start_X, ret_Overlap_End_X] = ratio_Line_Overlap(arg_box_top_left.x, arg_box_bottom_right.x, arg_line_start.x, arg_line_end.x);
	auto [ret_Overlap_Start_Y, ret_Overlap_End_Y] = ratio_Line_Overlap(arg_box_top_left.y, arg_box_bottom_right.y, arg_line_start.y, arg_line_end.y);

	// There is an Intersect if on both axes the overlap starts before the end of the box and ends after the start of the box
	if (ret_Overlap_Start_X < 1 && ret_Overlap_End_X >0 && ret_Overlap_Start_Y < 1 && ret_Overlap_End_Y >0)
		return true;
	else
		return false;
}
