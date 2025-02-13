#include "AI_PerceptionManager.h"
#include "GeometryStructures.h"
#include "Collisions2D.h"

// Pair(Sight, Hearing) contains the result of Perception in Pair(Sight, Hearing)
std::map<uint64_t, PerceptionData>& AI_PerceptionManager::run_FrameLogic(const Vector2D& arg_EntityWorldLoc, const FacingDirections& arg_SpriteFacing)
{
	m_OwningEntityWorldLocation = arg_EntityWorldLoc;
	//std::cout<< "OwningEntityWorldLocation" << m_OwningEntityWorldLocation.x<<" "<< m_OwningEntityWorldLocation.y<< '\n';
	m_OwningEntitySpriteFacing = arg_SpriteFacing;
	debug_SightCollision= debug_RefreshSightCollision(arg_EntityWorldLoc);	// Debug function to refresh the size of the SightCollision in order to give it to the graphics manager for draw.

	return m_PerceivedPopulation;
}

const Sense& AI_PerceptionManager::Sight() const
{
	return m_Sight;
}

const Sense& AI_PerceptionManager::Hearing() const
{
	return m_Hearing;
}
void AI_PerceptionManager::set_OwnerCollisions(std::reference_wrapper<SquareCollision> arg_square_collision)
{
	m_OwningEntityCollision = &arg_square_collision.get();
	std::cout << "AI_PerceptionManager::set_OwnerCollisions, Collision Location:" << arg_square_collision.get().location.x;
}
// Used at start of Perception check to reset the status of each PerceivedPopulation's m_Sight_Detected and m_Hearing_Detected to False
void AI_PerceptionManager::reset_Detected_SH()
{
	for (auto& for_Perceived : m_PerceivedPopulation)
	{
		for_Perceived.second.reset_Detected_Sight_Hearing();
	}
}

const Vector2D& AI_PerceptionManager::v_OwnerWorldLocation() const
{
	return m_OwningEntityWorldLocation;
}

const SquareCollision& AI_PerceptionManager::v_OwnerCollisions() const
{
	return *m_OwningEntityCollision;
}

std::map<uint64_t, PerceptionData>& AI_PerceptionManager::PerceivedPopulation()
{
	return m_PerceivedPopulation;
}

SquareCollision AI_PerceptionManager::debug_v_SightCollision()
{
	return debug_SightCollision;
}

double AI_PerceptionManager::v_Hearing_Range()
{
	return m_Hearing.m_MaxDistance;
}

bool AI_PerceptionManager::check_Sense_Range(const Vector2D& arg_WorldLocation, const double arg_Sense_MaxDistance_Squared) const
{
	return (arg_WorldLocation - m_OwningEntityWorldLocation).LengthSquared() < arg_Sense_MaxDistance_Squared;
}



bool AI_PerceptionManager::is_Seen(const SquareCollision& arg_EntityCollisions, const Vector2D& arg_EntityWorldLocation) const
{
	if (check_Sense_Range(arg_EntityWorldLocation, m_Sight.m_MaxDistance_Squared) == true)
	{
		SquareCollision PerceptionArea;
		switch (m_OwningEntitySpriteFacing)
		{
		case FacingDirections::RIGHT:
			PerceptionArea.size.x = m_Sight.m_MaxDistance;
			PerceptionArea.size.y = m_Sight.m_MaxDistance * 2;
			PerceptionArea.location.x = arg_EntityWorldLocation.x;
			PerceptionArea.location.y = arg_EntityWorldLocation.y - m_Sight.m_MaxDistance;
			break;
		case FacingDirections::LEFT:
			PerceptionArea.size.x = m_Sight.m_MaxDistance;
			PerceptionArea.size.y = m_Sight.m_MaxDistance * 2;
			PerceptionArea.location.x = arg_EntityWorldLocation.x-m_Sight.m_MaxDistance;
			PerceptionArea.location.y = arg_EntityWorldLocation.y - m_Sight.m_MaxDistance;
			break;
		case FacingDirections::UP:
			PerceptionArea.size.x = m_Sight.m_MaxDistance * 2;
			PerceptionArea.size.y = m_Sight.m_MaxDistance;
			PerceptionArea.location.x = arg_EntityWorldLocation.x - m_Sight.m_MaxDistance;
			PerceptionArea.location.y = arg_EntityWorldLocation.y - m_Sight.m_MaxDistance;
			break;
		case FacingDirections::DOWN:
			PerceptionArea.size.x = m_Sight.m_MaxDistance * 2;
			PerceptionArea.size.y = m_Sight.m_MaxDistance;
			PerceptionArea.location.x = arg_EntityWorldLocation.x - m_Sight.m_MaxDistance;
			PerceptionArea.location.y = arg_EntityWorldLocation.y;
			break;
		}
		return detect_Collisions(arg_EntityCollisions, PerceptionArea) ? true : false;
	}
	return false;
}

