#pragma once
//Project
#include "Collisions2D.h"
#include "classes_Pathfinding.h"

// STL
#include <vector>

// Libraries
#include "plf_colony.h"


Navmesh_Tile& traverse_Navmesh_Edge(Navmesh_Tile& arg_tile, Navmesh_Edge& arg_edge);

Navmesh_Tile& find_Current_Navmesh_Tile(const SquareCollision& arg_entity_collision,  plf::colony<Navmesh_Tile>& arg_nav_tiles);

std::vector<Navmesh_Edge*> run_AStar_Search(Navmesh_Tile& arg_start_tile, Navmesh_Tile& arg_target_tile);

std::vector<Navmesh_Waypoint> request_Navigation_Path(const Vector2D& start, const Vector2D& destination, Navmesh_Tile& arg_start_tile, Navmesh_Tile& arg_destination_tile);


void reset_Navmesh(Navmesh& arg_navmesh);



