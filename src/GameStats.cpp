#include "GameStats.h"
#include <optional>
#include <vector>
#include <stdexcept>

bool GameStats::is_Perceivable() const
{
	return (m_PerceivableSight || m_PerceivableHearing) ? true : false;
}

const EntityFaction& GameStats::v_Faction() const
{
	return m_Faction;
}

void GameStats::set_Faction(const EntityFaction& arg_Faction)
{
	m_Faction = arg_Faction;
}

std::optional<EntityFactionBehaviour> GameStats::check_FactionReaction(const EntityFaction& arg_Faction)
{
	;
	if (m_Faction_Behaviour_Map.count(arg_Faction)>0)
	{
		const EntityFactionBehaviour& Itr = m_Faction_Behaviour_Map.at(arg_Faction);
		return Itr;
	}
	else
	{
		return std::nullopt;
	}
	//return std::optional<const EntityFactionBehaviour&>();
}