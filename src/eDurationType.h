#pragma once

enum class DurationType
{
	CHRONO = 1,	// Lasts for a given duration 
	CHARGE_MAXIMUM = 2, // Can discharge it's effect X amount of time
	INSTANT = 3, // Immediately discharges it's effect and then disappears
	PERMANENT = 4,	// Lasts as long as the Area is active

	MAX_DURATIONTYPE
};