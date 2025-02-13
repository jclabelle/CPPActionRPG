//Project
#include "AStar.h"
#include "AIConstants.h"
// Libraries
#include "fmt\core.h"
#include "plf_colony.h"
// STL
#include <ranges>
#include <iostream>
#include <limits>
#include <algorithm>
#include <cassert>
#include <chrono>

AStarNode::AStarNode()
{}

AStarNode::AStarNode(Vector2D arg_world_location)
	:m_WorldLocation{ arg_world_location }
{}

// Check if a node is already connected to that Node
bool AStarNode::is_Connected(const AStarNode& arg_node)
{
	for (const auto& for_Edge : m_Edges)
	{
		if (&arg_node == &for_Edge.get().m_First || &arg_node == &for_Edge.get().m_Second)
			return true;
	}

	return false;
}

void AStarNode::reset()
{
	m_NotVisited = true;
	m_Predecessor = nullptr;
	m_Weight = std::numeric_limits<double>::max();
}


void AStarGraph::add_Node()
{
	m_Nodes.push_back(AStarNode());
}



void AStarGraph::add_Node(Vector2D arg_node_location)
{
	m_Nodes.push_back(AStarNode(arg_node_location));
}


void AStarGraph::add_Edge(AStarNode& arg_a, AStarNode& arg_b, double arg_weight)
{
	m_Edges.push_back(AStarEdge{ arg_a, arg_b });
	m_Edges.back().m_Weight = arg_weight;
	arg_a.m_Edges.push_back(m_Edges.back());
	arg_b.m_Edges.push_back(m_Edges.back());
}

// Copy of the Area's collision map
void AStarGraph::set_CollisionMap(const std::vector<SquareCollision> arg_CollisionMap)
{
	m_CollisionMap = arg_CollisionMap;
}


AStarGraph::AStarGraph()
{}

AStarGraph::AStarGraph(std::vector<Vector2D> arg_nodes)
{
	for (const auto& for_Vec2 : arg_nodes)
	{
		m_Nodes.push_back(for_Vec2);
	}
}



std::reference_wrapper<AStarNode>  AStarGraph::traverse_Edge(std::reference_wrapper<AStarNode>  arg_n, std::reference_wrapper<AStarEdge>  arg_e)
{
	return &arg_n.get() == &arg_e.get().m_First ? arg_e.get().m_Second : arg_e.get().m_First;
}

AStarNode& AStarGraph::v_Node(size_t arg_node)
{
	return m_Nodes[arg_node];
}

std::deque<std::reference_wrapper<AStarNode>> AStarGraph::request_Path(const Vector2D start, const Vector2D destination)
{
	std::reference_wrapper<AStarNode> ret_ClosestToStart = find_ClosestReachableNode(start);
	std::reference_wrapper<AStarNode> ret_ClosestToDestination = find_ClosestReachableNode(destination);
	std::deque<std::reference_wrapper<AStarNode>> ret_Path = run_GraphPath(ret_ClosestToStart, ret_ClosestToDestination);

	std::cout << m_Duration << "\t:Duration of A_Star search \n";
	m_Duration = 0;

	
	for (auto& for_Node : m_Nodes)
	{
		for_Node.reset();
	}

	return ret_Path;

}

//TODO: Split this in find reachable and find closest from list with arguments, ex: closest that is between
// the AI and the Player, so that the AI won't go backwards when starting his route. 
std::reference_wrapper<AStarNode> AStarGraph::find_ClosestReachableNode(const Vector2D arg_location)
{

	double tmp_ShortestDistance = std::numeric_limits<double>::max();
	std::reference_wrapper<AStarNode> tmp_ClosestNode = m_Nodes[0];

	plf::colony<std::reference_wrapper<AStarNode>> tmp_Reachable;


	 //Find nodes that are reachable from arg_location

	for (std::reference_wrapper<AStarNode> for_Node : m_Nodes)
	{
		bool tmp_ReachableStraightLine{ true };
		for (const auto& for_CollisionObject : m_CollisionMap)
		{
			if (detect_LineBoxCollision(for_CollisionObject.location	// Verify an unobstructed line can be drawn between these two nodes
				, Vector2D{ for_CollisionObject.location.x + for_CollisionObject.size.x, for_CollisionObject.location.y + for_CollisionObject.size.y }
				, arg_location
				, for_Node.get().m_WorldLocation) == true)
			{
				tmp_ReachableStraightLine = false;
			}
		}
		if (tmp_ReachableStraightLine == true)
		{
			tmp_Reachable.insert(for_Node);
		}
	}




	// Find the closest reachable nodes
	for (std::reference_wrapper<AStarNode> for_Node : tmp_Reachable)
	{
		auto tmp_Distance = make_RayTrace(for_Node.get().m_WorldLocation, arg_location).LengthSquared();
		if (tmp_Distance < tmp_ShortestDistance)
		{
			tmp_ShortestDistance = tmp_Distance;
			tmp_ClosestNode = for_Node;
		}
	}

	

	return tmp_ClosestNode;

}

std::deque<AStarNode>& AStarGraph::Nodes()
{
	return m_Nodes;
}



std::deque<AStarEdge>& AStarGraph::Edges()
{
	return m_Edges;
}

