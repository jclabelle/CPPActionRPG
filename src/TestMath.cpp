#include "TestMath.h"
#include "GeometryStructures.h"
#include <iostream>

void TestMath()
{
	Vector2D right(4, 0);

	Vector2D down(0, -5);

	Vector2D sum = right + down;

	std::cout << "Velocity: (" << sum.x << ", " << sum.y << ")\n";
}