//#define DEBUG_PRINT
/* Project */
#include "Area.h"
#include "Collisions2D.h"
#include "GraphicsRendererPayloads.h"
#include "AIConstants.h"
#include "functions_Pathfinding.h"
/* STL */
#include <memory>
#include <cassert>
#include <ranges>
/* Libraries */
#include "fmt/core.h"

Area::Area() 
{
	std::cout << "\nArea Default Constructor\n";
}

Area::Area(AreaData new_area_data, std::vector<Entity> new_entity_population)
	:m_AreaData{new_area_data}
	,m_Name{new_area_data.Name}
	,EntityPopulation{new_entity_population}
	,WorldSize{new_area_data.WorldSize}
	,CollisionMap{new_area_data.CollisionMap}
	,m_Pathfinding{new_area_data.PathFindingNodes}
	,m_Navmesh{make_Navmesh(new_area_data.m_NavMesh_Tiles, new_area_data.m_NavMesh_Edges)}
{}

void Area::init_Pathfinding()
{
	m_Pathfinding.set_CollisionMap(CollisionMap);
	m_Pathfinding.Nodes().clear();

	auto tmp_AStar_Nodes_For_This_Map = build_PathfindingNodesNetwork();

	for (const auto& for_NodesVector : tmp_AStar_Nodes_For_This_Map)
	{
		for (const auto& for_Node : for_NodesVector)
		{
			m_Pathfinding.add_Node(for_Node);
		}
	}


	for (AStarNode& for_Node : m_Pathfinding.Nodes())
	{
		auto SkipSelf = [&for_Node](const AStarNode& n)	// Used to filter for_Node if it comes up as for_TestNodeEdge
		{
			return &n != &for_Node;
		};

		for (AStarNode& for_TestNodeEdge : m_Pathfinding.Nodes() | std::views::filter(SkipSelf))
		{
			if (for_TestNodeEdge.is_Connected(for_Node) == false)	// If these nodes are not already connected
			{
				// If these nodes are neighbors, aka within less than the NodeFrequency Distance
				if ((make_RayTrace(for_TestNodeEdge.m_WorldLocation, for_Node.m_WorldLocation)).Length() <= (Vector2D(tmp_NodeFrequency_X, tmp_NodeFrequency_Y).Length() + 0.0001))
				{
					auto tmp_Distance = (make_RayTrace(for_TestNodeEdge.m_WorldLocation, for_Node.m_WorldLocation)).Length();	// Get the heuristic (=distance)
					m_Pathfinding.add_Edge(for_Node, for_TestNodeEdge, tmp_Distance);	// Create a Edge
				}
			}
		}
	}


	for (auto& for_Entity : EntityPopulation)
	{
		for_Entity.AI().init_Pathfinding(m_Pathfinding);
	}

}
void Area::init_AI()
{
	for (auto& for_Entity : EntityPopulation)
	{
		for_Entity.initAI();
	}
}

std::vector<Entity>& Area::Entity_Population()
{
	return EntityPopulation;
}

Navmesh& Area::Navmesh_()
{
	return *m_Navmesh;
}

// Not in use
void Area::build_PathfindingEdges_ConnectNeighbors()
{
	for (AStarNode& for_Node : m_Pathfinding.Nodes())
	{
		auto SkipSelf = [&for_Node](const AStarNode& n)	// Used to filter for_Node if it comes up as for_TestNodeEdge
		{
			return &n != &for_Node;
		};

		for (AStarNode& for_TestNodeEdge : m_Pathfinding.Nodes() | std::views::filter(SkipSelf))
		{
			if (for_TestNodeEdge.is_Connected(for_Node) == false)	// If these nodes are not already connected
			{
				// If these nodes are neighbors, aka within less than the NodeFrequency Distance
				if ((make_RayTrace(for_TestNodeEdge.m_WorldLocation, for_Node.m_WorldLocation)).Length() <= (Vector2D(tmp_NodeFrequency_X, tmp_NodeFrequency_Y).Length() + 0.0001))
				{
					auto tmp_Distance = (make_RayTrace(for_TestNodeEdge.m_WorldLocation, for_Node.m_WorldLocation)).Length();	// Get the heuristic (=distance)
					m_Pathfinding.add_Edge(for_Node, for_TestNodeEdge, tmp_Distance);	// Create a Edge
				}
			}
		}
	}
}

