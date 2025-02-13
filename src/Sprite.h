#pragma once
/* Project */
#include "eEntityStateType.h"
#include "eFacingDirections.h"
#include "WorldData.h"
#include "Chronometer.h"
#include "eSpriteRotation.h"
/* STL */
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <set>

// Stores the timing data of animations
class Sprite
{
public:
	// Constructors
	Sprite();
	Sprite(SpriteData new_sprite_data);

	/* State */
	const bool isAnimationCompleted() const;
	std::pair<std::reference_wrapper<SpriteRotation>, std::reference_wrapper<FacingDirections>> viewFacingStatus() ;

	/* View/Access */
	const std::string& viewName() const;
	const std::string& viewActiveAnimationName() const;
	std::chrono::steady_clock::duration viewFacing_ActionDuration(const FacingDirections& new_direction, const StateType& new_action_type);
	int  viewActiveAnimationFrameID();
	const Vector2D& viewSize() const;
	const Vector2D& viewCollisionSize() const;
	const Vector2D& viewCollisionOffset() const;

	/* Set */
	void setFacing_Action(const FacingDirections& new_direction, const StateType& new_action_type);

private:
	std::string Name{ "NOT INITIALIZED" };

	/* Animations */
	std::map<std::string, std::vector<double>> FrameTimings;	// Key: Anim Name, Value: Anim Frame Durations
	std::map<std::pair<FacingDirections, StateType>, std::string> FacingsActions_Names; // Key:Facing + State, Value: Anim Name
	std::map<std::string, std::pair<SpriteRotation, FacingDirections>> AnimNames_LegalRotationsAndOriginalDirection;	// What rotations are allowed with each of the sprite's animations
	std::string ActiveAnimationName{ "NOT INITIALIZED" };
	int ActiveAnimationFrameID{ -1 };
	std::chrono::duration<double, std::milli>  Elapsed{ 0 };
	std::chrono::steady_clock::time_point AnimationStartTime;

	/* Size and Collision offset */
	Vector2D Size{ 0,0 }; // Camera visibility checks use Sprite size
	Vector2D CollisionsSize{ 0,0 };
	Vector2D CollisionOffset{ 0,0 };
};