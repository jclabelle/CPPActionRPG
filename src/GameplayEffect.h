#pragma once

/* Project */
#include "eEntityStateType.h"
#include "VisualEffect.h"
#include "Collisions2D.h"
#include "Navigation.h"
#include "eGameEffect.h"
#include "InitDefaults.h"
#include "eDurationType.h"
#include "Thing.h"
#include "ActionOffsets.h"
/* STL */
#include <string>
#include <chrono>
#include <variant>
#include <optional>

// TODO: Create Class to do gameplay effects

class GameplayEffect : public Thing
{
public:
	GameplayEffect();
	GameplayEffect(const std::string& init_name
		, const int init_zdepth
		, const uint64_t init_creator_id
		, const GameEffect& init_effect_type
		, const ActionOffsets& init_offsets
		, const std::variant<std::chrono::steady_clock::duration, int, bool>& init_lifetime_new
		, const std::optional<VisualEffect>& init_visualeffect = std::nullopt
		, const std::optional<NavigationSystem>& init_navigation = std::nullopt
	);
	bool hasLifetime() const;	// Returns true if the effect has Lifetime left, false if not
	bool isActive() const;	// Returns true if the effect is still running.
	const SquareCollision& viewCollisions();
	const Vector2D& viewEffectLocation();
	const Vector2D& viewSpriteSize();
	Vector2D v_SpriteLocation();
	const std::string& viewSpriteName();
	const std::string& viewAnimationName();
	double findImageRotation();
	const std::size_t viewLifeTimeIndex();
	bool isVisible() const;
	bool hasNavigation() const;
	bool hasCollisionTrigger() const;
	int viewAnimationID();
	void run_FrameCleanUp();

	int viewZDepth();
	void runFrameLogic();

private:
	std::string Name{ NoInit };
	std::optional<VisualEffect> VisualFX{std::nullopt};
	int zDepth{ 0 };
	std::optional<NavigationSystem> Navigation{ std::nullopt };
	GameEffect Effect{GameEffect::GAMEPLAY_EFFECT_MAX};
	uint64_t CreatorID{ 0 };
	ActionOffsets Offsets{};
	
	/* Three possible durations: Index 0: Timer(Chrono), Index 1: Charges(Int), Index 2: Instant(bool), Distance(vector2d) */
	std::variant< std::chrono::steady_clock::time_point, int, bool> LifetimeStart{std::chrono::steady_clock::now()}; // The starting lifetime values
	std::variant< std::chrono::steady_clock::duration, int, bool> LifetimeNow{std::chrono::steady_clock::duration::zero}; // The current lifetime values
	std::variant< std::chrono::steady_clock::time_point, int, bool> LifetimeEnd{ std::chrono::steady_clock::now() };	// The value that indicates the end of the lifetime

};