// Not in use
void Area::build_PathfindingEdges_ConnectAllNodes()
{
	for (AStarNode& for_Node : m_Pathfinding.Nodes())
	{
		auto SkipSelf = [&for_Node](const AStarNode& n)
		{
			return &n != &for_Node;
		};

		for (AStarNode& for_TestNodeEdge : m_Pathfinding.Nodes() | std::views::filter(SkipSelf))
		{
			if (for_TestNodeEdge.is_Connected(for_Node) == false)	// If these nodes are not already connected
			{
				bool tmp_NoObstructionBetweenNodes{ true };
				for (const auto& for_CollisionObject : CollisionMap)
				{
					if (detect_LineBoxCollision(for_CollisionObject.location	// Verify an unobstructed line can be drawn between these two nodes
						, Vector2D{ for_CollisionObject.location.x + for_CollisionObject.size.x, for_CollisionObject.location.y + for_CollisionObject.size.y }
						, for_Node.m_WorldLocation
						, for_TestNodeEdge.m_WorldLocation) == true)
					{
						tmp_NoObstructionBetweenNodes = false;	// Don't create a Edge if a collision is detected
					}
				}

				if (tmp_NoObstructionBetweenNodes == true)	// Create a Edge if no collisions are detected
				{
					auto tmp_Distance = (make_RayTrace(for_TestNodeEdge.m_WorldLocation, for_Node.m_WorldLocation)).Length();	// Get the heuristic (=distance)
					m_Pathfinding.add_Edge(for_Node, for_TestNodeEdge, tmp_Distance);	// Create a Edge
				}

			}
		}
	}
}

void Area::make_PathfindingFromWorldData()
{
	// pathfinding node location = center of m_NavMesh vector elements
	// pathfinding edge weight = distance between m_NavMesh centers
}


void Area::setPlayerCharacter(Entity* new_player_character)
{
	PlayerCharacter = new_player_character;
	PlayerCharacter->init_Physics(CollisionType::PHYSICAL, v_DefaultSpawnPoint(), PlayerCharacter->v_SpriteCollisionSize(), WorldSize);
}

const Vector2D& Area::v_DefaultSpawnPoint()
{
	return DefaultSpawnpoint;
}

SquareCollision Area::make_Camera(const Entity& arg_Focus)
{
	SquareCollision Camera;
	auto CameraWidth = static_cast<double>(ScaledScreenWidth);
	auto CameraHeight = static_cast<double>(ScaledScreenHeight);

	Camera.location.x = (arg_Focus.v_WorldLocation().x - CameraWidth / 2);
	Camera.location.y = (arg_Focus.v_WorldLocation().y - CameraHeight / 2);
	
	// Upper left limit assumed to be 0,0
	// Adjust if the camera moves out of the provided limits
	if (Camera.location.x > WorldSize.x - CameraWidth)
		Camera.location.x = WorldSize.x - CameraWidth;

	if (Camera.location.y > WorldSize.y - CameraHeight)
		Camera.location.y = WorldSize.y - CameraHeight;

	if (Camera.location.x < 0)
		Camera.location.x = 0;

	if (Camera.location.y < 0)
		Camera.location.y = 0;

	Camera.size.x = CameraWidth;
	Camera.size.y = CameraHeight;
	return Camera;
}

