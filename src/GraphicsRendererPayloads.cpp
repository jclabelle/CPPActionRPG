
/* Project */ 
#include "GraphicsRendererPayloads.h"
#include "GeometryStructures.h"
#include "eMovementType.h"
#include "eFacingDirections.h"

FrameVisibleAnimation::FrameVisibleAnimation(  
	std::string sprite_name
	, std::string anim_name
	, int anim_frame_id
	, int z_depth
	, Vector2D sprite_location)
: SpriteName{sprite_name}
, AnimationName{anim_name}
, AnimationFrameID{anim_frame_id}
, ZDepth{z_depth}
, SpriteLocation{sprite_location}
{}


