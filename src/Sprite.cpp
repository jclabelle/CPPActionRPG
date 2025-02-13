/* Project */
#include "Sprite.h"
#include "eEntityStateType.h"
#include "eFacingDirections.h"
/* STL */
#include <iostream>
#include <vector>


// Constructors
Sprite::Sprite() {}

Sprite::Sprite(SpriteData new_sprite_data )
:Name{new_sprite_data.Name}
,FrameTimings{new_sprite_data.FrameTimings} 
,Size{new_sprite_data.CellSize}
,CollisionsSize{new_sprite_data.CollisionSize}
,CollisionOffset{new_sprite_data.CollisionOffset}
,FacingsActions_Names{new_sprite_data.FacingsActions_Names}
{}

const Vector2D& Sprite::viewSize() const
{
	return Size;
}

const Vector2D& Sprite::viewCollisionSize() const
{
	return CollisionsSize;
}

const Vector2D& Sprite::viewCollisionOffset() const
{
	return CollisionOffset;
}

/* Return true if the Animation has completely played at least once  */
const bool Sprite::isAnimationCompleted() const
{
	auto Timings = FrameTimings.at(ActiveAnimationName);
	std::chrono::milliseconds Total = std::chrono::milliseconds::duration(static_cast<long long>(std::accumulate(std::begin(Timings), std::end(Timings), 0.0)));

	return std::chrono::steady_clock::now() > Total + AnimationStartTime;
}

std::pair<std::reference_wrapper<SpriteRotation>, std::reference_wrapper<FacingDirections>> Sprite::viewFacingStatus()
{
	return std::pair<std::reference_wrapper<SpriteRotation>, std::reference_wrapper<FacingDirections>>{ AnimNames_LegalRotationsAndOriginalDirection.at(ActiveAnimationName).first, AnimNames_LegalRotationsAndOriginalDirection.at(ActiveAnimationName).second };
}

 const std::string& Sprite::viewName() const
{
	return Name;
}

 const std::string& Sprite::viewActiveAnimationName() const
{
	return ActiveAnimationName;
}

 /* Get the duration of the current Animation */
 std::chrono::steady_clock::duration Sprite::viewFacing_ActionDuration(const FacingDirections& new_direction, const StateType& new_action_type)
 {
	 auto AnimName = FacingsActions_Names.at(std::pair(new_direction, new_action_type));
	 auto Timings = FrameTimings.at(AnimName);
	 auto Total = std::accumulate(std::begin(Timings), std::end(Timings), 0.0);
	 return std::chrono::milliseconds::duration(static_cast<long long>(Total));
 }

 /* Set the Facing and Action */
 void Sprite::setFacing_Action(const FacingDirections& new_direction, const StateType& new_action_type)
 {
	 /* Switch animation if we're not already running this animation */
	if( ActiveAnimationName != FacingsActions_Names.at(std::pair(new_direction, new_action_type)))
	{
	 ActiveAnimationName = FacingsActions_Names.at(std::pair(new_direction, new_action_type));
	 ActiveAnimationFrameID = 0;
	 AnimationStartTime = std::chrono::steady_clock::now();
	}
 }


 int Sprite::viewActiveAnimationFrameID()
 {
	 /* Get the duration of the active animation */
	 double max{ 0 };
	 for (auto FrameTime : FrameTimings.at(ActiveAnimationName))
	 {
		 max += FrameTime;
	 }

	 /* Find how long since we started and get the max as a chrono duration */
	 Elapsed = std::chrono::steady_clock::now() - AnimationStartTime;
	 size_t count{ 0 };
	 std::chrono::duration<double, std::milli> MaxDuration = std::chrono::milliseconds::duration(static_cast<long long>(max));
	
	 /* Use the remain of elapsed time modulus max duration to figure out the current animation time, 
	 then add up frame timings until we go over the current animation time to
	 find the active animation frame ID */
	 auto CurrentAnimationTime = static_cast<long long>(Elapsed.count()) % static_cast<long long>(MaxDuration.count());
	 long long Accumulator{ 0 };
	 Accumulator = static_cast<long long>(FrameTimings.at(ActiveAnimationName)[0]);
	 while (Accumulator <= CurrentAnimationTime)
	 {
		 Accumulator += static_cast<long long>(FrameTimings.at(ActiveAnimationName)[count+1]);
		 ++count;
	 }
	 ActiveAnimationFrameID = count;

	return ActiveAnimationFrameID;
}
