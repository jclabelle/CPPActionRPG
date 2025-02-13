#include "EntityStates.h"

// Compares two entity states
bool EntityState::operator==(const EntityState& b)
{
	if (Action == b.Action && Facing == b.Facing)
	{
		return true;
	}
	return false;
}

bool operator==(const EntityState& a, const EntityState& b)
{
	if (a.Action == b.Action && a.Facing == b.Facing)
	{
		return true;
	}
	return false;
}


// Validates the state transition
bool EntityEvent::checkValidTo(const EntityEvent& new_event) const
{
	for (auto state : ValidTo)
	{
		if (state == new_event.EventState)
			return true;
	}
	return false;
}


