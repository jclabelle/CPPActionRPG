#include "GeometryStructures.h"
#include <cmath>
#include <numbers>

Vector2D::Vector2D()
{
}

Vector2D::Vector2D(double new_x, double new_y)
	:x{new_x}, y{new_y}
{}



/* Note: For our purpose, *Magnitude* and *Length* are synonyms */
double Vector2D::Length() const
{
	double Length = std::sqrt(x * x + y * y);
	return Length;
}

/* Cheaper to call this when comparing two vector lenghts with <=> 
or their individual equivalents */
double Vector2D::LengthSquared() const
{
	double LengthSquared = x * x + y * y;
	return LengthSquared;
}

/* Returns a normalized vector2d (unit length of 1) of the calling vector2d */
Vector2D Vector2D::Normalized() const
{
	if( Length() != 0)
	{ 
		Vector2D Normalized = (*this) / Length();
		return Normalized;
	}
	else
	{
		return { 0,0 };
	}
}

double Vector2D::Degrees() const
{
	return std::atan2(x, y) * 180 / std::numbers::pi;
}

Vector2D Vector2D::Rotated(double radians) const
{
	auto ca = std::acos(radians);
	auto sa = std::asin(radians);
	double X_rotated = (x * ca) - (y * sa);
	double Y_rotated = (x * sa) + (y * ca);
	
	return Vector2D{ X_rotated, Y_rotated };
}

Vector2D Vector2D::operator*(double scalar) const
{
	Vector2D ScaledVector2D{ x * scalar, y * scalar };
	return ScaledVector2D;
}

Vector2D Vector2D::operator/(double scalar) const
{
	Vector2D ScaledVector2D{ x / scalar, y / scalar };
	return ScaledVector2D;
}

Vector2D Vector2D::operator+(const Vector2D& rhs_vector2d) const
{
	return Vector2D(x + rhs_vector2d.x, y + rhs_vector2d.y);
}

Vector2D Vector2D::operator-(const Vector2D& rhs_vector2d) const
{
	return Vector2D(x - rhs_vector2d.x, y - rhs_vector2d.y);
}

Point2D::Point2D()
{}

Point2D::Point2D(double new_x, double new_y)
	:x{new_x}, y{new_y}
{}

Point2D Point2D::addVector(Vector2D vector_2d)
{
	Point2D NewPoint2D;
	NewPoint2D.x = x + vector_2d.x;
	NewPoint2D.y = y + vector_2d.y;
	return NewPoint2D;
}

Vector2D operator-(Point2D destination, Point2D origin)
{
	Vector2D NewVector2D;
	NewVector2D.x = destination.x - origin.x;
	NewVector2D.y = destination.y - origin.y;
	return NewVector2D;
}

Point2D addVectorToPoint(Point2D point_2d, Vector2D vector_2d)
{
	Point2D NewPoint2D;
	NewPoint2D.x = point_2d.x + vector_2d.x;
	NewPoint2D.y = point_2d.y + vector_2d.y;
	return NewPoint2D;
}

/// <summary>
/// Math/Physics definition: Lenght of Vector A * Lenght of Vector B * cosine of the angle between the two vectors.
/// Game: Product of the X components of the two vectors + the product of the Y components of the two vectors.
/// If two vectors are facing in the same direction, then the dot product is going to equal 1
/// </summary>
double DotProduct(const Vector2D& a, const Vector2D& b)
{
	return (a.x * b.x) + (a.y * b.y);
}

double DotProduct(const Vector3D& a, const Vector3D& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}
// Get a Local location relative to a Point of Origin by substracting the world location (arg_WorldLoc)
// of an Entity or other scene object from it's new point of Origin (arg_NewOrigin_WorldLoc).
Vector2D convert_Vec2WorldToLocal(const Vector2D& arg_WorldLoc, const Vector2D& arg_NewOrigin_WorldLoc)
{
	return arg_WorldLoc - arg_NewOrigin_WorldLoc;
}


// Get a World location by adding the local location (arg_World) of an Entity (or other scene object)
// to the World Location of it's Local Point of Origin(arg_World_CurrentOrigin).
Vector2D convert_Vec2LocalToWorld(const Vector2D& arg_LocalLoc, const Vector2D& arg_CurrentOrigin_WorldLoc)
{
	return arg_LocalLoc + arg_CurrentOrigin_WorldLoc;
}

Vector2D make_RayTrace(const Vector2D& arg_Destination, const Vector2D& arg_Origin)
{
	return Vector2D(arg_Destination.x - arg_Origin.x, arg_Destination.y - arg_Origin.y);
}

Vector2D make_RayTrace(const Point2D& arg_Destination, const Point2D& arg_Origin)
{
	return Vector2D(arg_Destination - arg_Origin);
}

Vector2D traceLine(const Vector2D& arg_Destination, const Vector2D& arg_Origin)
{
	return Vector2D(arg_Destination.x - arg_Origin.x, arg_Destination.y - arg_Origin.y);
}

Cell::Cell(int nx, int ny, int nw, int nh)
	:x{ nx }, y{ ny }, w{ nw }, h{ nh }
{}

Cell::Cell()
{}


