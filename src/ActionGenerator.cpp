#include "ActionGenerator.h"
#include "Action.h"
#include "eFacingDirections.h"

ActionGenerator::ActionGenerator()
{}

ActionGenerator::ActionGenerator(const ActionData& init_action_data
	, const GameplayEffectData& init_gameplayeffect_data
	, const NavigationSystemData& init_navigationsystem_data
	, const VisualEffectData& init_visualeffect_data
	, const std::vector<SpriteData>& init_spritescollection_data
	, const std::vector<NavigationPathData>& init_navigationpath_data)
	:
	ActionName{init_action_data.Name}
	,ActData{init_action_data}
	, GplayData{init_gameplayeffect_data}
	, NavData{init_navigationsystem_data}
	, VfxData{init_visualeffect_data}
	, SpritesCollectionData{init_spritescollection_data}
	, PathsData{init_navigationpath_data}
{}

const std::string& ActionGenerator::viewName() const
{
	return ActionName;
}

 const GameEffect& ActionGenerator::viewGameplayEffect()
{
	return GplayData.Effect;
}

NavigationPath ActionGenerator::makeNavigationPath(const NavigationPathData& new_navigationpath_data)
{
	return NavigationPath{ new_navigationpath_data.Name, new_navigationpath_data.Waypoints };
}

NavigationSystem ActionGenerator::makeNavigationSystem(const NavigationSystemData& new_navigationsystem_data
	, const Vector2D& spawn_location, const FacingDirections& owner_facing ,double new_speed, const std::optional<Vector2D>& new_sprite_size
	, const std::optional<Vector2D>& new_sprite_collision_size, const std::optional<Vector2D>& new_sprite_collision_offset)
{
	std::vector<NavigationPath> PathArray;
	for (const auto& PathData : PathsData)
	{
		PathArray.push_back(makeNavigationPath(PathData));
	}
	return NavigationSystem(new_navigationsystem_data.Name, PathArray, spawn_location, spawn_location, new_speed
		, new_sprite_size, new_sprite_collision_size, new_sprite_collision_offset, ActionOffsetsData, owner_facing);
}

VisualEffect ActionGenerator::makeVisualEffect(const VisualEffectData& new_visualeffect_data)
{
	std::vector<Sprite> SpriteArray;
	for (const auto& SprData : SpritesCollectionData)
	{
		SpriteArray.push_back(Sprite{ SprData });
	}
	return VisualEffect{ new_visualeffect_data.Name, SpriteArray, new_visualeffect_data.Sequence, new_visualeffect_data.Sequence_ };
}

GameplayEffect ActionGenerator::makeGameplayEffect(const GameplayEffectData& new_gameplayeffect_data, const uint64_t creator_id,
	const ActionOffsets& init_offsets, const Vector2D& spawn_point, const FacingDirections& owner_facing)
{
	std::optional<VisualEffect> Visuals{ makeVisualEffect(VfxData) };
	Visuals->setFacing(owner_facing);

	std::optional<NavigationSystem> Nav = makeNavigationSystem(NavData, spawn_point, owner_facing, new_gameplayeffect_data.Speed,
	 Visuals->viewSpriteSize(), Visuals->viewSpriteCollisionSize(), Visuals->viewSpriteCollisionOffset());

	return GameplayEffect{ new_gameplayeffect_data.Name
	   , new_gameplayeffect_data.zDepth
	   , creator_id
	   , new_gameplayeffect_data.Effect
	   , init_offsets
	   , &new_gameplayeffect_data.Lifetime
	   , Visuals
	   , Nav
	};
}

Action ActionGenerator::makeAction(const ActionData& new_action_data, const uint64_t creator_id, const Vector2D& spawn_point, const ActionOffsets& init_offsets, const FacingDirections& owner_facing, const std::chrono::steady_clock::duration& action_duration)
{
	return Action{new_action_data, makeGameplayEffect(GplayData, creator_id, init_offsets, spawn_point, owner_facing), action_duration };
}

Action ActionGenerator::generateAction(const uint64_t creator_id, const Vector2D& creator_location, const std::chrono::steady_clock::duration& action_duration, const FacingDirections& owner_facing)
{
	return makeAction(ActData, creator_id, creator_location, ActionOffsetsData, owner_facing, action_duration);
}

void ActionGenerator::setOffsetData(const ActionOffsets& new_offsets)
{
	ActionOffsetsData = new_offsets;
}