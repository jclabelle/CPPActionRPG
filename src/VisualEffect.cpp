
/* Project */
#include "Sprite.h"
#include "VisualEffect.h"
/* STL */
#include <optional>
#include <iostream> 

VisualEffect::VisualEffect()
{}

VisualEffect::VisualEffect(std::string init_name, std::vector<Sprite> init_sprite_collection, std::map<size_t, StateType> init_state_sequence)
	:Name{init_name}, SpriteCollection{init_sprite_collection}
{}

VisualEffect::VisualEffect(std::string init_name, std::vector<Sprite> init_sprite_collection, std::map<size_t, std::pair<StateType, FacingDirections>> init_sequence)
	: Name{ init_name }, SpriteCollection{ init_sprite_collection }, Sequence{init_sequence}
{
	//SpriteCollection[SequenceIndex].setFacing_Action(Sequence[ActiveSprite].second, Sequence[ActiveSprite].first);
}

VisualEffect::VisualEffect(std::string init_name, std::vector<Sprite> init_sprite_collection, std::map<size_t, std::pair<StateType, FacingDirections>> init_sequence, std::vector<std::tuple<size_t, StateType, FacingDirections, FacingDirections>> init_sequence_)
	: Name{ init_name }, SpriteCollection{ init_sprite_collection }, Sequence{ init_sequence }, Sequence_{init_sequence_}
{
	const auto& SeqIndexData = Sequence_[SequenceIndex];
	SpriteCollection[std::get<0>(SeqIndexData)].setFacing_Action(std::get<3>(SeqIndexData), std::get<1>(SeqIndexData));
}


VisualEffect::VisualEffect(std::string init_name, std::vector<Sprite> init_sprite_collection, std::map<size_t, StateType> init_state_sequence, FacingDirections init_direction)
	: Name{ init_name }, SpriteCollection{ init_sprite_collection }, OwnerFacing{ init_direction }
{}

const Sprite& VisualEffect::viewSprite() const
{
	return SpriteCollection[std::get<0>(Sequence_[SequenceIndex])];
}

const std::string& VisualEffect::viewSpriteName()
{
	return SpriteCollection[std::get<0>(Sequence_[SequenceIndex])].viewName();
}

const Vector2D& VisualEffect::viewSpriteSize() const
{
	return SpriteCollection[std::get<0>(Sequence_[SequenceIndex])].viewSize();
}

const Vector2D& VisualEffect::viewSpriteCollisionSize() const
{
	return SpriteCollection[std::get<0>(Sequence_[SequenceIndex])].viewCollisionSize();
}

const Vector2D& VisualEffect::viewSpriteCollisionOffset() const
{
	return SpriteCollection[std::get<0>(Sequence_[SequenceIndex])].viewCollisionOffset();
}

int VisualEffect::viewActiveAnimationID()
{
	return SpriteCollection[std::get<0>(Sequence_[SequenceIndex])].viewActiveAnimationFrameID();
}

Sprite& VisualEffect::accessSprite()
{
	return SpriteCollection[std::get<0>(Sequence_[SequenceIndex])];
}

void VisualEffect::setFacing(const FacingDirections& new_facing)
{
	OwnerFacing = new_facing;
}

std::optional<SpriteChange> VisualEffect::runFrameLogic()
{
	//const auto& SeqIndexData = Sequence_[SequenceIndex];
	/* If the current animation is completed */
	//std::cout << "Sprite Anim: "<< SpriteCollection[std::get<0>(Sequence_[SequenceIndex])].isAnimationCompleted()<<'\n';
	if (SpriteCollection[std::get<0>(Sequence_[SequenceIndex])].isAnimationCompleted())
	{
		/* Start the next animation and return the Facing */
		++SequenceIndex;
		//std::cout<<"Index Increment:"<<SequenceIndex<<'\n';
 		if (SequenceIndex < Sequence_.size())
		{
			/* If the previous animation is complete, give it's facing and state to the new Active Sprite,
			then return the data needed by Navigation */
			SpriteCollection[std::get<0>(Sequence_[SequenceIndex])].setFacing_Action(std::get<3>(Sequence_[SequenceIndex]), std::get<1>(Sequence_[SequenceIndex]));
			
			return SpriteChange{
				SpriteCollection[std::get<0>(Sequence_[SequenceIndex])].viewSize()
				,SpriteCollection[std::get<0>(Sequence_[SequenceIndex])].viewCollisionSize()
				,SpriteCollection[std::get<0>(Sequence_[SequenceIndex])].viewCollisionOffset()
			,OwnerFacing };
		}
	}
	return std::optional<SpriteChange>();
}

bool VisualEffect::isSequenceComplete()
{
	return SequenceIndex >= Sequence_.size();
}

bool VisualEffect::doesAnimationNeedsRotating()
{
	/* If the Owner Facing is different from the Sprite's rotation */
	if (OwnerFacing != std::get<2>(Sequence_[SequenceIndex]))
	{
		return true;
	}
	return false;
}

double VisualEffect::findActiveAnimationRotation()
{
	const auto& FacingOnImage = std::get<3>(Sequence_[SequenceIndex]);

	switch (OwnerFacing)
	{
	case FacingDirections::UP:
		switch (FacingOnImage)
		{
		case FacingDirections::LEFT:
			return 90;
		case FacingDirections::DOWN:
			return 180;
		case FacingDirections::RIGHT:
			return 270;
		}
	case FacingDirections::DOWN:
		switch (FacingOnImage)
		{
		case FacingDirections::UP:
			return 180;
		case FacingDirections::LEFT:
			return 270;
		case FacingDirections::RIGHT:
			return 90;
		}
	case FacingDirections::LEFT:
		switch (FacingOnImage)
		{
		case FacingDirections::UP:
			return 270;
		case FacingDirections::DOWN:
			return 90;
		case FacingDirections::RIGHT:
			return 180;
		}
	case FacingDirections::RIGHT:
		switch (FacingOnImage)
		{
		case FacingDirections::UP:
			return 90;
		case FacingDirections::DOWN:
			return 270;
		case FacingDirections::LEFT:
			return 180;
		}
	}
	return 0;
}

bool VisualEffect::isActive()
{
	return SequenceIndex < Sequence_.size();
}
