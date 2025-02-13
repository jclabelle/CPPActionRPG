/* Project */
#include "Navigation.h"
#include "eFacingDirections.h"
#include "GeometryStructures.h"
#include <iostream>

NavigationPath::NavigationPath()
{}

NavigationPath::NavigationPath(std::string init_name, std::initializer_list<Vector2D> init_waypoints)
	:Name{ init_name }, Waypoints{ init_waypoints }
{}

NavigationPath::NavigationPath(std::string init_name, std::vector<Vector2D> init_waypoints)
	: Name{ init_name }, Waypoints{ init_waypoints }
{}

std::optional<Vector2D> NavigationPath::runFrameLogic(double new_speed)
{
	return runFrameMove(new_speed, LocalLocation);
}


std::optional<Vector2D> NavigationPath::runFrameMove(double new_speed, Vector2D new_local_location)
{
	if (Waypoints_Index < Waypoints.size())
	{
		Vector2D Local = new_local_location;

		/* Moving from Waypoints[CurrentIndex-1] to Waypoints[CurrentIndex] */
		Vector2D MoveDirection = Waypoints[Waypoints_Index] - Waypoints[Waypoints_Index - 1];
		Vector2D MoveDirectionNormalized = MoveDirection.Normalized();
		Vector2D Move = MoveDirectionNormalized * new_speed * (1.0 / 60.0);
		Local = Local + Move;

		/* If we've gone past the Waypoint, retrieve the remaining "speed" and call move again */
		if ((Local - Waypoints[Waypoints_Index - 1]).LengthSquared() > MoveDirection.LengthSquared())
		{
			/* Remove the excess movement, set LocalLocation to the current Waypoint and increment Waypoints_Index */
			Vector2D ExcessMove = Local - Waypoints[Waypoints_Index];
			double ExcessSpeed = new_speed * (ExcessMove.LengthSquared() / Move.LengthSquared());
			Move = Move - ExcessMove;
			Local = Waypoints[Waypoints_Index];
			++Waypoints_Index;

			/* Try run Move again with the Excess movement */
			if (auto FrameMoveResult = runFrameMove(ExcessSpeed, Local); (FrameMoveResult))
			{
				Move = Move + *FrameMoveResult;
			}
		}
		LocalLocation = Local;
		return std::make_optional<Vector2D>(Move);
	}
	// Report an empty optional if we've run out of waypoints
	return std::optional<Vector2D>();
}


NavigationSystem::NavigationSystem()
{}

NavigationSystem::NavigationSystem(std::string init_name
	, std::vector<NavigationPath> init_paths
	, Vector2D init_spawn
	, Vector2D init_effect_location
	, double init_speed
	, std::optional<Vector2D> init_sprite_size
	, std::optional<Vector2D> init_sprite_collision_size
	, std::optional<Vector2D> init_sprite_collision_offset
	, std::optional<ActionOffsets> init_action_offsets
	, std::optional<FacingDirections> init_facing_direction)
	: Name{ init_name }
	, Paths{ init_paths }
	, SpawnLocation{ init_spawn }
	, Speed{ init_speed }
	, EffectLocation{ init_effect_location }
	, SpriteSize{ init_sprite_size }
	, SpriteCollisionSize{init_sprite_collision_size}
	, SpriteCollisionOffset{init_sprite_collision_offset}
	, Facing{init_facing_direction}
	, ActOffsets{init_action_offsets}
{
	if ((Facing) && (ActOffsets))
	{
		if (ActOffsets->SpawnOffsets.count(*Facing) > 0)
		{
			if(*Facing==FacingDirections::RIGHT)
			{ 
				SpawnLocation = SpawnLocation + ActOffsets->SpawnOffsets.at(*Facing);
				SpawnLocation.x = SpawnLocation.x - (SpriteCollisionSize->x / 2);
				SpawnLocation.y = SpawnLocation.y - (SpriteCollisionSize->y / 2);
			}

			if (*Facing == FacingDirections::UP)
			{
				SpawnLocation = SpawnLocation + ActOffsets->SpawnOffsets.at(*Facing);
				SpawnLocation.x = SpawnLocation.x - (SpriteCollisionSize->x / 4);
				SpawnLocation.y = SpawnLocation.y + (SpriteCollisionSize->y);
			}

			if (*Facing == FacingDirections::DOWN)
			{
				SpawnLocation = SpawnLocation + ActOffsets->SpawnOffsets.at(*Facing);
				SpawnLocation.x = SpawnLocation.x + (SpriteCollisionSize->x / 4);
				SpawnLocation.y = SpawnLocation.y - (SpriteCollisionSize->y );
			}

			if (*Facing == FacingDirections::LEFT)
			{
				SpawnLocation = SpawnLocation + ActOffsets->SpawnOffsets.at(*Facing);
				SpawnLocation.x = SpawnLocation.x + (SpriteCollisionSize->x / 2);
				SpawnLocation.y = SpawnLocation.y + (SpriteCollisionSize->y / 2);

			}
			EffectLocation = SpawnLocation;


		}
	}

	if ((SpriteSize) && (SpriteCollisionSize) && (SpriteCollisionOffset))
	{
		Collisions = SquareCollision{ CollisionType::GAMEPLAYEFFECT, EffectLocation + *SpriteCollisionOffset, rotateRightToFacing(*Facing, *SpriteCollisionSize )};
		if (Facing)
		{
			//Collisions.size = rotateRightToFacing(*Facing, Collisions.size);
		}
	}

}

