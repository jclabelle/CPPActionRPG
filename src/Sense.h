#pragma once
#include "eSenseType.h"

struct Sense
{
	//const SquareCollision& v_PerceptionBox();
	SenseType m_Type{ SenseType::SENSETYPE_MAX };
	double m_MaxDistance{ 50 };
	double m_MaxDistance_Squared{ m_MaxDistance * m_MaxDistance };
	//SquareCollision Box{ CollisionType::MAX_COLLISIONS, {0,0}, {1,1} };
};