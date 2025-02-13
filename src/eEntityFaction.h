#pragma once

enum class EntityFaction
{
	PLAYER,
	NEUTRAL,
	MONSTER,

	MAX_ENTITYFACTION
};

enum class EntityFactionBehaviour
{
	AGGRESSIVE,
	PASSIVE,
	ALLY,

	MAX_BEHAVIOUR
};