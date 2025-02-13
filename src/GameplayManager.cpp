#include "GameplayManager.h"
#include <ranges>

GameplayManager::GameplayManager()
{}

bool GameplayManager::isActiveActionCompleted()
{
	return ActiveAction.isFinished();
}

void GameplayManager::startNewAction(const EntityState& new_state
	, const uint64_t creator_id
	, const Vector2D& creator_location
	, const std::chrono::steady_clock::duration& action_duration
	, const FacingDirections& owner_facing)
{
	m_GeneratorActionBasicAttack = AvailableActions.at("BasicAttack");

	//if (new_state.Action == StateType::IDLE)
	//	ActiveAction = m_GeneratorActionIdle.generateAction(creator_id, creator_location, action_duration, owner_facing);

	if (new_state.Action == StateType::ATTACK_BASIC)
	{
 		ActiveAction = m_GeneratorActionBasicAttack.generateAction(creator_id, creator_location, action_duration, owner_facing);
		OwnedEffects.push_back(ActiveAction.moveGameplayEffect());
		std::cout << "Added to Owned Effects\n";
	}
	if (new_state.Action == StateType::ACTION_SPECIAL_ONE)
	{
		ActiveAction = m_GeneratorActionSpecialOne.generateAction(creator_id, creator_location, action_duration, owner_facing);
		OwnedEffects.push_back(ActiveAction.moveGameplayEffect());
	}
}

void GameplayManager::run_FrameCleanup()
{
	// TODO: Add method to delete completed effects
	for (auto& effect : OwnedEffects)
	{
		effect.run_FrameCleanUp();
	}

	OwnedEffects.erase(
		std::remove_if(OwnedEffects.begin(), OwnedEffects.end(), [](const GameplayEffect& e) {return e.isActive() == false; }),
		OwnedEffects.end());
	
	//std::cout << OwnedEffects.size();
}

void GameplayManager::moveEffectToOwnedList()
{
	OwnedEffects.push_back(ActiveAction.moveGameplayEffect());
}

void GameplayManager::runFrameLogic()
{
	auto Active = [](const GameplayEffect& ge) {return ge.isActive(); };

	for (auto& GameplayEffect : OwnedEffects | std::views::filter(Active))
	{
		GameplayEffect.runFrameLogic();
	}
	//std::cout << "Owned Effects:" << OwnedEffects.size() << '\n';

	//runCleanup();
}

void GameplayManager::addGenerator(const ActionGenerator& new_action_generator) 
{
	AvailableActions.insert(std::make_pair(new_action_generator.viewName(), new_action_generator));
}

void GameplayManager::setActionOffsets(const std::map<GameEffect, ActionOffsets>& new_action_offsets)
{
	EntityOffsets = new_action_offsets;
}

std::vector<GameplayEffect>& GameplayManager::accessOwnedEffects()
{
	return OwnedEffects;
	
}

GameStats& GameplayManager::Stats()
{
	return m_Stats;
}

const GameStats& GameplayManager::v_Stats() const
{
	return m_Stats;
}


