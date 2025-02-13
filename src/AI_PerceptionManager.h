#pragma once
// Project
#include "GameplayManager.h"
#include "PerceptionData.h"
#include "Sense.h"
#include "GeometryStructures.h"
#include "functions_Pathfinding.h"
// STL
#include <vector>
#include <memory>

struct AI_PerceptionManager
{
public:
	// Returns the list Perceived Gameplay Managers
	std::map<uint64_t,PerceptionData>& run_FrameLogic(const Vector2D& arg_OwningEntityWorldLoc, const FacingDirections& arg_SpriteFacing);
	const Sense& Sight() const;
	const Sense& Hearing() const;
	void set_OwnerCollisions(std::reference_wrapper<SquareCollision>  arg_square_collision);
	void reset_Detected_SH();
	const Vector2D& v_OwnerWorldLocation() const;
	const SquareCollision& v_OwnerCollisions() const;
	std::map<uint64_t, PerceptionData>& PerceivedPopulation();
	SquareCollision debug_v_SightCollision();
	double v_Hearing_Range();
	std::pair<SenseType, SenseType> is_Perceived(const SquareCollision& arg_EntityCollisions, const Vector2D& arg_EntityWorldLocation) const;
	void register_Entity(const Vector2D& arg_WorldLocation
		, const SquareCollision& arg_Collisions
		, const EntityFaction& arg_Faction
		, const FacingDirections& arg_SpriteFacing
		,  GameplayManager& arg_Gameplay
		, const uint64_t arg_GlobalID
		, const SenseType& arg_Sight
		, const SenseType& arg_Hearing);

	Sense m_Sight{SenseType::SIGHT, 50};
	Sense m_Hearing{SenseType::HEARING, 10};
	std::map<uint64_t, PerceptionData> m_PerceivedPopulation{};
	Vector2D m_OwningEntityWorldLocation{0,0};
	SquareCollision* m_OwningEntityCollision;
	FacingDirections m_OwningEntitySpriteFacing{FacingDirections::DOWN};

	// Internal Methods
	bool check_Sense_Range(const Vector2D& arg_WorldLocation, const double arg_Sense_MaxDistance_Squared) const;
	bool is_Seen(const SquareCollision& arg_EntityCollisions, const Vector2D& arg_EntityWorldLocation) const;
	bool is_Heard(const Vector2D& arg_EntityWorldLocation) const;


	SquareCollision debug_RefreshSightCollision(const Vector2D& arg_EntityWorldLocation);
	SquareCollision debug_SightCollision;
};