std::vector< std::reference_wrapper<Entity>> Area::run_FrameLogic_V2(const std::tuple<EntityEvent, EntityEvent>& arg_CommandsMovementAction)
{
	// References must remain valid until end of scope
	auto FullPop = list_FullPopulation();
	auto PerceivingPop = list_PerceivingPopulation();
	auto PerceivablePop = list_PerceivablePopulation();
	auto CollidingPop = list_CollisionPopulation();
	auto CollidingEffects = list_CollisionEffects();

	// Update Player's current Navmesh tile
	for (auto& for_Navmesh : m_Navmesh.get()->m_Tiles)
	{
		if (detect_Collisions(PlayerCharacter->v_Collision(), for_Navmesh.m_Tile))
		{
			PlayerCharacter->m_Gameplay.m_Player_Current_Navmesh_Tile = &for_Navmesh;
		}
	}

	// Run AI Perception, generate or refresh Perception data
	for (auto for_Entity : PerceivingPop)
	{
		run_PerceptionsCheck(&for_Entity.get(), PerceivablePop);
	}

	// Refresh m_IsReachable of PerceptionData
	for (auto for_Entity : PerceivingPop)
	{
		for (auto& for_Perceived : for_Entity.get().AI().Perception().PerceivedPopulation())
			for_Perceived.second.m_IsReachable = run_ReachableCheck(for_Entity.get().GlobalID, for_Entity.get().v_WorldLocation(), for_Perceived.first, for_Perceived.second.m_WorldLocation, PerceivablePop);
	}

	
	// Iterate through all entities */
	for (auto AnEntity : FullPop)
	{
		// Run the entity's game play logic */
		AnEntity.get().run_FrameLogic_V2(arg_CommandsMovementAction);

		 // Check Collisions
		if (AnEntity.get().has_Collisions() == true)
		{
			run_CollisionsCheck(&AnEntity.get(), CollidingEffects, CollidingPop);
		}
	}

	return FullPop;
}

void Area::run_CollisionsCheck(Entity* arg_Entity
	, std::vector<std::reference_wrapper<GameplayEffect>>& arg_CollidingEffects
	, std::vector<std::reference_wrapper<Entity>>& arg_CollidingPopulation)
{
	for (auto for_Effect : arg_CollidingEffects)
	{
		if (arg_Entity->is_Colliding(for_Effect.get().viewCollisions()) == true)
		{
			//std::cout << "COLLIDER COLLIDER!!\n";
		}
	}

	/* Check the Player Character's collisions against Physical Collision objects */
	if(arg_Entity == PlayerCharacter )
	{
		for (auto& CollidingObj : CollisionMap)
		{
			arg_Entity->is_Colliding(CollidingObj);
		}

		for (auto& for_CollidingEnt : arg_CollidingPopulation)
		{
			if (for_CollidingEnt.get().GlobalID != arg_Entity->GlobalID)
			{
				arg_Entity->is_Colliding(for_CollidingEnt.get().v_Collision());
			}
		}
	}

	/* Check collisions against all Entities */
	for (auto CollidingEnt : arg_CollidingPopulation)
	{
		/* Don't check against the entity itself or the player character  */
		if (CollidingEnt.get().GlobalID != arg_Entity->GlobalID && CollidingEnt.get().GlobalID != PlayerCharacter->GlobalID)
		{
			if (arg_Entity->is_Colliding(CollidingEnt.get().v_Collision()))
			{
				if (arg_Entity->has_AI() && CollidingEnt.get().has_AI())
				{
					arg_Entity->AI().register_CollisionWithEntity(true, CollidingEnt.get().Physics().get().get_CollisionsReference(), CollidingEnt.get().AI());
				}
			}
		}
	}
}

