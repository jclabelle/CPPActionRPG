#define DEBUG_PRINT
//#define DEBUG_PRINT_MAP_LAYERS
/* Project */
#include "WorldData.h"
#include "PAS_JsonUtilities.h"
#include "TextManipulationFunctions.h"
#include "GeometryStructures.h"
#include "AIConstants.h"
/* STL */
#include <memory>
#include <string>
#include <optional>
#include <iostream>
/* Libraries */
#include "fmt/ranges.h"

void from_json(const nlohmann::json& j, std::filesystem::path& fsp)
{
	fsp = j.at("file").get<std::string>();
}

void WorldData::from_json(const nlohmann::json& j, WorldData& w_data)
{
	/* Access the [WorldName]_World_MasterFile.json and extract the filepaths
	to MasterFiles of all assets.
	The MasterFiles reside in the root of the Assets/ directory, along with
	the World MasterFile */
	nlohmann::json jActions_MasterFile = loadJson(std::string(j.at("actions")));
	nlohmann::json jAreas_MasterFile = loadJson(std::string(j.at("areas")));
	nlohmann::json jCharacters_MasterFile = loadJson(std::string(j.at("characters")));
	nlohmann::json jEntities_MasterFile = loadJson(std::string(j.at("entities")));
	nlohmann::json jGameplayEffects_MasterFile = loadJson(std::string(j.at("gameplayeffects")));
	nlohmann::json jMaps_MasterFile = loadJson(std::string(j.at("maps")));
	nlohmann::json jNavigationPaths_MasterFile = loadJson(std::string(j.at("navigationpaths")));
	nlohmann::json jNavigationSystems_MasterFile = loadJson(std::string(j.at("navigationsystems")));
	nlohmann::json jSprites_MasterFile = loadJson(std::string(j.at("sprites")));
	nlohmann::json jTextures_MasterFile = loadJson(std::string(j.at("textures")));
	nlohmann::json jTilesets_MasterFile = loadJson(std::string(j.at("tilesets")));
	nlohmann::json jVisualEffects_MasterFile = loadJson(std::string(j.at("visualeffects")));
	

	/* Access each MasterFile and load the final files list.
	Areas, Maps, Sprites and Tiletsets have their own individual Json files.
	Actions, GameplayEffects, Navigation Systems/Paths, Visual Effects, Characters and Entities 
	are grouped together in Jsons by categories. Textures are listed together in a single Json.*/
	w_data.Action_Filepath = loadFilepathsFromJson(jActions_MasterFile, "actions");
	w_data.Area_Filepath = loadFilepathsFromJson(jAreas_MasterFile, "areas");
	w_data.Character_Filepath = loadFilepathsFromJson(jCharacters_MasterFile, "characters");
	w_data.Entity_Filepath = loadFilepathsFromJson(jEntities_MasterFile, "entities");
	w_data.GameplayEffect_Filepath = loadFilepathsFromJson(jGameplayEffects_MasterFile, "gameplayeffects");
	w_data.Map_Filepath = loadFilepathsFromJson(jMaps_MasterFile, "maps");
	w_data.NavigationPath_Filepath = loadFilepathsFromJson(jNavigationPaths_MasterFile, "navigationpaths");
	w_data.NavigationSystem_Filepath = loadFilepathsFromJson(jNavigationSystems_MasterFile, "navigationsystems");
	w_data.Sprite_Filepath = loadFilepathsFromJson(jSprites_MasterFile, "sprites");
	w_data.Texture_Filepath = loadFilepathsFromJson(jTextures_MasterFile, "textures");
	w_data.Tileset_Filepath = loadFilepathsFromJson(jTilesets_MasterFile, "tilesets");
	w_data.VisualEffect_Filepath = loadFilepathsFromJson(jVisualEffects_MasterFile, "visualeffects");
	std::cout << "\n";

	/* Data Loading functions */

	for (auto myPair : w_data.GameplayEffect_Filepath)
	{
		nlohmann::json Category = loadJson(myPair.second);

		for (auto jData : Category.at("gameplayeffects"))
		{
			GameplayEffectData Data;
			Data.from_json(jData, Data);
			w_data.GameplayEffectName_GameplayEffect.insert(std::pair(Data.Name, Data));
		}
	}

	for (auto myPair : w_data.NavigationSystem_Filepath)
	{
		nlohmann::json jNavigationSystemCategory = loadJson(myPair.second);

		for (auto jNavigationSystem : jNavigationSystemCategory.at("navigationsystems"))
		{
			NavigationSystemData Data;
			Data.from_json(jNavigationSystem, Data);
			w_data.NavigationSystemName_NavigationSystem.insert(std::pair(Data.Name, Data));
		}
	}

	for (auto myPair : w_data.NavigationPath_Filepath)
	{
		nlohmann::json jNavigationPathCategory = loadJson(myPair.second);

		for (auto jNavigationPath : jNavigationPathCategory.at("navigationpaths"))
		{
			NavigationPathData Data;
			Data.from_json(jNavigationPath, Data);
			w_data.NavigationPathName_NavigationPath.insert(std::pair(Data.Name, Data));
		}
	}

	for (auto myPair : w_data.VisualEffect_Filepath)
	{
		nlohmann::json Category = loadJson(myPair.second);

		for (auto jData : Category.at("visualeffects"))
		{
			VisualEffectData Data;
			Data.from_json(jData, Data);
			w_data.VisualEffectName_VisualEffect.insert(std::pair(Data.Name, Data));
		}
	}

	for (auto myPair : w_data.Action_Filepath)
	{
		nlohmann::json Category = loadJson(myPair.second);

		for (auto jData : Category.at("actions"))
		{
			ActionData Data;
			Data.from_json(jData, Data);
			w_data.ActionName_Action.insert(std::pair(Data.Name, Data));
		}
	}

	for (auto myPair : w_data.Sprite_Filepath)
	{
		SpriteData Data;
		nlohmann::json jData = loadJson(myPair.second);
		Data.Name = myPair.first;
		Data.from_json(jData, Data);
		w_data.SpriteName_Sprite.insert(std::pair(myPair.first, Data));
	}

	for (auto myPair : w_data.Tileset_Filepath)
	{
		TilesetData Data;
		nlohmann::json jData = loadJson(myPair.second);
		Data.Name = myPair.first;
		Data.from_json(jData, Data);
		w_data.TilesetName_Tileset.insert(std::pair(myPair.first, Data));
	}

	for (auto myPair : w_data.Entity_Filepath)
	{
		nlohmann::json jEntityCategory = loadJson(myPair.second);

		for (auto jEntity: jEntityCategory.at("entities"))
		{
			EntityData Data;
			Data.from_json(jEntity, Data);
			w_data.EntityName_Entity.insert(std::pair(Data.Name, Data));
		}
	}

	for (auto myPair : w_data.Character_Filepath)
	{
		nlohmann::json jCharacterCategory = loadJson(myPair.second);

		for (auto jCharacter : jCharacterCategory.at("characters"))
		{
			CharacterData Data;
			Data.from_json(jCharacter, Data);
			w_data.CharacterName_Character.insert(std::pair(Data.Name, Data));
		}
	}

	for (auto myPair : w_data.Area_Filepath)
	{
		AreaData Data;
		nlohmann::json jArea = loadJson(myPair.second);
		
		Data.Name = myPair.first;
		Data.from_json(jArea, Data);

		auto MapFilePath = w_data.Map_Filepath.at(Data.Map);
		nlohmann::json jMap = loadJson(MapFilePath);
		Data.map_from_json(jMap, Data);

		w_data.AreaName_Area.insert(std::pair(myPair.first, Data));
	}
}

