#include "ActionOffsets.h"

ActionOffsets::ActionOffsets()
{}

ActionOffsets::ActionOffsets(Vector2D init_up, Vector2D init_down, Vector2D init_left, Vector2D init_right, std::chrono::steady_clock::duration init_delay)
	:up{init_up}, down{init_down}, left{init_left}, right{init_right}, Delay{init_delay}
{}

ActionOffsets::ActionOffsets(std::map<FacingDirections, Vector2D> init_spawn_offsets, std::chrono::steady_clock::duration init_delay)
	:SpawnOffsets{init_spawn_offsets}, Delay{init_delay}
{}
