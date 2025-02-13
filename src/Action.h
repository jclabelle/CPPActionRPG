#pragma once
#include "GameplayEffect.h"
#include <chrono>

class Action
{
public:
	Action();
	Action(const ActionData& init_action, GameplayEffect init_effect, std::chrono::steady_clock::duration action_duration);
	GameplayEffect&& moveGameplayEffect();
	bool isFinished();
	void setOffsets(const ActionOffsets& new_offsets);

private:
	std::string Name{};
	GameplayEffect Effect{};
	std::chrono::steady_clock::time_point ActionEndTime{};	// The time point after which the action is finished.
	ActionOffsets Offsets;
	//void OnStart(std::chrono::steady_clock::duration &entity_action_duration );
};

const Action actBasicAttack;
const Action actIdle;
const Action actWaterBlast;