std::map<std::string, std::filesystem::path> WorldData::loadFilepathsFromJson(const nlohmann::json& j, const std::string search_term)
{
	std::map<std::string, std::filesystem::path> returnMap;
	for (auto Path : j.at(search_term))
	{
#ifdef DEBUG_PRINT
		std::cout<<Path<<'\n';
#endif
		std::string name = Path.at("name");
		std::filesystem::path file = std::string(Path.at("file"));
		returnMap.insert(std::make_pair(name, file));
	}
	return returnMap;
}

void WorldData::PrintToCmdLine()
{
	std::cout<<"\n\n-----Begin Printing Data for World-----\n\n";
	for (auto Data : EntityName_Entity)
	{
		Data.second.PrintToCmdLine();
	}

	std::cout << "\n";
	for (auto Data : CharacterName_Character)
	{
		Data.second.PrintToCmdLine();
	}

	std::cout << "\n";
	for (auto Data : SpriteName_Sprite)
	{
		Data.second.PrintToCmdLine();
	}
	std::cout << "\n";
	for (auto Data : TilesetName_Tileset)
	{
		Data.second.PrintToCmdLine();
	}
	std::cout << "\n";
	for (auto Data : AreaName_Area)
	{
		Data.second.PrintToCmdLine();
	}

	std::cout << "\n";
	for (auto Data : VisualEffectName_VisualEffect)
	{
		Data.second.PrintToCmdLine();
	}

	std::cout << "\n";
	for (auto Data : ActionName_Action)
	{
		Data.second.PrintToCmdLine();
	}


	std::cout << "\n";
	for (auto Data : NavigationPathName_NavigationPath)
	{
		Data.second.PrintToCmdLine();
	}

	std::cout << "\n";
	for (auto Data : NavigationSystemName_NavigationSystem)
	{
		Data.second.PrintToCmdLine();
	}

	std::cout << "\n";
	for (auto Data : GameplayEffectName_GameplayEffect)
	{
		Data.second.PrintToCmdLine();
	}
}

