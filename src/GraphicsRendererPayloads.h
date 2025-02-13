#pragma once
/* Project */
#include "GeometryStructures.h"
#include "eFacingDirections.h"
#include "eMovementType.h"
/* STL */
#include <string>
#include <vector>

struct FrameVisibleAnimation
{
	FrameVisibleAnimation(std::string sprite_name
		, std::string anim_name
		, int anim_frame_id
		, int z_depth
		, Vector2D world_location);



	std::string SpriteName{ "NOT INITIALIZED" };
	std::string AnimationName{ "NOT INITIALIZED" };

	Vector2D WorldLocation{};
	Vector2D SpriteLocation{};
	Vector2D Size{};	// Effectively Collision sizes. Used to draw Collisions during debugging
	Vector2D CollisionLocation{};
	Vector2D SightCollisions_WorldLocation{};
	Vector2D SightCollisions_Size{};
	std::pair<Vector2D, Vector2D> m_debug_LineToNearestEnemy;
	std::vector<Vector2D> m_debug_CurrentPath;
	double HearingRange{};

	int AnimationFrameID{ -1 };
	int ZDepth{ 50 };
	double ImageRotation{0};

};
