#pragma once
#include "GeometryStructures.h"
#include "eFacingDirections.h"
#include <chrono>
#include <map> 

struct ActionOffsets
{
	ActionOffsets();
	ActionOffsets(Vector2D init_up, Vector2D init_down, Vector2D init_left, Vector2D init_right, std::chrono::steady_clock::duration init_delay = std::chrono::steady_clock::duration::zero());
	ActionOffsets(std::map<FacingDirections, Vector2D> init_spawn_offsets, std::chrono::steady_clock::duration init_delay=std::chrono::steady_clock::duration::zero());
	Vector2D up{};	// Add to creator location to find Spawnpoint.
	Vector2D down{};	// Add to creator location to find Spawnpoint.
	Vector2D left{};	// Add to creator location to find Spawnpoint.
	Vector2D right{};	// Add to creator location to find Spawnpoint.
	std::map<FacingDirections, Vector2D> SpawnOffsets;
	std::chrono::steady_clock::duration Delay{ std::chrono::steady_clock::duration::zero() };	// Add to creator Now() to find Effect Trigger time.
};