const Vector2D& NavigationSystem::viewEffectLocation()
{
	return EffectLocation;
}

const Vector2D& NavigationSystem::viewSpriteSize()
{
	return *SpriteSize;
}

Vector2D NavigationSystem::viewSpriteLocation()
{
	return Vector2D{ EffectLocation.x - SpriteCollisionOffset->x - (Collisions.size.x / 2)
		,EffectLocation.y - SpriteCollisionOffset->y - (Collisions.size.y / 2) };
}

const SquareCollision& NavigationSystem::viewCollisions() const
{
	return Collisions;
}

bool NavigationSystem::runFrameLogic()
{
	// If we have paths left to run
	if(Paths_Index<Paths.size())
	{	// If the path returns a movement, run it through rotation then update the World Location
		if(auto PathResult = Paths[Paths_Index].runFrameLogic(Speed); (PathResult) )
		{
			EffectLocation = EffectLocation + rotateRightToFacing(*Facing, *PathResult);
		}
		else
		{
			++Paths_Index;
		}
		// Report we still have movement left
		return true;
	}
	else
	{	
		// If we're done with all available Paths
		return false;
	}
}

bool NavigationSystem::isActive()
{
	return Paths_Index < Paths.size();
}

void NavigationSystem::setSpriteSize(Vector2D new_size)
{
	if (SpriteSize)
		SpriteSize = new_size;
}

void NavigationSystem::setNewSprite(const Vector2D& new_size, const Vector2D& new_collision_offset, const Vector2D& new_collision_size, const FacingDirections& new_facing_direction)
{
	SpriteSize = new_size;
	SpriteCollisionOffset = new_collision_offset;
	SpriteCollisionSize = new_collision_size;
	Facing = new_facing_direction;
	Collisions = SquareCollision{ CollisionType::GAMEPLAYEFFECT, EffectLocation + *SpriteCollisionOffset, *SpriteCollisionSize };
	Collisions.size = rotateRightToFacing(*Facing, Collisions.size);
}

Vector2D NavigationSystem::rotateRightToFacing(FacingDirections new_facing, Vector2D rotating_vector)
{
	switch (new_facing)

	{
	case FacingDirections::UP:
		return Vector2D{rotating_vector.y, -(rotating_vector.x) };
		break;
	case FacingDirections::LEFT:
		return Vector2D{ -(rotating_vector.x), -(rotating_vector.y) };
		break;
	case FacingDirections::DOWN:
		return Vector2D{ -(rotating_vector.y), (rotating_vector.x) };
		break;
	default:
		return rotating_vector;
	}
}

