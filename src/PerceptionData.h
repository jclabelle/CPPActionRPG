#pragma once
// Project
#include "GeometryStructures.h"
#include "Collisions2D.h"
#include "eEntityFaction.h"
#include "eFacingDirections.h"
#include "GameplayManager.h"
#include "eSenseType.h"
#include "classes_Pathfinding.h"
// STL
#include <functional>
#include <chrono>

struct PerceptionData
{
	PerceptionData(const Vector2D& arg_WorldLocation
		, const SquareCollision& arg_Collisions
		, const EntityFaction& arg_Faction
		, const FacingDirections& arg_SpriteFacing
		,  GameplayManager& arg_Gameplay
		, const uint64_t arg_GlobalID
		, SenseType arg_Sight
		, SenseType arg_Hearing);

	// References to the Entity's Member Variables
	const Vector2D& m_WorldLocation;	// From m_Physics
	const SquareCollision& m_Collisions;	// From m_Physics
	const EntityFaction& m_Faction;	// From m_Gameplay.Stats
	const FacingDirections& m_SpriteFacing;	// From m_State
	GameplayManager& m_Gameplay;	// The Entity's GameplayManager
	const uint64_t m_GlobalID;

	// Additional data
	std::chrono::steady_clock::time_point m_Event_Sight_Latest{};	// The most recent time point when this entity registered on Sight
	std::chrono::steady_clock::time_point m_Event_Hearing_Latest{};	// The most recent time point when this entity registered on Hearing
	SenseType m_Sight_Detected{ SenseType::SIGHT };	// If==Sight, was seen in the last Perception check
	SenseType m_Hearing_Detected{ SenseType::HEARING }; // IF==Hearing, was heard in the last Perception check
	Vector2D m_Sight_WorldLocation_Latest{};	// Entity World Location at the time of the latest Sight detection
	Vector2D m_Hearing_WorldLocation_Latest{};	//  Entity World Location at the time of the latest Hearing detection
	bool m_IsReachable{ true };	// Can the Entity be reached by moving straight towards it, or is a path needed to maneuver around obstructions?
	
	// Methods
	void refresh_Data_SH(bool arg_Sight, bool arg_Hearing);
	void reset_Detected_Sight_Hearing();
};