void AreaData::from_json(const nlohmann::json& j, AreaData& a_data)
{
	j.at("map").get_to(a_data.Map);
	
	for (auto jEntity : j.at("areaentities"))
	{
		std::string EntityName = jEntity.at("name").get<std::string>();
		double SpawnPointX = jEntity.at("spawnpoint").at(0);
		double SpawnPointY = jEntity.at("spawnpoint").at(1);
		Vector2D Spawn{ SpawnPointX, SpawnPointY };
		a_data.Entity_SpawnPoint.insert(std::make_pair(EntityName, Spawn));
	}

	for (auto jCharacter : j.at("areacharacters"))
	{
		std::string CharacterName = jCharacter.at("name").get<std::string>();
		double SpawnPointX = jCharacter.at("spawnpoint").at(0);
		double SpawnPointY = jCharacter.at("spawnpoint").at(1);
		Vector2D Spawn{ SpawnPointX, SpawnPointY };
		a_data.Character_SpawnPoint.insert(std::make_pair(CharacterName, Spawn));
	}
}

void AreaData::map_from_json(const nlohmann::json& j, AreaData& a_data)
{
	j.at("width").get_to(a_data.Size.x);
	j.at("height").get_to(a_data.Size.y);
	j.at("tileheight").get_to(a_data.WorldSize.x);
	j.at("tilewidth").get_to(a_data.WorldSize.y);
	a_data.WorldSize.x = a_data.WorldSize.x * a_data.Size.x;
	a_data.WorldSize.y = a_data.WorldSize.y * a_data.Size.y;

	for (auto jTileset : j.at("tilesets"))
	{
		 std::filesystem::path TilesetPath = std::string(jTileset.at("source"));
		 std::string TilesetName = TilesetPath.filename().string();
		 TilesetName = getNameFromFileName(TilesetName);
		 //std::cout << TilesetName << '\n';
		 int FirstTile = jTileset.at("firstgid");
		 a_data.FirstTileID_TilesetName.insert(std::pair(FirstTile, TilesetName));
	}

	for (auto jLayer : j.at("layers"))
	{
		/* If the layer is a tile layer */
		if (jLayer.at("type").get<std::string>() == std::string("tilelayer"))
		{
			int LayerDepth = jLayer.at("properties").at(0).at("value");
			std::vector<int> LayerData = jLayer.at("data");
			a_data.ZDepth_Layout.insert(std::make_pair(LayerDepth, LayerData));
		}
		// If the layer type is an objectgroup
		if (jLayer.at("type").get<std::string>() == std::string("objectgroup"))
		{
			auto Objects = jLayer.at("objects");
			for (auto AnObject : Objects)
				//TODO Deprecated, remove.
			{	// If this is a collision object 
				if (AnObject.at("type").get<std::string>() == std::string("collision"))
				{
					SquareCollision NewSquareCollision;
					NewSquareCollision.type = CollisionType::PHYSICAL;
					NewSquareCollision.size.x = AnObject.at("width").get<double>();
					NewSquareCollision.size.y = AnObject.at("height").get<double>();
					NewSquareCollision.location.x = AnObject.at("x").get<double>();
					NewSquareCollision.location.y = AnObject.at("y").get<double>();
					CollisionMap.push_back(NewSquareCollision);
				}
				//TODO Deprecated, remove.
				// If this is a pathfinding object 
				if (AnObject.at("type").get<std::string>() == std::string("pathfinding"))
				{
					double tmp_Location_X = AnObject.at("x").get<double>();
					double tmp_Location_Y = AnObject.at("y").get<double>();		
					PathFindingNodes.push_back(Vector2D{ tmp_Location_X, tmp_Location_Y });
				}
			}
		}	

		// If this is a Collision layer
		if (jLayer.at("type").get<std::string>() == std::string("objectgroup") && jLayer.at("name").get<std::string>() == std::string("Collisions"))
		{
			auto Objects = jLayer.at("objects");
			for (auto AnObject : Objects)
			{	
				SquareCollision NewSquareCollision;
				NewSquareCollision.type = CollisionType::PHYSICAL;
				NewSquareCollision.size.x = AnObject.at("width").get<double>();
				NewSquareCollision.size.y = AnObject.at("height").get<double>();
				NewSquareCollision.location.x = AnObject.at("x").get<double>();
				NewSquareCollision.location.y = AnObject.at("y").get<double>();
				CollisionMap.push_back(NewSquareCollision);
			}
		}

		// If this is a Navmesh layer
		if (jLayer.at("type").get<std::string>() == std::string("objectgroup") && jLayer.at("name").get<std::string>() == std::string("NavMesh"))
		{
			auto Objects = jLayer.at("objects");
			for (auto AnObject : Objects)
			{
				SquareCollision NewSquareCollision;
				NewSquareCollision.type = CollisionType::NAVMESH;
				NewSquareCollision.size.x = AnObject.at("width").get<double>();
				NewSquareCollision.size.y = AnObject.at("height").get<double>();
				NewSquareCollision.location.x = AnObject.at("x").get<double>();
				NewSquareCollision.location.y = AnObject.at("y").get<double>();
				m_NavMesh_Tiles.insert(std::pair(std::stoi(std::string(AnObject.at("name"))), NewSquareCollision));
			}
		}

		// If this is a Navmesh Edges layer
		if (jLayer.at("type").get<std::string>() == std::string("objectgroup") && jLayer.at("name").get<std::string>() == std::string("NavMesh_Edges"))
		{
			auto Objects = jLayer.at("objects");
			for (auto AnObject : Objects)
			{
				m_NavMesh_Edges.push_back
				(
					std::tuple
					(
					Vector2D ( AnObject.at("x").get<double>(), AnObject.at("y").get<double>() )
					, AnObject.at("properties").at(0).at("value").get<int>()
					, AnObject.at("properties").at(1).at("value").get<FacingDirections>()
					, AnObject.at("properties").at(2).at("value").get<int>()
					, AnObject.at("properties").at(3).at("value").get<FacingDirections>()
					)
				);
			}
		}
	}

	a_data.m_GeneratedPathfindingNodes = build_PathfindingNodesNetwork(CollisionMap, WorldSize);
}


