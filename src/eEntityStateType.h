#pragma once

enum class StateType
{
	/* Movement, reserving 1-10*/
	IDLE = 1,
	WALK = 2,

	/* Misc, reserving 11-20 */
	PICKUP = 11,

	/* Basic Combat Moves, reserving 21-40 
	//TODO: Remove "Basic Attack", make everything an "Action"*/ 
	ATTACK_BASIC = 21,

	/* Special Actions, reserving 41-70*/
	ACTION_SPECIAL_ONE = 31,
	ACTION_SPECIAL_TWO = 32,
	ACTION_SPECIAL_THREE = 33,
	ACTION_SPECIAL_FOUR = 34,

	/* Visual Effects: reserving 101-199 */
	VFX_ONE = 101,
	VFX_TWO = 102,
	VFX_THREE = 103,
	VFX_FOUR = 104,
	VFX_FIVE = 105,
	VFX_SIX = 106,

	ACTIONTYPES_MAX
};