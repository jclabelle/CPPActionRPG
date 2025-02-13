

#pragma once
/* Project */
#include "GeometryStructures.h"
#include "eFacingDirections.h"
#include "Collisions2D.h"
#include "InitDefaults.h"
#include "ActionOffsets.h"
/* STL */
#include <vector>
#include <string>
#include <optional>
#include <chrono>

/// <summary>
/// Used by: NavigationSystem
/// A pre-determined path expressed as an array of 2D location vectors and a current
/// position along that path
/// </summary>
struct NavigationPath
{
	NavigationPath();
	NavigationPath(std::string init_name, std::initializer_list<Vector2D> init_waypoints);
	NavigationPath(std::string init_name, std::vector<Vector2D> init_waypoints);
	std::optional<Vector2D> runFrameLogic(double new_speed);
	std::optional<Vector2D> runFrameMove(double new_speed, Vector2D new_local_location);

	std::string Name{ NoInit };
	std::vector<Vector2D> Waypoints;	// Describes a series of locations relative to 0,0
	Vector2D LocalLocation{};
	size_t Waypoints_Index{ 1 };	// The waypoint we are currently moving towards - starts at 1. Index 0 is always = 0,0 to make code simpler
};

/// <summary>
/// Used by: GameplayEffect
/// Iterates through an array of NavigationPaths, situating them in the game world
/// by using facing, collision and animation data from VisualEffect
/// </summary>
class NavigationSystem
{
public:
	/* Constructors */
	NavigationSystem();
	NavigationSystem(std::string init_name
		, std::vector<NavigationPath> init_paths
		, Vector2D init_spawn
		, Vector2D init_effect_location
		, double init_speed = 0
		, std::optional<Vector2D> init_sprite_size= std::nullopt
		, std::optional<Vector2D> init_sprite_collision_size= std::nullopt
		, std::optional<Vector2D> init_sprite_collision_offset= std::nullopt
	, std::optional<ActionOffsets> init_action_offsets = std::nullopt
	, std::optional<FacingDirections> init_facing_direction = std::nullopt);

	/* View */
	const Vector2D& viewEffectLocation();
	const Vector2D& viewSpriteSize();
	Vector2D viewSpriteLocation();
	const SquareCollision& viewCollisions() const;
	bool runFrameLogic();
	bool isActive();

	/* Set */
	void setSpriteSize(Vector2D new_size);
	void setNewSprite(const Vector2D& new_size
		, const Vector2D& new_collision_offset
		, const Vector2D& new_collision_size
	, const FacingDirections& new_facing_direction);


private:
	std::string Name{NoInit};

	/* Location and physics */
	Vector2D SpawnLocation{};
	Vector2D EffectLocation{};
	SquareCollision Collisions{};
	std::optional<FacingDirections> Facing{};
	std::optional<ActionOffsets> ActOffsets{};

	/* Sprite */
	std::optional<Vector2D> SpriteSize{};
	std::optional<Vector2D> SpriteCollisionOffset{};
	std::optional<Vector2D> SpriteCollisionSize{};

	/* Paths */
	size_t Paths_Index{ 0 };
	std::vector<NavigationPath> Paths{};
	double Speed{0};
	std::chrono::steady_clock::time_point StartTime{ std::chrono::steady_clock::now() };

	/* Methods */
	Vector2D rotateRightToFacing(FacingDirections new_facing, Vector2D rotating_vector);
};