void AreaData::PrintToCmdLine()
{
	std::cout << fmt::format("[Area]\nName: {}, Map: {}, Size: [w{}, h{}]\n", Name, Map, Size.x, Size.y);
	
	std::cout << "-Entities-\n";
	for (auto MapEntity : Entity_SpawnPoint)
	{
		std::cout << fmt::format("Name: {}, SpawnPoint: [x{}, y{}]\n", MapEntity.first, MapEntity.second.x, MapEntity.second.y);
	}

	std::cout << "-Characters-\n";
	for (auto MapCharacter : Character_SpawnPoint)
	{
		std::cout << fmt::format("Name: {}, SpawnPoint: [x{}, y{}]\n", MapCharacter.first, MapCharacter.second.x, MapCharacter.second.y);
	}

	std::cout << "-Tilesets-\n";
	for (auto TileID : FirstTileID_TilesetName)
	{
		std::cout << fmt::format("Tileset: {} starts at ID# {}\n", TileID.first, TileID.second);
	}

	std::cout << "-Layers-\n";
	for (auto Layer : ZDepth_Layout)
	{
		std::cout << fmt::format("Layer at ZDepth: {}\n", Layer.first);
#ifdef DEBUG_PRINT_MAP_LAYERS
		for (auto ID : Layer.second)
		{
			std::cout << ID << " ";
		}
		std::cout << '\n';
#endif
	}
	std::cout << '\n';
}

