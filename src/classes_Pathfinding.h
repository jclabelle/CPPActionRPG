#pragma once
// Project
#include "Collisions2D.h"
#include "eCollisionType.h"
#include "eFacingDirections.h"

// STL
#include <vector>
#include <utility>
#include <map>
#include <limits>
#include <string>

// Libraries
#include "plf_colony.h"

///the elements in a navmesh:
// 1- The array of navmesh tiles in the level. For each tile:
//	a) invariant data: its tile (a squarecollision), world location of its AStar node for search purpose (center point of the square collision) and an array of pointers or references to its Edges.
//  b) variable data for AStar search purpose: current predecessor AStar Node, current weight and current visited status.
// 3- The array of edges that connects the navmesh tiles together:
//  a) invariant data: Pointers or references to the two Navmesh tiles it connects, its AStar weight (the distance between the two tile's AStar Node/centerpoints) and the left and right points (world locations) of the edge for ssfa purposes. 
///

struct Navmesh_Tile;	// fwd. decl.

struct Navmesh_Edge
{
	Navmesh_Edge( Navmesh_Tile& arg_first_tile,  Navmesh_Tile& arg_second_tile, std::pair<Point2D, Point2D> arg_edge_points);

	Navmesh_Tile* m_First_Tile;	// AStar. First connected Tile.
	Navmesh_Tile* m_Second_Tile;	// AStar. Second connected Tile.
	const double m_Weight;	// AStar. Cost of the Edge during a search.
	const Point2D m_Left_EndPoint;	// SSFA
	const Point2D m_Right_EndPoint; // SSFA
};

struct Navmesh_Tile
{
	Navmesh_Tile(SquareCollision arg_Collision, int arg_id);

	const int m_ID{ -1 }; // Unique ID within the Navmesh
	const SquareCollision m_Tile{ CollisionType::NAVMESH, {0,0}, {1,1} };	// Navigation limits, must not overlap with Physical Collisions. Invariant.
	const Point2D m_Node_WorldLocation{(m_Tile.location.x + m_Tile.size.x/2), (m_Tile.location.y + m_Tile.size.y / 2) };	// AStar Node. Invariant.
	mutable std::vector<Navmesh_Edge*> m_Edges;								// AStar. Invariant.
	
	bool m_NotVisited{ true };										// AStar. Visited status during a search.
	Navmesh_Tile* m_Predecessor{ nullptr };							// AStar. Predecessor : node traversed to get to this node.
	Navmesh_Edge* m_Predecessor_Edge{ nullptr };
	double m_Weight{ std::numeric_limits<double>::max() };			// AStar. Cost of the the node during a search.
};

struct Navmesh
{
	Navmesh();
	plf::colony<Navmesh_Tile> m_Tiles;
	plf::colony<Navmesh_Edge> m_Edges;
};

struct Navmesh_Waypoint
{
	Vector2D m_Location{};
	Navmesh_Tile* m_First_Tile_Ptr{};
	Navmesh_Tile* m_Second_Tile_Ptr{};
};

std::pair<Point2D, Point2D> find_Edge_EndPoints(const SquareCollision& arg_collision_a, const FacingDirections& arg_facing_a, const SquareCollision& arg_collision_b, const FacingDirections& arg_facing_b);

std::unique_ptr<Navmesh> make_Navmesh(std::map<int, SquareCollision> arg_navmesh_tiles, std::vector<std::tuple<Vector2D, int, FacingDirections, int, FacingDirections>>m_NavMesh_Edges);

