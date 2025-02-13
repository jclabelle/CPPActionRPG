#pragma once
/* Project */
#include "WorldData.h"
#include "Sprite.h"
#include "Action.h"
#include "eFacingDirections.h"
/* STL */
#include <vector>

class ActionGenerator
{
public:
	ActionGenerator();
	ActionGenerator(const ActionData& init_action_data
		, const GameplayEffectData& init_gameplayeffect_data
		, const NavigationSystemData& init_navigationsystem_data
		, const VisualEffectData& init_visualeffect_data
		, const std::vector<SpriteData>& init_spritescollection_data
		, const std::vector<NavigationPathData>& init_navigationpath_data);

	const std::string& viewName() const;
	const GameEffect& viewGameplayEffect() ;

	Action generateAction(const uint64_t creator_id
		, const Vector2D& creator_location
		, const std::chrono::steady_clock::duration& action_duration
		, const FacingDirections& owner_facing);

	void setOffsetData(const ActionOffsets& new_offsets);

private:
	std::string ActionName{};
	ActionData ActData{};
	GameplayEffectData GplayData{};
	NavigationSystemData NavData{};
	VisualEffectData VfxData{};
	std::vector<SpriteData> SpritesCollectionData{};
	std::vector<NavigationPathData> PathsData{};
	ActionOffsets ActionOffsetsData{};

	NavigationPath makeNavigationPath(const NavigationPathData& new_navigationpath_data);
	NavigationSystem makeNavigationSystem(const NavigationSystemData& new_navigationsystem_data, const Vector2D& spawn_location, const FacingDirections& owner_facing, double new_speed = 0, const std::optional<Vector2D>& new_sprite_size = std::nullopt, const std::optional<Vector2D>& new_sprite_collision_size = std::nullopt, const std::optional<Vector2D>& new_sprite_collision_offset = std::nullopt);
	VisualEffect makeVisualEffect(const VisualEffectData& new_visualeffect_data);
	GameplayEffect makeGameplayEffect(const GameplayEffectData& new_gameplayeffect_data, const uint64_t creator_id, const ActionOffsets& init_offsets, const Vector2D& spawn_point, const FacingDirections& owner_facing);
	Action makeAction(const ActionData& new_action_data, const uint64_t creator_id, const Vector2D& spawn_point, const ActionOffsets& init_offsets, const FacingDirections& owner_facing, const std::chrono::steady_clock::duration& action_duration);
};