bool AI_PerceptionManager::is_Heard(const Vector2D& arg_EntityWorldLocation) const
{
	if (check_Sense_Range(arg_EntityWorldLocation, m_Hearing.m_MaxDistance_Squared) == true)
	{
		return true;
	}
	return false;
}

SquareCollision AI_PerceptionManager::debug_RefreshSightCollision(const Vector2D& arg_EntityWorldLocation)
{
	SquareCollision PerceptionArea;
	switch (m_OwningEntitySpriteFacing)
	{
	case FacingDirections::RIGHT:
		PerceptionArea.size.x = m_Sight.m_MaxDistance;
		PerceptionArea.size.y = m_Sight.m_MaxDistance * 2;
		PerceptionArea.location.x = arg_EntityWorldLocation.x;
		PerceptionArea.location.y = arg_EntityWorldLocation.y - m_Sight.m_MaxDistance;
		break;
	case FacingDirections::LEFT:
		PerceptionArea.size.x = m_Sight.m_MaxDistance;
		PerceptionArea.size.y = m_Sight.m_MaxDistance * 2;
		PerceptionArea.location.x = arg_EntityWorldLocation.x - m_Sight.m_MaxDistance;
		PerceptionArea.location.y = arg_EntityWorldLocation.y - m_Sight.m_MaxDistance;
		break;
	case FacingDirections::UP:
		PerceptionArea.size.x = m_Sight.m_MaxDistance * 2;
		PerceptionArea.size.y = m_Sight.m_MaxDistance;
		PerceptionArea.location.x = arg_EntityWorldLocation.x - m_Sight.m_MaxDistance;
		PerceptionArea.location.y = arg_EntityWorldLocation.y - m_Sight.m_MaxDistance;
		break;
	case FacingDirections::DOWN:
		PerceptionArea.size.x = m_Sight.m_MaxDistance * 2;
		PerceptionArea.size.y = m_Sight.m_MaxDistance;
		PerceptionArea.location.x = arg_EntityWorldLocation.x - m_Sight.m_MaxDistance;
		PerceptionArea.location.y = arg_EntityWorldLocation.y;
		break;
	}
	return PerceptionArea;
}

std::pair<SenseType, SenseType> AI_PerceptionManager::is_Perceived(const SquareCollision& arg_EntityCollisions, const Vector2D& arg_EntityWorldLocation) const
{
	std::pair<SenseType, SenseType> tmp_Pair_See_Hear{SenseType::FALSE, SenseType::FALSE};
	if (is_Seen(arg_EntityCollisions, arg_EntityWorldLocation))
		tmp_Pair_See_Hear.first = SenseType::SIGHT;

	if (is_Heard(arg_EntityWorldLocation))
		tmp_Pair_See_Hear.second = SenseType::HEARING;

	return tmp_Pair_See_Hear;
}


void AI_PerceptionManager::register_Entity(const Vector2D& arg_WorldLocation
	, const SquareCollision& arg_Collisions
	, const EntityFaction& arg_Faction
	, const FacingDirections& arg_SpriteFacing
	,  GameplayManager& arg_Gameplay
	, const uint64_t arg_GlobalID
	, const SenseType& arg_Sight
	, const SenseType& arg_Hearing)
{
	if (m_PerceivedPopulation.contains(arg_GlobalID) == false)
	{
		m_PerceivedPopulation.insert(std::pair(
			arg_GlobalID,
			PerceptionData{arg_WorldLocation
			,arg_Collisions
			,arg_Faction
			,arg_SpriteFacing
			,arg_Gameplay
			,arg_GlobalID
			,arg_Sight
			,arg_Hearing}));
	}
	else
	{
		auto iterator = m_PerceivedPopulation.find(arg_GlobalID);
		if(iterator != m_PerceivedPopulation.end())
		{ 
		iterator->second.refresh_Data_SH((arg_Sight == SenseType::SIGHT), (arg_Hearing == SenseType::HEARING));
		}
	}
}
