// Project
#include "classes_Pathfinding.h"
#include "eFacingDirections.h"

// STL
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <memory>
#include <string>

// Libraries
#include "plf_colony.h"
#include "iostream"

Navmesh_Edge::Navmesh_Edge( Navmesh_Tile& arg_first_tile,  Navmesh_Tile& arg_second_tile, std::pair<Point2D, Point2D> arg_edge_endpoints)
	:m_First_Tile{&arg_first_tile}
	,m_Second_Tile{&arg_second_tile}
	,m_Weight{ Vector2D (std::abs(arg_first_tile.m_Node_WorldLocation.x - arg_second_tile.m_Node_WorldLocation.x), std::abs(arg_first_tile.m_Node_WorldLocation.y - arg_second_tile.m_Node_WorldLocation.y)).Length() }
	,m_Left_EndPoint{arg_edge_endpoints.first}
	,m_Right_EndPoint{arg_edge_endpoints.second}
{}

Navmesh_Tile::Navmesh_Tile(SquareCollision arg_Collision, int arg_id)
	:m_ID{ arg_id }
	, m_Tile{ CollisionType::NAVMESH, arg_Collision.location, arg_Collision.size }
	, m_Node_WorldLocation{ (m_Tile.location.x + m_Tile.size.x / 2), (m_Tile.location.y + m_Tile.size.y / 2) }
{}

Navmesh::Navmesh()
{
	std::cout << "\nNavmesh Constructor\n";
}

/// <summary>
/// Compute the two points that make up an Edge.
// 1 - Use the facing of the sides that connect both tiles to find the connecting sides.
// 2 - Get the size of the Vector2D that makes up both of these sides.
// 3 - Compare the Vector2Ds and find the smallest side. The two end points of that side will be the end points of the edge.
/// </summary>
std::pair<Point2D, Point2D> find_Edge_EndPoints(const SquareCollision& arg_collision_a, const FacingDirections& arg_facing_a, const SquareCollision& arg_collision_b, const FacingDirections& arg_facing_b)
{
	auto lmb_FindSideLength = [](const SquareCollision& arg_col, const FacingDirections& arg_face) -> double
	{
		switch (arg_face)
		{
		case FacingDirections::UP:
			return arg_col.calc_TopSide().Length();
		case FacingDirections::DOWN:
			return arg_col.calc_BottomSide().Length();
		case FacingDirections::LEFT:
			return arg_col.calc_LeftSide().Length();
		case FacingDirections::RIGHT:
			return arg_col.calc_RightSide().Length();
		}
		throw  std::runtime_error("find_Edge_Endpoints: lmb_FindSideLenghts: switch failed to find a valid FacingDirectins");
	};

	auto lmb_FindSidePoints = [](const SquareCollision& arg_col, const FacingDirections& arg_face) ->std::pair<Point2D, Point2D>
	{
		switch (arg_face)
		{
		case FacingDirections::UP:
			return std::pair(arg_col.calc_TopLeftCorner(), arg_col.calc_TopRightCorner());
		case FacingDirections::DOWN:
			return std::pair(arg_col.calc_BottomLeftCorner(), arg_col.calc_BottomRightCorner());
		case FacingDirections::LEFT:
			return std::pair(arg_col.calc_TopLeftCorner(), arg_col.calc_BottomLeftCorner());
		case FacingDirections::RIGHT:
			return std::pair(arg_col.calc_TopRightCorner(), arg_col.calc_BottomRightCorner());
		}
		throw  std::runtime_error("find_Edge_Endpoints: lmb_FindSidePoints: switch failed to find a valid FacingDirectins");
	};

	return (lmb_FindSideLength(arg_collision_a, arg_facing_a) <= lmb_FindSideLength(arg_collision_b, arg_facing_b)) ? lmb_FindSidePoints(arg_collision_a, arg_facing_a) : lmb_FindSidePoints(arg_collision_b, arg_facing_b);
}

/// <summary>
/// Construct a Navmesh and moves the result to preserve references
/// </summary>

std::unique_ptr<Navmesh> make_Navmesh(std::map<int, SquareCollision> arg_navmesh_tiles, std::vector<std::tuple<Vector2D, int, FacingDirections, int, FacingDirections>> m_NavMesh_Edges)
{
	std::unique_ptr<Navmesh> tmp_Navmesh{std::make_unique<Navmesh>()};

	// Search for a tile using a TileID
	int tmp_TileID{ -1 };
	auto lmb_FindTile = [&tmp_TileID](const Navmesh_Tile& t) ->bool
	{
		return t.m_ID == tmp_TileID;
	};

	// Add the navmesh tiles to the colony.
	for (const auto& for_Tile : arg_navmesh_tiles)
	{
		tmp_Navmesh->m_Tiles.insert(Navmesh_Tile{ for_Tile.second, for_Tile.first });
	}


	// Add the edges to the colony
	for (const auto& for_Edge : m_NavMesh_Edges)
	{
		// Get refs to the Tiles
		tmp_TileID = std::get<1>(for_Edge);
		auto& tmp_First_Tile = *(std::ranges::find_if(tmp_Navmesh->m_Tiles.begin(), tmp_Navmesh->m_Tiles.end(), lmb_FindTile));
		tmp_TileID = std::get<3>(for_Edge);
		auto& tmp_Second_Tile = *(std::ranges::find_if(tmp_Navmesh->m_Tiles.begin(), tmp_Navmesh->m_Tiles.end(), lmb_FindTile));

		// Find endpoints, create the Edge.
		tmp_Navmesh->m_Edges.insert(Navmesh_Edge{ tmp_First_Tile, tmp_Second_Tile, find_Edge_EndPoints(tmp_First_Tile.m_Tile, std::get<2>(for_Edge), tmp_Second_Tile.m_Tile, std::get<4>(for_Edge)) });
	}

	// Give the Tiles references to their edges
	for (auto& for_Edge : tmp_Navmesh->m_Edges)
	{
		for_Edge.m_First_Tile->m_Edges.push_back(&for_Edge);
		for_Edge.m_Second_Tile->m_Edges.push_back(&for_Edge);
	}

	for (const auto& for_Tile : tmp_Navmesh->m_Tiles)
	{
		for(const auto& for_Edge : for_Tile.m_Edges)
		{
			std::cout << "\nPrinting Tile's Edge addresses from make_Navmesh:" << &for_Edge << "\n";

			if (for_Edge == nullptr)
				std::cout << "\nTile's Edge address is NULLPTR\n";
		}
	}

	for (const auto& for_Tile : tmp_Navmesh->m_Tiles)
	{
		std::cout << "\nPrinting Tile addresses from make_Navmesh:" << &for_Tile << "\n";
	}

	for (const auto& for_Edge : tmp_Navmesh->m_Edges)
	{
		std::cout << "\nPrinting Edge addresses from make_Navmesh:" << &for_Edge << "\n";
	}

	return std::move(tmp_Navmesh);
}