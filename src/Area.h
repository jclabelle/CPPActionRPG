#pragma once
/* Project */
#include "Entity.h"

#include "WorldData.h"
#include "GeometryStructures.h"
#include "GraphicsRendererPayloads.h"
#include "AStar.h"
#include "classes_Pathfinding.h"
/* STL */
#include <string>
#include <vector>
#include <memory>
/* Libraries */
#include "fmt/core.h"


class Area
{
public:
	Area();
	Area(AreaData new_area_data, std::vector<Entity> new_entity_population);

	// Initialisation
	void init_Pathfinding();
	void init_AI();
	std::vector<Entity>& Entity_Population();
	Navmesh& Navmesh_();

	/* View and Set */
	void setPlayerCharacter(Entity* new_player_character);
	const Vector2D& v_DefaultSpawnPoint();

	/* Additional Viewers */
	SquareCollision make_Camera(const Entity& arg_Focus);

	/* Frame logic */
	std::vector< std::reference_wrapper<Entity>> run_FrameLogic_V2(const std::tuple<EntityEvent, EntityEvent>& arg_CommandsMovementAction);
	void run_CollisionsCheck(Entity* arg_Entity, std::vector<std::reference_wrapper<GameplayEffect>>& arg_CollidingEffects, std::vector<std::reference_wrapper<Entity>>& arg_CollidingPopulation);
	void run_PerceptionsCheck(Entity* arg_Entity, std::vector<std::reference_wrapper<Entity>>& arg_PerceivablePopulation);
	std::vector<FrameVisibleAnimation> list_VisibleAnimations(SquareCollision arg_Camera);
	void run_FrameCleanup(std::vector<std::reference_wrapper<Entity>>& arg_FullPop);

	

	/* Test functions */
	void printToCmdLine();
	
private:
	std::vector<Entity> EntityPopulation{ 0 };
	std::vector<SquareCollision> CollisionMap;
	AStarGraph m_Pathfinding;
	Entity* PlayerCharacter{};
	AreaData m_AreaData;
	std::unique_ptr<Navmesh> m_Navmesh{nullptr};

	/* Map */
	Vector2D WorldSize; // World in pixel-size
	std::string m_Name{ "NOT INITIALIZED" };
	Vector2D DefaultSpawnpoint{ 160,560 };

	// Methods
	bool run_ReachableCheck(uint64_t arg_entity_ID, const Vector2D& arg_entity_world_location, uint64_t arg_perceived_ID, const Vector2D& arg_perceived_world_location, std::vector< std::reference_wrapper<Entity>>& arg_PerceivablePopulation) const;
	std::vector<std::vector<Vector2D>> build_PathfindingNodesNetwork();
	void build_PathfindingEdges_ConnectAllNodes();
	void make_PathfindingFromWorldData();
	void build_PathfindingEdges_ConnectNeighbors();


	// Population Filter Methods
	 std::vector< std::reference_wrapper<Entity>> list_VisiblePopulation() ;

	 std::vector<std::reference_wrapper<Entity>> list_FullPopulation();

	 std::vector<std::reference_wrapper<Entity>> list_CollisionPopulation();

	 std::vector<std::reference_wrapper<GameplayEffect>> list_VisibleEffects();

	 std::vector<std::reference_wrapper<Entity>> list_PerceivingPopulation();

	 std::vector<std::reference_wrapper<GameplayEffect>> list_ActiveEffects();

	 std::vector<std::reference_wrapper<GameplayEffect>> list_CollisionEffects();

	 std::vector<std::reference_wrapper<Entity>> list_PerceivablePopulation();

};


template<class T>
std::vector<std::reference_wrapper<T>> FilterView__(T* Plus, std::vector<T>* From)
{
	std::vector<std::reference_wrapper<T>> Filtered;

	std::reference_wrapper<T> ref = std::ref(*Plus);

	Filtered.push_back(ref);

	return Filtered;
}