void AStarGraph::test_GraphInit()
{
	m_Nodes.clear();
	m_Edges.clear();


	add_Node();
	add_Node();
	add_Node();
	add_Node();
	add_Node();
	add_Node();

	add_Edge(v_Node(0), v_Node(3), 1);
	add_Edge(v_Node(3), v_Node(4), 1);
	add_Edge(v_Node(1), v_Node(4), 1);
	add_Edge(v_Node(0), v_Node(1), 1);
	add_Edge(v_Node(1), v_Node(2), 1);
	add_Edge(v_Node(2), v_Node(5), 1);
	add_Edge(v_Node(4), v_Node(5), 1);


	m_Nodes[0].m_WorldLocation = Vector2D(0, -5);
	m_Nodes[1].m_WorldLocation = Vector2D(0, -4);
	m_Nodes[2].m_WorldLocation = Vector2D(0, -3);
	m_Nodes[3].m_WorldLocation = Vector2D(1, -5);
	m_Nodes[4].m_WorldLocation = Vector2D(1, -4);
	m_Nodes[5].m_WorldLocation = Vector2D(1, -3);

}

std::deque<std::reference_wrapper<AStarNode>> AStarGraph::run_GraphPath(std::reference_wrapper<AStarNode> arg_start, std::reference_wrapper<AStarNode> arg_target)
{
	std::chrono::steady_clock::time_point chrono_Function_Start = std::chrono::steady_clock::now();



	auto fun_NotVisited = [](const AStarNode& n) ->bool {return n.m_NotVisited ? true : false; };

//#define ENABLE_ASSERT_VERIFY_NODES_VISITED_STATUS_IS_RESET
#ifdef ENABLE_ASSERT_VERIFY_NODES_VISITED_STATUS_IS_RESET
	auto Assert_AllNotVisited = [fun_NotVisited](const std::deque<AStarNode>& n) ->bool
	{
		for (const auto for_Node : n)
		{
			if (!fun_NotVisited(for_Node))
				return false;
		}
		return true;
	};

	assert(Assert_AllNotVisited(m_Nodes) == true && "AStarGraph::run_GraphPath: SOME NODES NOT RESET");
#endif

	// Set the starting node
	arg_start.get().m_Weight = 0.0;
	arg_start.get().m_NotVisited = false;
	std::reference_wrapper<AStarNode> current_node = arg_start.get();
	size_t tmp_CountVisited{ 0 };
	std::vector<std::reference_wrapper<AStarNode>> tmp_NextNodeBatch;



	// Trying to path between unconnected Nodes = infinite loop
	while (tmp_CountVisited < m_Nodes.size())
	{
		
		// Traverse all edges of the current_node, compute or update the weight of any unvisited nodes

		for (const auto& for_Edge : current_node.get().m_Edges)
		{
			AStarNode& tmp_Neighbor = traverse_Edge(current_node, for_Edge);

			if (tmp_Neighbor.m_NotVisited == true)
			{
				double g_weight = current_node.get().m_Weight + for_Edge.get().m_Weight;
				double h_weight = (current_node.get().m_WorldLocation - tmp_Neighbor.m_WorldLocation).Length();
				double f_weight = (g_weight + h_weight) * const_AStarGreed;

				if (f_weight < tmp_Neighbor.m_Weight)
				{
					tmp_Neighbor.m_Weight = f_weight;
					tmp_Neighbor.m_Predecessor = &current_node.get();
				}
				//MODIF
				tmp_NextNodeBatch.push_back(tmp_Neighbor);

			}
		}


		// Remove visited Nodes
		
		tmp_NextNodeBatch.erase(
			std::remove_if(tmp_NextNodeBatch.begin(), tmp_NextNodeBatch.end(), [](const std::reference_wrapper<AStarNode>& e) {return e.get().m_NotVisited == false; }),
			tmp_NextNodeBatch.end());

		double tmp_LowestWeight{ std::numeric_limits<double>::max() };
		for (std::reference_wrapper<AStarNode> for_Node : tmp_NextNodeBatch | std::views::filter(fun_NotVisited))
		{
			if (for_Node.get().m_Weight < tmp_LowestWeight)
			{
				tmp_LowestWeight = for_Node.get().m_Weight;
				current_node = for_Node;
			}
		}

		// Mark the lowest weight node as visited
		current_node.get().m_NotVisited = false;
		++tmp_CountVisited;

		// If we found the target, reconstruct the path
		if (&current_node.get() == &arg_target.get())
		{

			std::deque<std::reference_wrapper<AStarNode>> tmp_Path;
			auto tmp_Backtrack = arg_target;
			tmp_Path.push_front(tmp_Backtrack);

			while (&tmp_Backtrack.get() != &arg_start.get())
			{
				tmp_Backtrack = *tmp_Backtrack.get().m_Predecessor;
				tmp_Path.push_front(tmp_Backtrack);
			}

			std::chrono::steady_clock::time_point chrono_Function_End = std::chrono::steady_clock::now();
			m_Duration = (chrono_Function_End - chrono_Function_Start).count();

			return tmp_Path;
		}

	}

	// This should never happen.
	std::deque<std::reference_wrapper<AStarNode>> NodeQueue{ std::ref(m_Nodes[0]) };
	return NodeQueue;
}