void TilesetData::from_json(const nlohmann::json& j, TilesetData& t_data)
{
	j.at("name").get_to(t_data.Texture);
	j.at("tilewidth").get_to(t_data.TileSize.x);
	j.at("tileheight").get_to(t_data.TileSize.y);

	/* Temp values to ease the generation loops */
	int TileW = t_data.TileSize.x;
	int TileH = t_data.TileSize.y;
	int ImageW = j.at("imagewidth");
	int ImageH = j.at("imageheight");
	//int TileCount = j.at("tilecount");
	int ColumnsTotal = j.at("columns");

	/* Calculate RowsTotal */
	int PixelsRowSurfaceSize = ColumnsTotal * TileW * TileH;
	int RowsTotal = (ImageW * ImageH) / (PixelsRowSurfaceSize);

	for (int RowItr = 0; RowItr < RowsTotal; ++RowItr)
	{
		for (int ColumnItr = 0; ColumnItr < ColumnsTotal; ++ColumnItr)
		{
			Cell NextCell{ (ColumnItr * TileW), (RowItr * TileH),TileW,TileH };
			t_data.Tiles.push_back(NextCell);
		}
	}
}

void TilesetData::PrintToCmdLine()
{
	std::cout << fmt::format("[Tileset]\nName: {}, Texture: {}, Tilesize: [x{},y{}], Tiles count: {}\n", Name, Texture,
		TileSize.x, TileSize.y, Tiles.size());
}

void EntityData::from_json(const nlohmann::json& j, EntityData& e_data)
{
	j.at("name").get_to(e_data.Name);
	j.at("dimensions").at(0).get_to(e_data.Size.x);
	j.at("dimensions").at(1).get_to(e_data.Size.y);
	j.at("velocity").at(0).get_to(e_data.Velocity.x);
	j.at("velocity").at(1).get_to(e_data.Velocity.y);
}

void EntityData::PrintToCmdLine()
{
	std::cout << fmt::format("[Entity]\tName: {}, Dimensions: {},{}, Velocity: {},{}\n",
		Name, Size.x, Size.y, Velocity.x, Velocity.y);
}