void Area::run_PerceptionsCheck(Entity* arg_Entity, std::vector<std::reference_wrapper<Entity>>& arg_PerceivablePopulation)
{	
	// Reset the statuses of Perceived's Sight_Detected and Hearing_Detected
	arg_Entity->AI().Perception().reset_Detected_SH();

	// Iterate throughn the perceivable population, test for perception and give references to perceived entities to the arg_Entity
	for (auto for_Entity : arg_PerceivablePopulation)
	{	
		// Don't run perception against yourself
		if(for_Entity.get().GlobalID != arg_Entity->GlobalID)
		{	
			auto tmp_Perception_SH = arg_Entity->AI().Perception().is_Perceived(for_Entity.get().v_Collision(), for_Entity.get().v_WorldLocation());
			if(tmp_Perception_SH.first==SenseType::SIGHT || tmp_Perception_SH.second==SenseType::HEARING)
			{
				// Register the detected Entity
				arg_Entity->AI().Perception().register_Entity(for_Entity.get().v_WorldLocation()
					, for_Entity.get().Physics().get().v_Collisions()
					, for_Entity.get().v_Gameplay().v_Stats().v_Faction()
					, for_Entity.get().v_State().v_Sprite_Facing()
					, for_Entity.get().m_Gameplay
					, for_Entity.get().GlobalID
					, tmp_Perception_SH.first
					, tmp_Perception_SH.second);
			}
		}
	}
}

std::vector<FrameVisibleAnimation> Area::list_VisibleAnimations(SquareCollision arg_Camera)
{
	#define DRAW_PHYSICS_DEBUG

	std::vector<FrameVisibleAnimation> VisibleAnimations;
	Vector2D CamLocation = arg_Camera.location;
	Vector2D CamSize = arg_Camera.size;

	/* Check the Population for visibility */
	for (auto forEntity : list_VisiblePopulation())
	{
		if (checkCollisionAxisAligned(CamLocation, CamSize, forEntity.get().v_SpriteLocation(), forEntity.get().v_SpriteSize()) == true)
		{
			FrameVisibleAnimation Visible{
				  forEntity.get().a_Sprite()->viewName()
				, forEntity.get().a_Sprite()->viewActiveAnimationName()
				, forEntity.get().a_Sprite()->viewActiveAnimationFrameID()
				, forEntity.get().a_ZDepth()
				, forEntity.get().v_SpriteLocation() };
			//std::cout<< forEntity.get().viewSpriteLocation().x << "x "<< forEntity.get().viewSpriteLocation().y<<"y \n";	
#ifdef DRAW_PHYSICS_DEBUG
			Visible.Size = forEntity.get().Physics().get().v_Collisions().size; // Used to draw collisions during debug
			Visible.CollisionLocation = forEntity.get().Physics().get().v_Collisions().location;;	// Used to draw collisions during debug
			Visible.WorldLocation = forEntity.get().Physics().get().v_WorldLocation();
			if (forEntity.get().has_AI() == true)
			{
				Visible.SightCollisions_WorldLocation = forEntity.get().AI().Perception().debug_v_SightCollision().location;
				Visible.SightCollisions_Size = forEntity.get().AI().Perception().debug_v_SightCollision().size;
				Visible.m_debug_LineToNearestEnemy.first = forEntity.get().AI().debug_LineToNearestEnemy().first;
				Visible.m_debug_LineToNearestEnemy.second = forEntity.get().AI().debug_LineToNearestEnemy().second;
				Visible.m_debug_CurrentPath = forEntity.get().AI().debug_CurrentPath();
			}
#endif
			VisibleAnimations.push_back(Visible);
		}
	}

	/* Check Effects for visibility */
	for (auto GameplayEffect : list_VisibleEffects())
	{
		if (checkCollisionAxisAligned(CamLocation, CamSize, GameplayEffect.get().v_SpriteLocation(), GameplayEffect.get().viewSpriteSize()) == true)
		{
			FrameVisibleAnimation Visible{ 
				  GameplayEffect.get().viewSpriteName()
				, GameplayEffect.get().viewAnimationName()
				, GameplayEffect.get().viewAnimationID()
				, GameplayEffect.get().viewZDepth()
				, GameplayEffect.get().v_SpriteLocation() };
#ifdef DRAW_PHYSICS_DEBUG
			Visible.Size = GameplayEffect.get().viewCollisions().size; // Used to draw collisions during debug
			Visible.CollisionLocation = GameplayEffect.get().viewCollisions().location;	// Used to draw collisions during debug
			Visible.ImageRotation = GameplayEffect.get().findImageRotation();
#endif

			VisibleAnimations.push_back(Visible);
		}
	}
	return VisibleAnimations;
}

