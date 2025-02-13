#pragma once


/* Potential types:
	-Velocity
	-Distance
	-View (Unit Vector/Normalized)
	-
	*/
struct Vector2D
{
	/* Constructors */
	Vector2D();
	Vector2D(double new_x, double new_y);
	
	/* Member functions */
	double Length() const;
	double LengthSquared() const;
	Vector2D Normalized() const;
	double Degrees() const;
	Vector2D Rotated(double angle) const;

	/* Vector values */
	double x{ 0.f };
	double y{ 0.f };

	/* Operator overloads */
	Vector2D operator*(double scalar) const;
	Vector2D operator/(double scalar) const;
	Vector2D operator+(const Vector2D& rhs_vector2d) const;
	Vector2D operator-(const Vector2D& rhs_vector2d) const;

};

const Vector2D ZeroV2D{ 0,0 };


struct Vector2Di
{
	int x{ 0 };
	int y{ 0 };
};

struct Vector3D
{
	/* Vector values */
	double x{ 0.f };
	double y{ 0.f };
	double z{ 0.f };
};

struct Point2D
{
	/* Constructors */
	Point2D();
	Point2D(double new_x, double new_y);
	double x{ 0.f };
	double y{ 0.f };
	Point2D addVector(Vector2D vector_2d);
};

struct Point2Di
{
	int x{ 0 };
	int y{ 0 };
	//void from_json(const nlohmann::json& j, Point2Di p);
};

/* Used by :
-Sprites
-Tilesets
*/
struct Cell
{
	Cell();
	Cell(int nx, int ny, int nw, int nh);
	int x{ 0 };
	int y{ 0 };
	int w{ 0 };
	int h{ 0 };
};

/* Operator overloads */
/* Vector2D representing the distance between two points */
Vector2D operator-(Point2D destination, Point2D origin);


// CONE 
struct Cone2D
{
	Point2D m_Origin{};
	Vector2D m_Left_Angle{};
	Vector2D m_Right_Angle{};
};


/* Utility functions */

Point2D addVectorToPoint(Point2D point_2d, Vector2D vector_2d);

/* Compute the dot product between two vectors.
1: Vectors face the same direction
0: Vectors are at a 90 degree angle relative to each other
-1: Vectors face away from each other
Must provide normalized vectors. */

double DotProduct(const Vector2D& a, const Vector2D& b);
double DotProduct(const Vector3D& a, const Vector3D& b);

Vector2D convert_Vec2WorldToLocal(const Vector2D& arg_WorldLoc, const Vector2D& arg_NewOrigin_WorldLoc);
Vector2D convert_Vec2LocalToWorld(const Vector2D& arg_LocalLoc, const Vector2D& arg_CurrentOrigin_WorldLoc);
Vector2D make_RayTrace(const Vector2D& arg_Destination, const Vector2D& arg_Origin);
Vector2D make_RayTrace(const Point2D& arg_Destination, const Point2D& arg_Origin);
Vector2D traceLine(const Vector2D& arg_Destination, const Vector2D& arg_Origin);

