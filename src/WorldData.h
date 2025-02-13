#pragma once
/* Project */
#include "GeometryStructures.h"
#include "GraphicsStructures.h"
#include "Collisions2D.h"
#include "eFacingDirections.h"
#include "eEntityStateType.h"
#include "eGameEffect.h"
#include "InitDefaults.h"
#include "ActionOffsets.h"
#include "eSpriteRotation.h"
/* STL */
#include <string>
#include <vector>
#include <filesystem>
#include <map>
#include <chrono>
#include <variant>
#include <optional>
/* Libraries */
#include "fmt/core.h"
#include "nlohmann/json.hpp"

// TODO: Load VFX Loop data into Game Effect and Visual effects
// TODO: Find way to inspect directories and files and load Seliel's graphs automatically
// TODO: Creating naming conventions, find ways to load all files automatically
// TODO: Add defensive code to loading to prevent crashes.
// TODO: Add log system to report failures to load and content of data.


void from_json(const nlohmann::json& j, std::filesystem::path& fsp);
struct VisualEffectData
{
	std::string Name{NoInit};
	std::vector<std::string> SpriteNames{ };
	std::map<size_t, std::pair<StateType, FacingDirections>> Sequence;
	std::vector<std::tuple<size_t, StateType, FacingDirections, FacingDirections>> Sequence_;	// Index: State, Facing, SpriteFacing

	void from_json(const nlohmann::json& j, VisualEffectData& ve_data);

	void PrintToCmdLine();
};

struct ActionData
{
	std::string Name{ NoInit };
	std::string GameplayEffectName {NoInit};

	void from_json(const nlohmann::json& j, ActionData& a_data);

	void PrintToCmdLine();
};

struct NavigationPathData
{
	std::string Name{ NoInit };
	std::vector<Vector2D> Waypoints;	// Relative to point of origin

	void from_json(const nlohmann::json& j, NavigationPathData& np_data);

	void PrintToCmdLine();

};

struct NavigationSystemData
{
	std::string Name{ NoInit };
	std::vector<std::string> PathNames{  };

	void from_json(const nlohmann::json& j, NavigationSystemData& ns_data);

	void PrintToCmdLine();

};

struct GameplayEffectData
{
	std::string Name{ NoInit };	
	std::string VisualEffectName{ NoInit };	
	std::string NavigationSystemName{ NoInit }; 
	GameEffect Effect{};
	size_t LifetimeIndex{};
	std::variant<std::chrono::steady_clock::duration, int, bool> Lifetime{};
	double Speed{ 0 };
	int zDepth{ 0 };	

	void from_json(const nlohmann::json& j, GameplayEffectData& ge_data);

	void PrintToCmdLine();
};

struct SpriteData
{
	std::string Name{ "NOT_INITIALIZED" };
	std::string Texture{ "NOT_INITIALIZED"};
	Vector2D CollisionOffset{};
	Vector2D CollisionSize{NoInitNeg, NoInitNeg};
	double CellWidth{ 0 };
	double CellHeight{ 0 };
	Vector2D CellSize{};

	/* Aligned. FrameTimings[0] -> Cells[0] */
	std::map<std::string, std::vector<double>> FrameTimings;
	std::map<std::string, std::vector<Cell>> Cells;
	//std::map<FacingDirections, std::string> Facings;
	//std::map<StateType, std::string> Actions;
	std::map<std::pair<FacingDirections, StateType>, std::string> FacingsActions_Names;	// Allows the Sprite to use a State and Direction to find the appropriate Animation Name
	std::map<std::string, std::pair<SpriteRotation, FacingDirections>> AnimNames_LegalRotationsAndOriginalDirection;	// What rotations are allowed with each of the sprite's animations


	/* Offsets for Visual Effects and start delays for Action's Gameplay Effect start delays */
	std::optional<std::map<GameEffect, ActionOffsets>> SpriteOffsets{std::nullopt};

	void from_json(const nlohmann::json& j, SpriteData& s_data);

	void PrintToCmdLine();

};

struct EntityData
{
	std::string Name{ "NOT_INITIALIZED" };

	Vector2D Size{ 0,0 };	// Used for Collisions
	Vector2D Velocity{ 100,0 };

	void from_json(const nlohmann::json& j, EntityData& e_data);

	void PrintToCmdLine();
};