void Area::run_FrameCleanup(std::vector<std::reference_wrapper<Entity>>& arg_FullPop)
{
	for (auto for_Entity : arg_FullPop)
	{
		for_Entity.get().run_FrameCleanup();
	}
}


void Area::printToCmdLine()
{
	std::cout << fmt::format("[Area]\nName: {}, Size: [w{}, h{}]\n", m_Name, WorldSize.x, WorldSize.y);

	std::cout << "-Entities-\n";
	for (auto AreaEntity : EntityPopulation)
	{
		std::cout << fmt::format("Name: {}, Location: [x{}, y{}]\n", AreaEntity.v_Name(), AreaEntity.v_WorldLocation().x, AreaEntity.v_WorldLocation().y);
	}
	std::cout << '\n';
}


#pragma warning (disable: 4100)
bool Area::run_ReachableCheck(uint64_t arg_entity_ID, const Vector2D& arg_entity_world_location, uint64_t arg_perceived_ID, const Vector2D& arg_perceived_world_location, std::vector< std::reference_wrapper<Entity>>& arg_PerceivablePopulation) const
{
	for (const auto& for_CollisionObject : CollisionMap)
	{
		if (detect_LineBoxCollision(for_CollisionObject.location
			, Vector2D{ for_CollisionObject.location.x + for_CollisionObject.size.x, for_CollisionObject.location.y + for_CollisionObject.size.y }
			, arg_entity_world_location
			, arg_perceived_world_location))
			return false;
	}

	return true;
}

