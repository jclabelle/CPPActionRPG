#include "PerceptionData.h"

PerceptionData::PerceptionData(const Vector2D& arg_WorldLocation
	, const SquareCollision& arg_Collisions
	, const EntityFaction& arg_Faction
	, const FacingDirections& arg_SpriteFacing
	,  GameplayManager& arg_Gameplay
	, const uint64_t arg_GlobalID
	, SenseType arg_Sight
	, SenseType arg_Hearing)
	:m_WorldLocation{arg_WorldLocation}
	,m_Collisions{arg_Collisions}
	,m_Faction{arg_Faction}
	,m_SpriteFacing{arg_SpriteFacing}
	,m_Gameplay{arg_Gameplay}
	,m_GlobalID{arg_GlobalID}
	,m_Sight_Detected{arg_Sight}
	,m_Hearing_Detected{arg_Hearing}
{
	if (m_Sight_Detected == SenseType::SIGHT)
	{
		m_Event_Sight_Latest = std::chrono::steady_clock::now();
		m_Sight_WorldLocation_Latest = m_WorldLocation;
	}

	if (m_Hearing_Detected == SenseType::HEARING)
	{
		m_Event_Hearing_Latest = std::chrono::steady_clock::now();
		m_Hearing_WorldLocation_Latest = m_WorldLocation;
	}
}

void PerceptionData::reset_Detected_Sight_Hearing()
{
	m_Sight_Detected = SenseType::FALSE;
	m_Hearing_Detected = SenseType::FALSE;
}

void PerceptionData::refresh_Data_SH(bool arg_Sight, bool arg_Hearing)
{
	if (arg_Sight)
	{
		m_Event_Sight_Latest = std::chrono::steady_clock::now();
		m_Sight_Detected = SenseType::SIGHT;
		m_Sight_WorldLocation_Latest = m_WorldLocation;
	}

	if (arg_Hearing)
	{
		m_Event_Hearing_Latest = std::chrono::steady_clock::now();
		m_Hearing_Detected = SenseType::HEARING;
		m_Hearing_WorldLocation_Latest = m_WorldLocation;
	}
}
