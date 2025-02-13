#pragma once

const double tmp_NodeFrequency_X{ 25};	// A node every tmp_NodeFrequency_X pixels on the x axis
const double tmp_NodeFrequency_Y{ 25 }; // A node every tmp_NodeFrequency_Y pixels on the Y axis
const double tmp_DistanceBuffer_X{ 20 };	// Distance from collisions where Nodes are not allowed.
const double tmp_DistanceBuffer_Y{ 20 };
const int const_CollisionClearance_Map{ 10 }; // How far the AI will go after clearing a map collision.
const int const_CollisionClearance_Entity{ 20 }; // How far the AI will go after clearing a collision with another entity.
const double const_AStarGreed{1};