void SpriteData::from_json(const nlohmann::json& j, SpriteData& s_data)
{
	j.at("imagefile").get_to(s_data.Texture);
	j.at("collisionsize_x").get_to(s_data.CollisionSize.x);
	j.at("collisionsize_y").get_to(s_data.CollisionSize.y);
	j.at("collisionoffset_x").get_to(s_data.CollisionOffset.x);
	j.at("collisionoffset_y").get_to(s_data.CollisionOffset.y);

	int tempCellWidth = j.at("cellwidth");
	int tempCellHeight = j.at("cellheight");
	int tempCounter{ 0 };
	CellWidth = static_cast<double>(tempCellWidth);
	CellHeight = static_cast<double>(tempCellHeight);
	CellSize = Vector2D{ CellWidth, CellHeight };

	for (auto AnimData : j.at("animations"))
	{
		std::string name = AnimData.at("name");
		std::vector<double> timings = AnimData.at("frametimes");
		FacingDirections facing = AnimData.at("facing");
		StateType action = AnimData.at("type");
		std::pair<FacingDirections, StateType> FacingAction{ facing, action };
		SpriteRotation legalrotation = AnimData.at("legalrotations");
		s_data.AnimNames_LegalRotationsAndOriginalDirection.insert(std::pair(name, std::pair(legalrotation, facing)));
		std::vector<Cell> cvec;

		for (size_t itr = 0; itr < AnimData.at("cellcount"); ++itr)
		{
			Cell c;
			c.x = int(itr) * tempCellWidth;
			c.y = tempCounter * tempCellHeight;
			c.w = tempCellWidth;
			c.h = tempCellHeight;
			cvec.push_back(c);
		}
		/* asserts if value of cellcount does not match size of timings - 
		there are either too many or too few frametimes in the data files*/
		const size_t CellCount = AnimData.at("cellcount");
		assert(CellCount == timings.size());

		s_data.FrameTimings.insert(std::pair(name, timings));
		s_data.Cells.insert(std::pair(name, cvec));
		s_data.FacingsActions_Names.insert(std::pair(FacingAction, name));

		++tempCounter;
	}

	if (j.find("gameeffectsoffsets") != j.end())
	{
		std::map<GameEffect, ActionOffsets> NewOffsets;
		for (auto Offset : j.at("gameeffectsoffsets"))
		{
			std::map<FacingDirections, Vector2D> DirectionOffsets;
			DirectionOffsets.insert(std::pair(FacingDirections::UP, Vector2D{ 0, Offset.at("up") }));
			DirectionOffsets.insert(std::pair(FacingDirections::DOWN, Vector2D{ 0 , Offset.at("down") }));
			DirectionOffsets.insert(std::pair(FacingDirections::LEFT, Vector2D{ Offset.at("left"), 0 }));
			DirectionOffsets.insert(std::pair(FacingDirections::RIGHT, Vector2D{ Offset.at("right"), 0 }));
			ActionOffsets NewOffset{ DirectionOffsets, std::chrono::milliseconds(Offset.at("timedelay")) };
			NewOffsets.insert(std::pair(GameEffect{ Offset.at("gameeffecttype") }, NewOffset));
		}
		s_data.SpriteOffsets = NewOffsets;
	}
}

void SpriteData::PrintToCmdLine()
{
	std::cout<< fmt::format("[Sprite]\nName: {}\nTexture: {}\n", Name, Texture);

	assert(FrameTimings.size() == Cells.size() && "SpriteData Print Failed: sizes of Frametiming and Cells do not match");

	auto DoubleAccumulator = [](const std::vector<double>& VecD) ->double 
	{
		double SumDoubles{};
		for (auto f : VecD)
		{
			SumDoubles += f;
		}
		return SumDoubles;
	};

	for (auto Frametime : FrameTimings)
	{
		std::string ID = Frametime.first;
		double Duration = DoubleAccumulator(Frametime.second);
		double x = Cells.at(Frametime.first).data()->x;
		double y = Cells.at(Frametime.first).data()->y;
		double w = Cells.at(Frametime.first).data()->w;
		double h = Cells.at(Frametime.first).data()->h;

		std::cout << fmt::format("Name: {}, Duration: {}, Cell: [x{}, y{}, w{}, h{}]\n", ID, Duration, x, y, w, h);
	}
}

