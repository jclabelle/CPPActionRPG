#pragma once
#include "eEntityFaction.h"
#include <vector>
#include <map>
#include <optional>

class GameStats
{
public:
	bool is_Perceivable() const;
	const EntityFaction& v_Faction() const;
	void set_Faction(const EntityFaction& arg_Faction);
	std::optional<EntityFactionBehaviour> check_FactionReaction(const EntityFaction& arg_Faction);

private:

	EntityFaction m_Faction{ EntityFaction::NEUTRAL };
	//std::vector<EntityFaction> m_Enemy_Factions{ EntityFaction::PLAYER, EntityFaction::PLAYER_ALLY};
	std::map<EntityFaction,EntityFactionBehaviour> m_Faction_Behaviour_Map{ std::pair{EntityFaction::PLAYER,EntityFactionBehaviour::AGGRESSIVE} };
	bool m_PerceivableSight{ true };
	bool m_PerceivableHearing{ true };
};