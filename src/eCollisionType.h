#pragma once

enum class CollisionType
{
	CAMERA,
	GAMEPLAYEFFECT,
	PHYSICAL,
	PERCEPTION_SIGHT,
	PERCEPTION_HEARING,
	NAVMESH,
	IGNORE,	// For entities that need to exist in the scene but don't need to check Collisions


	MAX_COLLISIONS
};