void CharacterData::from_json(const nlohmann::json& j, CharacterData& c_data)
{
	j.at("name").get_to(c_data.Name);
	j.at("dimensions").at(0).get_to(c_data.Size.x);
	j.at("dimensions").at(1).get_to(c_data.Size.y);
	j.at("velocity").at(0).get_to(c_data.MoveSpeed.x);
	j.at("velocity").at(1).get_to(c_data.MoveSpeed.y);
	j.at("sprite").get_to(c_data.SpriteName);
	j.at("zdepth").get_to(c_data.ZDepth);
	AvailableActions = j.at("actions").get<std::vector<std::string>>();
}

void CharacterData::PrintToCmdLine()
{
	std::cout << fmt::format("[Character]\tName: {}, Dimensions: {},{}, Velocity: {},{}, Sprite: {}, ZDepth:{}\n",
		Name, Size.x, Size.y, MoveSpeed.x, MoveSpeed.y, SpriteName, ZDepth);
}

void ActionData::from_json(const nlohmann::json& j, ActionData& a_data)
{
		j.at("name").get_to(a_data.Name);
		j.at("gameplayeffect").get_to(a_data.GameplayEffectName);
}

void ActionData::PrintToCmdLine()
{
	std::cout << fmt::format("[Action]\tName: {}, GameplayEffect: {}\n", Name, GameplayEffectName);
}

void VisualEffectData::from_json(const nlohmann::json& j, VisualEffectData& ve_data)
{

		j.at("name").get_to(ve_data.Name);
		SpriteNames = j.at("sprites").get<std::vector<std::string>>();

		for (auto Seq : j.at("sequence"))
		{
			//size_t& SpriteIndex= Seq.at("spriteindex");
			ve_data.Sequence_.push_back(std::tuple(size_t(Seq.at("spriteindex")), StateType(Seq.at("statetype")), FacingDirections(Seq.at("facing")), FacingDirections(Seq.at("spriteanimationfacing"))));
		}
}

void VisualEffectData::PrintToCmdLine()
{
	std::cout << fmt::format("[Visual Effect]\nName: {}\n", Name);
	fmt::print("Sprites: {} \n", fmt::join(SpriteNames, ", "));
	fmt::print("StateSequence: {}\n", fmt::join(Sequence, ", "));
}

void NavigationPathData::from_json(const nlohmann::json& j, NavigationPathData& np_data)
{
		j.at("name").get_to(np_data.Name);
		for (auto Waypoint : j.at("waypoints"))
		{
			np_data.Waypoints.push_back(Vector2D(Waypoint.at("x").get<double>(), Waypoint.at("y").get<double>()));
		}
}

void NavigationPathData::PrintToCmdLine()
{
	std::cout << fmt::format("[Navigation Path]\tName: {}\t", Name);

	fmt::print("Waypoints: ");

	for (auto Point : Waypoints)
	{
		fmt::print("(x{}, y{})", Point.x, Point.y);
	}
	std::cout << "\n";
}

void NavigationSystemData::from_json(const nlohmann::json& j, NavigationSystemData& ns_data)
{
		j.at("name").get_to(ns_data.Name);
		for (auto PathName : j.at("pathnames"))
		{
			ns_data.PathNames.push_back(PathName.get<std::string>());
		}
}

void NavigationSystemData::PrintToCmdLine()
{
	std::cout << fmt::format("[Navigation System]\tName: {}\t", Name);
	fmt::print("Paths: {} \n", fmt::join(PathNames, ", "));
}