struct CharacterData
{
	std::string Name{ "NOT_INITIALIZED" };
	std::string SpriteName{ "NOT_INITIALIZED" };
	std::vector<std::string> AvailableActions;

	Vector2D Size{ 0,0 };	// Used for Collisions
	Vector2D MoveSpeed{ 2,0 };
	int ZDepth{ 100 };	// depths 100-199 and 300-399 are reserved for characters and animations

	CollisionType m_CollisionType{ CollisionType::PHYSICAL };

	void from_json(const nlohmann::json& j, CharacterData& c_data);

	void PrintToCmdLine();

};

struct TilesetData
{
	std::string Name{ "NOT_INITIALIZED" };
	std::string Texture{ "NOT_INITIALIZED" };

	Vector2Di TileSize{ 0,0 };
	std::vector<Cell> Tiles;	
	std::vector<SquareCollision> Collisions;
	
	void from_json(const nlohmann::json& j, TilesetData& t_data);

	void PrintToCmdLine();
};

struct AreaData
{
	std::string Name{ "NOT_INITIALIZED" };
	std::string Map{ "NOT_INITIALIZED" };

	/* Character and Entities present in the area and where to spawn them */
	std::map<std::string, Vector2D> Character_SpawnPoint;
	std::map<std::string, Vector2D> Entity_SpawnPoint;
	
	/* Map */
	Vector2D Size; //#Columns and #Rows
	Vector2D WorldSize;	// Size in world location
	std::map<int, std::vector<int>> ZDepth_Layout;	// Depths 0-99 and 200-299 are reserved for tiles
	std::map<int, std::string> FirstTileID_TilesetName;
	std::vector<SquareCollision> CollisionMap;
	std::map<int, SquareCollision> m_NavMesh_Tiles;
	std::vector<std::tuple<Vector2D, int, FacingDirections, int, FacingDirections>>m_NavMesh_Edges; // Edge Location, First_ID, First_FacingDirection, Second_ID, Second_FacingDirection
	std::vector<Vector2D> PathFindingNodes;
	std::vector<std::vector<Vector2D>> m_GeneratedPathfindingNodes;

	void from_json(const nlohmann::json& j, AreaData& a_data);
	void map_from_json(const nlohmann::json& j, AreaData& a_data);

	void PrintToCmdLine();
};

struct WorldData
{

	/* Filepaths */
	std::map<std::string, std::filesystem::path> Action_Filepath;
	std::map<std::string, std::filesystem::path> Area_Filepath;
	std::map<std::string, std::filesystem::path> Character_Filepath;
	std::map<std::string, std::filesystem::path> Entity_Filepath;
	std::map<std::string, std::filesystem::path> GameplayEffect_Filepath;
	std::map<std::string, std::filesystem::path> Map_Filepath;
	std::map<std::string, std::filesystem::path> NavigationSystem_Filepath;
	std::map<std::string, std::filesystem::path> NavigationPath_Filepath;
	std::map<std::string, std::filesystem::path> Sprite_Filepath;
	std::map<std::string, std::filesystem::path> Tileset_Filepath;
	std::map<std::string, std::filesystem::path> Texture_Filepath;
	std::map<std::string, std::filesystem::path> VisualEffect_Filepath;
	
	/* Data */
	std::map<std::string, ActionData> ActionName_Action;
	std::map<std::string, AreaData> AreaName_Area;
	std::map<std::string, CharacterData> CharacterName_Character;
	std::map<std::string, EntityData> EntityName_Entity;
	std::map<std::string, GameplayEffectData> GameplayEffectName_GameplayEffect;
	std::map<std::string, NavigationPathData> NavigationPathName_NavigationPath;
	std::map<std::string, NavigationSystemData> NavigationSystemName_NavigationSystem;
	std::map<std::string, SpriteData> SpriteName_Sprite;
	std::map<std::string, TilesetData> TilesetName_Tileset;
	std::map<std::string, VisualEffectData> VisualEffectName_VisualEffect;

	
	/* Note: actual graphics data (textures) load orders are sent from the Data Manager */
	void from_json(const nlohmann::json& j, WorldData& w_data);

	std::map<std::string, std::filesystem::path> loadFilepathsFromJson(const nlohmann::json& j, const std::string search_term);

	void PrintToCmdLine();
};

std::vector<std::vector<Vector2D>> build_PathfindingNodesNetwork(std::vector<SquareCollision> arg_collision_map, Vector2D arg_worldsize);