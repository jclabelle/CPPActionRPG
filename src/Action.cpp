#include "Action.h"


Action::Action()
{}

Action::Action(const ActionData& init_action, GameplayEffect init_effect, std::chrono::steady_clock::duration action_duration)
	:Name{init_action.Name}, Effect{init_effect}, ActionEndTime{action_duration+std::chrono::steady_clock::now()}
{}

GameplayEffect&& Action::moveGameplayEffect()
{
	return std::move(Effect);
}

bool Action::isFinished()
{
	return std::chrono::steady_clock::now() > ActionEndTime;
}

void Action::setOffsets(const ActionOffsets& new_offsets)
{
	Offsets = new_offsets;
}