void GameplayEffectData::from_json(const nlohmann::json& j, GameplayEffectData& ge_data)
{

		j.at("name").get_to(ge_data.Name);
		j.at("speed").get_to(ge_data.Speed);
		j.at("zdepth").get_to(ge_data.zDepth);
		j.at("gameeffect").get_to(ge_data.Effect);
		j.at("visualeffect").get_to(ge_data.VisualEffectName);
		j.at("navigationsystem").get_to(ge_data.NavigationSystemName);
		j.at("lifetimeindex").get_to(ge_data.LifetimeIndex);

		if (LifetimeIndex == 0)
			Lifetime = std::chrono::steady_clock::duration(j.at("lifetime").get<long long>());

		if (LifetimeIndex == 1)
			Lifetime.emplace<1>(j.at("lifetime").get<int>());
		
		if (LifetimeIndex == 2)
			Lifetime.emplace<2>(j.at("lifetime").get<bool>());
		
}

void GameplayEffectData::PrintToCmdLine()
{
	std::cout << fmt::format("[Gameplay Effect]\tName: {}, Visual Effect: {}, Navigation System: {}, GameEffect: {}, Speed : {}, zDepth : {}\n"
		, Name, VisualEffectName, NavigationSystemName, Effect, Speed, zDepth);
}



std::vector<std::vector<Vector2D>> build_PathfindingNodesNetwork(std::vector<SquareCollision> arg_collision_map, Vector2D arg_worldsize)
{


	//double tmp_NodeFrequency_X{ 50 };
	//double tmp_NodeFrequency_Y{ 50 };
	//double tmp_DistanceBuffer_X{ 10 };
	//double tmp_DistanceBuffer_Y{ 10 };
	size_t tmp_Nodes_On_X = static_cast<size_t>(std::floor(arg_worldsize.x / tmp_NodeFrequency_X));
	size_t tmp_Nodes_On_Y = static_cast<size_t>(std::floor(arg_worldsize.y / tmp_NodeFrequency_Y));

	std::vector<std::vector<Vector2D>> tmp_World_Nodes(tmp_Nodes_On_Y, std::vector<Vector2D>(tmp_Nodes_On_X));


	// Make a copy of the collision map
	// Extend the size of each collisions by DistanceBuffer on each sides.

	auto tmp_CollisionsWithDistanceBuffer = arg_collision_map;

	for (auto& for_Collision : tmp_CollisionsWithDistanceBuffer)
	{
		for_Collision.location.x -= tmp_DistanceBuffer_X;
		for_Collision.location.y -= tmp_DistanceBuffer_Y;
		for_Collision.size.x += tmp_DistanceBuffer_X * 2;
		for_Collision.size.y += tmp_DistanceBuffer_Y * 2;
	}

	// Create collisions at each of the 4 sides of the world so that the Area's limits have the same padding as collisions
	tmp_CollisionsWithDistanceBuffer.push_back(SquareCollision{ CollisionType::PHYSICAL, Vector2D{0,0}, Vector2D{arg_worldsize.x, tmp_DistanceBuffer_Y} });					// Up limit
	tmp_CollisionsWithDistanceBuffer.push_back(SquareCollision{ CollisionType::PHYSICAL, Vector2D{0,0}, Vector2D{tmp_DistanceBuffer_X, arg_worldsize.y} });					// Left limit
	tmp_CollisionsWithDistanceBuffer.push_back(SquareCollision{ CollisionType::PHYSICAL, Vector2D{0,arg_worldsize.y - tmp_DistanceBuffer_Y}, Vector2D{arg_worldsize.x, tmp_DistanceBuffer_Y} });	// Bottom limit
	tmp_CollisionsWithDistanceBuffer.push_back(SquareCollision{ CollisionType::PHYSICAL, Vector2D{arg_worldsize.x - tmp_DistanceBuffer_X,0}, Vector2D{tmp_DistanceBuffer_X, arg_worldsize.y} });  // Right limit

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
