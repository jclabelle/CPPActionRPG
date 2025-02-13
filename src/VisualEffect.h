#pragma once
/* Project */
#include "Sprite.h"
#include "InitDefaults.h"
#include "GeometryStructures.h"
#include "eFacingDirections.h"
#include "eEntityStateType.h"
/* STL */
#include <optional>

struct SpriteChange
{
	const Vector2D& Size;
	const Vector2D& CollisionSize;
	const Vector2D& CollisionOffset;
	const FacingDirections& Facing;
};

//TODO: Handle Loopings animations, add method to shut down loops when needed.
class VisualEffect
{
public:
	VisualEffect();
	VisualEffect(std::string init_name, std::vector<Sprite> init_sprite_collection, std::map<size_t, StateType> init_state_sequence);
	VisualEffect(std::string init_name, std::vector<Sprite> init_sprite_collection, std::map<size_t, std::pair<StateType, FacingDirections>> init_sequence);
	VisualEffect(std::string init_name, std::vector<Sprite> init_sprite_collection, std::map<size_t, std::pair<StateType, FacingDirections>> init_sequence, std::vector<std::tuple<size_t, StateType, FacingDirections, FacingDirections>> init_sequence_);
	VisualEffect(std::string init_name, std::vector<Sprite> init_sprite_collection, std::map<size_t, StateType> init_state_sequence, FacingDirections init_direction);
	const Sprite& viewSprite() const;
	const std::string& viewSpriteName();
	const Vector2D& viewSpriteSize() const;
	const Vector2D& viewSpriteCollisionSize() const;
	const Vector2D& viewSpriteCollisionOffset() const;
	int viewActiveAnimationID();
	Sprite& accessSprite();
	void setFacing(const FacingDirections& new_facing);
	std::optional<SpriteChange> runFrameLogic();
	bool isSequenceComplete();
	bool isActive();
	double findActiveAnimationRotation();
	bool doesAnimationNeedsRotating();
	

	/* Plays an animation sequence. Sprite is selected using the direction + the State from StateOrder, starting from Zero.*/
private:
	std::string Name{ NoInit };
	std::vector<Sprite> SpriteCollection;
	std::map<size_t, std::pair<StateType, FacingDirections>> Sequence;
	std::vector<std::tuple<size_t, StateType, FacingDirections, FacingDirections>> Sequence_;	// Index: Animation State, Animation Rotation Rule, Sprite Original Rotation
	size_t SequenceIndex{0};
	//size_t ActiveSprite{ 0 };
	std::chrono::steady_clock::duration ActiveAnimationDuration{};
	std::chrono::steady_clock::time_point ActiveAnimationStartTime{};
	FacingDirections OwnerFacing{};

};