std::vector<std::vector<Vector2D>> Area::build_PathfindingNodesNetwork()
{
	size_t tmp_Nodes_On_X = static_cast<size_t>(std::floor(WorldSize.x / tmp_NodeFrequency_X));
	size_t tmp_Nodes_On_Y = static_cast<size_t>(std::floor(WorldSize.y / tmp_NodeFrequency_Y));

	std::vector<std::vector<Vector2D>> tmp_World_Nodes(tmp_Nodes_On_Y, std::vector<Vector2D>(tmp_Nodes_On_X));


	// Make a copy of the collision map
	// Extend the size of each collisions by DistanceBuffer on each sides.

	auto tmp_CollisionsWithDistanceBuffer = CollisionMap;

	for (auto& for_Collision : tmp_CollisionsWithDistanceBuffer)
	{
		for_Collision.location.x -= tmp_DistanceBuffer_X;
		for_Collision.location.y -= tmp_DistanceBuffer_Y;
		for_Collision.size.x += tmp_DistanceBuffer_X * 2;
		for_Collision.size.y += tmp_DistanceBuffer_Y * 2;
	}

	// Create collisions at each of the 4 sides of the world so that the Area's limits have the same padding as collisions
	tmp_CollisionsWithDistanceBuffer.push_back(SquareCollision{ CollisionType::PHYSICAL, Vector2D{0,0}, Vector2D{WorldSize.x, tmp_DistanceBuffer_Y} });					// Up limit
	tmp_CollisionsWithDistanceBuffer.push_back(SquareCollision{ CollisionType::PHYSICAL, Vector2D{0,0}, Vector2D{tmp_DistanceBuffer_X, WorldSize.y} });					// Left limit
	tmp_CollisionsWithDistanceBuffer.push_back(SquareCollision{ CollisionType::PHYSICAL, Vector2D{0,WorldSize.y- tmp_DistanceBuffer_Y}, Vector2D{WorldSize.x, tmp_DistanceBuffer_Y} });	// Bottom limit
	tmp_CollisionsWithDistanceBuffer.push_back(SquareCollision{ CollisionType::PHYSICAL, Vector2D{WorldSize.x-tmp_DistanceBuffer_X,0}, Vector2D{tmp_DistanceBuffer_X, WorldSize.y} });  // Right limit

	// Go through all Rows 
	double tmp_Increment_Location_X{ 0 };
	double tmp_Increment_Location_Y{ 0 };
	for (auto& for_Y_Row_Vector : tmp_World_Nodes)
	{
		// Go through all Nodes of the row
		tmp_Increment_Location_X = 0;
		for (auto& for_Node : for_Y_Row_Vector)
		{
			// Run a Collison check for the potential location of the node (Increment_Locations X and Y) against the map of collisions
			bool tmp_CollisionStatus{ false };
			for (const auto& for_CollisionBox : tmp_CollisionsWithDistanceBuffer)
			{
				// If a collision is detected, set CollisionStatus to true
				if (detect_Collisions(SquareCollision{ CollisionType::PHYSICAL, Vector2D{tmp_Increment_Location_X, tmp_Increment_Location_Y}, Vector2D{1,1} }, for_CollisionBox) == true)
				{
					tmp_CollisionStatus = true;
					break;
				}
			}
			// If no collisions were detected, set the Node's location to the current Increment Locations
			if (tmp_CollisionStatus == false)
			{
				for_Node.x = tmp_Increment_Location_X;
				for_Node.y = tmp_Increment_Location_Y;
			}
			// If a collision is detected, set the Node's location to -1 so we can destroy it later
			else
			{
				for_Node.x = -1;
				for_Node.y = -1;
			}

			// After checking a Node, increment the X by NodeFrequency
			tmp_Increment_Location_X += tmp_NodeFrequency_X;
		}
			// After checking a Row, increment the Y by NodeFrequency
			tmp_Increment_Location_Y += tmp_NodeFrequency_Y;
	}

	// Get rid of nodes that would be inside a Collision or within a distance <10
	for (auto& for_NodeVec : tmp_World_Nodes)
	{
		for_NodeVec.erase(
		std::remove_if(for_NodeVec.begin(), for_NodeVec.end(), [](const Vector2D& v) {return v.x < 0; }),
			for_NodeVec.end());
	}

	return tmp_World_Nodes;
}



