#pragma once
// Project
#include "GeometryStructures.h"
#include "Collisions2D.h"

#include <utility>
#include <deque>
#include <vector>
#include <limits>


	class AStarEdge;
	class AStarNode;

	class AStarEdge
	{
	public:
		AStarNode& m_First;
		AStarNode& m_Second;
		double m_Weight;
	};

	class AStarNode
	{
	public:
		AStarNode();
		AStarNode(Vector2D arg_world_location);
		std::vector<std::reference_wrapper<AStarEdge>> m_Edges;
		bool m_NotVisited{ true };
		AStarNode* m_Predecessor{ nullptr };
		double m_Weight{ std::numeric_limits<double>::max() };
		Vector2D m_WorldLocation;
		bool is_Connected(const AStarNode& arg_node);
		void reset();
	};

	class AStarGraph
	{
	public:
		AStarGraph();
		AStarGraph(std::vector<Vector2D> arg_nodes);

		std::reference_wrapper<AStarNode> traverse_Edge(std::reference_wrapper<AStarNode> arg_n, std::reference_wrapper<AStarEdge> arg_e);
		std::deque<std::reference_wrapper<AStarNode>> run_GraphPath(std::reference_wrapper<AStarNode> arg_start, std::reference_wrapper<AStarNode> arg_target);
		
		void add_Node();
		void add_Node(Vector2D arg_node_location);
		void add_Edge(AStarNode& arg_a, AStarNode& arg_b, double arg_weight);
		void set_CollisionMap(const std::vector<SquareCollision> arg_CollisionMap);
		
		std::deque<AStarNode>& Nodes();
		std::deque<AStarEdge>& Edges();

		AStarNode& v_Node(size_t arg_node);

		std::deque<std::reference_wrapper<AStarNode>> request_Path(const Vector2D start, const Vector2D destination);

		std::reference_wrapper<AStarNode> find_ClosestReachableNode(const Vector2D arg_location);

		void test_GraphInit();
	private:

		std::deque<AStarNode> m_Nodes;
		std::deque<AStarEdge> m_Edges;
		std::vector<SquareCollision> m_CollisionMap;
		long long m_Duration{ 0 };
	};
