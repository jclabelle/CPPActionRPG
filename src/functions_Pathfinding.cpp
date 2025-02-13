// Project
#include "functions_Pathfinding.h"
#include "Collisions2D.h"
#include "GeometryStructures.h"

// STL
#include <stdexcept>
#include <ranges>
#include <limits>
#include <iostream>
#include <numeric>

// Libraries
#include "plf_colony.h"

Navmesh_Tile& traverse_Navmesh_Edge(Navmesh_Tile& arg_tile, Navmesh_Edge& arg_edge)
{
	return &arg_tile == &*arg_edge.m_First_Tile ? *arg_edge.m_Second_Tile : *arg_edge.m_First_Tile;
}

 Navmesh_Tile& find_Current_Navmesh_Tile(const SquareCollision& arg_entity_collision,  plf::colony<Navmesh_Tile>& arg_nav_tiles)
{
	for (auto& Navmesh_Tile : arg_nav_tiles)
	{
		if (detect_Collisions(Navmesh_Tile.m_Tile, arg_entity_collision))
		{
			return Navmesh_Tile;

		}
	}

	throw  std::runtime_error("find_Current_Navmesh_Tile: detect_Collisions: all results negative, NPC is not on a Navmesh Tile. ");
}


std::vector<Navmesh_Edge*> run_AStar_Search(Navmesh_Tile& arg_start_tile, Navmesh_Tile& arg_target_tile)
{
	double tmp_Greed{1};
	auto fun_NotVisited = [](const Navmesh_Tile& t) ->bool {return t.m_NotVisited ? true : false; };

	// Initialize the starting node
	arg_start_tile.m_Weight = 0.0;
	arg_start_tile.m_NotVisited = false;
	Navmesh_Tile* tmp_current_node = &arg_start_tile;
	size_t tmp_CountVisited{ 0 };
	std::vector<Navmesh_Tile*> tmp_NextNodeBatch;
	std::vector<Navmesh_Tile*> tmp_CleanUp;
	tmp_CleanUp.push_back(tmp_current_node);

	// Trying to path between unconnected Nodes = infinite loop
	do
	{
		// Traverse all edges of the current_node, compute or update the weight of any unvisited nodes

		for (auto& for_Edge : tmp_current_node->m_Edges)
		{
			Navmesh_Tile& tmp_Neighbor = traverse_Navmesh_Edge(*tmp_current_node, *for_Edge);

			if (tmp_Neighbor.m_NotVisited == true)
			{
				double g_weight = tmp_current_node->m_Weight + for_Edge->m_Weight;
				double h_weight = (tmp_current_node->m_Node_WorldLocation - tmp_Neighbor.m_Node_WorldLocation).Length();
				double f_weight = (g_weight + h_weight) * tmp_Greed;

				if (f_weight < tmp_Neighbor.m_Weight)
				{
					tmp_Neighbor.m_Weight = f_weight;
					tmp_Neighbor.m_Predecessor = tmp_current_node;
					tmp_Neighbor.m_Predecessor_Edge = for_Edge;
				}
	
				tmp_NextNodeBatch.push_back(&tmp_Neighbor);
				tmp_CleanUp.push_back(&tmp_Neighbor);
			}
		}

		// Remove visited Tiles
		tmp_NextNodeBatch.erase(
			std::remove_if(tmp_NextNodeBatch.begin(), tmp_NextNodeBatch.end(), [](const Navmesh_Tile* t) {return t->m_NotVisited == false; }),
			tmp_NextNodeBatch.end());

		double tmp_LowestWeight{ std::numeric_limits<double>::max() };
		for (auto for_Node : tmp_NextNodeBatch)
		{
			if (for_Node->m_Weight < tmp_LowestWeight)
			{
				tmp_LowestWeight = for_Node->m_Weight;
				tmp_current_node = for_Node;
			}
		}

		// Mark the lowest weight node as visited
		tmp_current_node->m_NotVisited = false;
		++tmp_CountVisited;

		// If we found the target, reconstruct the path
		if (*&tmp_current_node == &arg_target_tile)
		{
			std::vector<Navmesh_Edge*> tmp_Path;
			Navmesh_Tile* tmp_Backtrack = &arg_target_tile;
			//tmp_Path.push_back(arg_target_tile.m_Predecessor_Edge);

			// Backtrack until we find the first Edge
			while (&*tmp_Backtrack->m_Predecessor_Edge != nullptr)
			{
				tmp_Path.push_back(tmp_Backtrack->m_Predecessor_Edge);
				tmp_Backtrack = &*tmp_Backtrack->m_Predecessor;
			}

			// Reset Tiles
			for (auto for_Tile : tmp_CleanUp)
			{
				for_Tile->m_NotVisited = true;
				for_Tile->m_Predecessor = nullptr;
				for_Tile->m_Predecessor_Edge = nullptr;
				for_Tile->m_Weight = std::numeric_limits<double>::max();
			}

			return tmp_Path;
		}

	} 	while (tmp_NextNodeBatch.size() > 0);

	throw  std::runtime_error("run_GraphPath: Failed to find path. ");
}

std::vector<Navmesh_Waypoint> request_Navigation_Path(const Vector2D& start, const Vector2D& destination, Navmesh_Tile& arg_start_tile, Navmesh_Tile& arg_destination_tile)
{

	auto ret_Path_Edges = run_AStar_Search(arg_start_tile, arg_destination_tile);

	auto tmp_itr_Path = ret_Path_Edges.rbegin();

	if (start.x && destination.x) {}

	// Initialize tmp_SSFA_Angle with data from the first Edge
	Cone2D tmp_SSFA_Angle;
	tmp_SSFA_Angle.m_Origin = { start.x, start.y };
	tmp_SSFA_Angle.m_Left_Angle = make_RayTrace(ret_Path_Edges[0]->m_Left_EndPoint, tmp_SSFA_Angle.m_Origin).Normalized();
	tmp_SSFA_Angle.m_Right_Angle = make_RayTrace(ret_Path_Edges[0]->m_Left_EndPoint, tmp_SSFA_Angle.m_Origin).Normalized();

	// Initialize the path, add the midpoint of the first Edge
	std::vector<Navmesh_Waypoint> tmp_Path;


	for (const auto& for_Edge : ret_Path_Edges)
	{
		Vector2D tmp_MidPoint{ std::midpoint(for_Edge->m_Left_EndPoint.x, for_Edge->m_Right_EndPoint.x), std::midpoint(for_Edge->m_Left_EndPoint.y, for_Edge->m_Right_EndPoint.y) };
		tmp_Path.push_back(Navmesh_Waypoint{ tmp_MidPoint, *&for_Edge->m_First_Tile, *&for_Edge->m_Second_Tile });
	}

	return tmp_Path;
}

void reset_Navmesh(Navmesh& arg_navmesh)
{
	for (auto& for_Tile : arg_navmesh.m_Tiles)
	{
		for_Tile.m_NotVisited = true;
		for_Tile.m_Predecessor = nullptr;
		for_Tile.m_Predecessor_Edge = nullptr;
		for_Tile.m_Weight = std::numeric_limits<double>::max();
	}
}