// Access the Visible Population + the player character
 std::vector< std::reference_wrapper< Entity>> Area::list_VisiblePopulation() 
{
	 constexpr auto EntityVisible = [](const Entity& e) { return e.is_Visible(); };
	 std::vector< std::reference_wrapper<Entity>> r_Population;

	for (Entity& forEntity : EntityPopulation | std::views::filter(EntityVisible))
	{
		 std::reference_wrapper<Entity> ref = std::ref(forEntity);
		r_Population.push_back(ref);
	}

	r_Population.push_back(std::reference_wrapper<Entity>(*PlayerCharacter));
	return r_Population;
}

 // Access the Entity Population + the Player Character
 std::vector< std::reference_wrapper<Entity>> Area::list_FullPopulation()
 {
	 std::vector< std::reference_wrapper< Entity>> r_Population;

	 for (Entity& forEntity : EntityPopulation)
	 {
		 std::reference_wrapper< Entity> ref = std::ref(forEntity);
		 r_Population.push_back(ref);
	 }

	 r_Population.push_back(std::reference_wrapper<Entity>(*PlayerCharacter));
	 return r_Population;
 }

 // Access the Population with collisions + the player character
 std::vector< std::reference_wrapper<Entity>> Area::list_CollisionPopulation()
 {
	 constexpr auto EntityCollider = [](const Entity& e) { return e.has_Collisions(); };
	 std::vector< std::reference_wrapper< Entity>> r_Population;

	 for (Entity& forEntity : EntityPopulation | std::views::filter(EntityCollider))
	 {
		 std::reference_wrapper< Entity> ref = std::ref(forEntity);
		 r_Population.push_back(ref);
	 }

	 r_Population.push_back(std::reference_wrapper<Entity>(*PlayerCharacter));
	 return r_Population;
 }

 // Access all visible effects in the level
 std::vector<std::reference_wrapper<GameplayEffect>> Area::list_VisibleEffects()
 {
	 constexpr auto EffectVisible = [](const GameplayEffect& e) { return e.isVisible(); };

	 std::vector<std::reference_wrapper<GameplayEffect>> ret_VisibleEffects;

	 for (auto for_Entity : list_FullPopulation())
	 {
		 for (GameplayEffect& for_Effect : for_Entity.get().a_GameplayManager().accessOwnedEffects() | std::views::filter(EffectVisible))
		 {
			 std::reference_wrapper<GameplayEffect> ref = std::ref(for_Effect);
			 ret_VisibleEffects.push_back(ref);
		 }
	 }
	 return ret_VisibleEffects;
 }

 // Access the Population with Perception in the level
 std::vector<std::reference_wrapper<Entity>> Area::list_PerceivingPopulation()
 {
	 constexpr auto EntityPerception = [](const Entity& e) { return e.v_AI().has_Perception(); };
	 std::vector< std::reference_wrapper< Entity>> r_Population;

	 for (Entity& forEntity : EntityPopulation | std::views::filter(EntityPerception))
	 {
		 std::reference_wrapper< Entity> ref = std::ref(forEntity);
		 r_Population.push_back(ref);
	 }

	 return r_Population;
 }

 // Access all active effects in the level
 std::vector<std::reference_wrapper<GameplayEffect>> Area::list_ActiveEffects()
 {
	 constexpr auto EffectActive = [](const GameplayEffect& e) { return e.isActive(); };

	 std::vector<std::reference_wrapper<GameplayEffect>> ret_ActiveEffects;

	 for (auto for_Entity : list_FullPopulation())
	 {
		 for (GameplayEffect& for_Effect : for_Entity.get().a_GameplayManager().accessOwnedEffects() | std::views::filter(EffectActive))
		 {
			 std::reference_wrapper<GameplayEffect> ref = std::ref(for_Effect);
			 ret_ActiveEffects.push_back(ref);
		 }
	 }
	 return ret_ActiveEffects;
 }

 // Access all active effects with collisions
 std::vector<std::reference_wrapper<GameplayEffect>> Area::list_CollisionEffects()
 {
	constexpr auto EffectActive = [](const GameplayEffect& e) { return e.isActive(); };
	constexpr auto EffectCollider = [](const GameplayEffect& e) { return e.hasCollisionTrigger(); };

	std::vector<std::reference_wrapper<GameplayEffect>> ret_CollidingEffects;

	 for (auto for_Entity : list_FullPopulation())
	 {
		 for (GameplayEffect& for_Effect : for_Entity.get().a_GameplayManager().accessOwnedEffects() | std::views::filter(EffectActive) | std::views::filter(EffectCollider) )
		 {
			 std::reference_wrapper<GameplayEffect> ref = std::ref(for_Effect);
			 ret_CollidingEffects.push_back(ref);
		 }
	 }
	 return ret_CollidingEffects;
 }

 std::vector<std::reference_wrapper<Entity>> Area::list_PerceivablePopulation()
 {
	 constexpr auto EntityPerceivable = [](const Entity& e) { return e.v_Gameplay().v_Stats().is_Perceivable(); };
	 std::vector< std::reference_wrapper< Entity>> r_Population;
	 
	for (Entity& forEntity : EntityPopulation | std::views::filter(EntityPerceivable))
	{
		std::reference_wrapper< Entity> ref = std::ref(forEntity);
		r_Population.push_back(ref);
	}
	if (EntityPerceivable(*PlayerCharacter) == true)
		r_Population.push_back(*PlayerCharacter);
	 
	 return r_Population;
 }
