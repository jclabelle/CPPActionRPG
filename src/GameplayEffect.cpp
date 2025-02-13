#include "GameplayEffect.h"
#include "Navigation.h"
#include <cassert>

GameplayEffect::GameplayEffect()
{}

GameplayEffect::GameplayEffect(const std::string& init_name
	, const int init_zdepth
	, const uint64_t init_creator_id
	, const GameEffect& init_effect_type
	, const ActionOffsets& init_offsets
	, const std::variant<std::chrono::steady_clock::duration, int, bool>& init_lifetime
	, const std::optional<VisualEffect>& init_visualeffect
	, const std::optional<NavigationSystem>& init_navigation)
	:Name{init_name}
	,zDepth{init_zdepth}
	,CreatorID{init_creator_id}
	,Effect{init_effect_type}
	,Offsets{init_offsets}
	,LifetimeNow{init_lifetime}
	,VisualFX{init_visualeffect}
	,Navigation{init_navigation}
{
	if (init_lifetime.index() == 0)
	{
		/* Time */
		LifetimeStart.emplace<0>(std::chrono::steady_clock::now());
		LifetimeEnd.emplace<0>(std::chrono::steady_clock::now() + std::get<std::chrono::steady_clock::duration>(init_lifetime));
	}
	if (LifetimeNow.index() == 1)
	{
		/* Charges */
		LifetimeStart.emplace<1>(std::get<int>(LifetimeNow));
		LifetimeEnd.emplace<1>(0);
	}

	if (LifetimeNow.index() == 2)
	{
		/* Bool */
		LifetimeStart.emplace<2>(std::get<bool>(LifetimeNow));
		LifetimeEnd.emplace<2>(false);
	}
}

bool GameplayEffect::hasLifetime() const
{
	if (LifetimeNow.index() == 0)
	{
		/* If the duration is not completely over */
		return std::chrono::steady_clock::now() < std::get<std::chrono::steady_clock::time_point>(LifetimeEnd);
	}

	if (LifetimeNow.index() == 1)
	{
		/* If there are charges left */
		return std::get<int>(LifetimeNow) > std::get<int>(LifetimeEnd);
	}

	if (LifetimeNow.index() == 2)
	{
		/* If the effect has not yet affected an Entity */
		return std::get<bool>(LifetimeNow);
	}

	return false;
}

bool GameplayEffect::isActive() const
{
	return (!VisualFX && !Navigation) ? false : true;
}


const SquareCollision& GameplayEffect::viewCollisions()
{
	return Navigation->viewCollisions();
}

const Vector2D& GameplayEffect::viewEffectLocation()
{
	return Navigation->viewEffectLocation();
}

const Vector2D& GameplayEffect::viewSpriteSize()
{// TODO: Do we need the setter here?

	return VisualFX->viewSpriteSize();
}

Vector2D GameplayEffect::v_SpriteLocation()
{
	return Navigation->viewSpriteLocation();
}

const std::string& GameplayEffect::viewSpriteName()
{
	return VisualFX->viewSpriteName();
}

const std::string& GameplayEffect::viewAnimationName()
{
	return VisualFX->accessSprite().viewActiveAnimationName();
}

// Called by Area::listVisibleAnimations()
double GameplayEffect::findImageRotation()
{
	if (VisualFX)
	{
		if (VisualFX->doesAnimationNeedsRotating())
			return VisualFX->findActiveAnimationRotation();
	}
	return 0;
}

const std::size_t GameplayEffect::viewLifeTimeIndex()
{
	return LifetimeNow.index();
}

bool GameplayEffect::isVisible() const
{
	if (VisualFX)
		return true;
	
	return false;
}

bool GameplayEffect::hasNavigation() const
{
	if (Navigation)
		return true;

	return false;
}

/* Returns true if the effect requires a collision to trigger */
bool GameplayEffect::hasCollisionTrigger() const
{
	if (Navigation)
	{
		if(Navigation->viewCollisions().viewType()==CollisionType::GAMEPLAYEFFECT)
		return true;
	}
	return false;
}

int GameplayEffect::viewAnimationID()
{
	return VisualFX->viewActiveAnimationID();
}

void GameplayEffect::run_FrameCleanUp()
{
	if (VisualFX)
	{
		if (VisualFX->isActive()==false)
		{
			VisualFX.reset();
		}
	}

	if (!VisualFX)
	{
		if (Navigation)
		{
			if (!Navigation->isActive())
			{
				Navigation.reset();
			}
		}
	}
}

int GameplayEffect::viewZDepth()
{
	return zDepth;
}

void GameplayEffect::runFrameLogic()
{
	if (VisualFX)
	{
		auto NewSprite = VisualFX->runFrameLogic();
		
		if (NewSprite)
		{
			Navigation->setNewSprite(NewSprite->Size, NewSprite->CollisionOffset, NewSprite->CollisionSize, NewSprite->Facing);
		}
	}

	if (Navigation)
	{
		Navigation->runFrameLogic();
	}

	//run_FrameCleanUp();
}