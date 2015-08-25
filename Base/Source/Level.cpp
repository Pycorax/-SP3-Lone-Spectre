#include "Level.h"

#include "SONIO.h"
#include "LoadResource.h"

Level::Level(void)
{
}

Level::~Level(void)
{
}

void Level::Load(string levelFilePath, int viewWidth, int viewHeight, vector<Mesh*>& meshList)
{
	enum ATTRIBUTE_TYPE
	{
		A_TILEMAP_FILE_PATH,
		A_TILE_SIZE,
		A_NUM_MAP_TILE,
		A_MAX_TIMER,
		A_MISSION_TYPE,
		NUM_ATTRIBUTE
	};

	const string ROOT_NAME = "LevelContainer";

	const string ATTRIBUTE_NAMES[NUM_ATTRIBUTE] =
	{
		"TileMapFilePath",
		"TileSize",
		"NumMapTile",
		"MaxTimer",
		"MissionType"
	};

	const string MISSION_NAMES[NUM_LEVEL_MISSIONS] =
	{
		"LM_COLLECT",
		"LM_ASSASSINATE",
		"LM_DEFUSE_BOMB",
		"LM_PLANT_BOMB"
	};

	// Variables for Initialization
	string tileMapFilePath;
	int tileSize;
	Vector2 numMapTile;
	
	Branch level = SONIO::LoadSON(levelFilePath);

	if (level.name == ROOT_NAME)
	{
		// For each attribute
		for (vector<Attribute>::iterator attribIter = level.attributes.begin(); attribIter != level.attributes.end(); ++attribIter)
		{
			Attribute attrib = *attribIter;

			if (attrib.name == ATTRIBUTE_NAMES[A_TILEMAP_FILE_PATH])
			{
				tileMapFilePath = attrib.value;
			}
			else if (attrib.name == ATTRIBUTE_NAMES[A_TILE_SIZE])
			{
				tileSize = stoi(attrib.value);
			}
			else if (attrib.name == ATTRIBUTE_NAMES[A_NUM_MAP_TILE])
			{
				vector<float> values = StringToFloats(attrib.value);

				if (values.size() == 2)
				{
					numMapTile.Set(values[0], values[1]);
				}
			}
			else if (attrib.name == ATTRIBUTE_NAMES[A_MAX_TIMER])
			{
				m_levelMaxTime = stod(attrib.value);
			}
			else if (attrib.name == ATTRIBUTE_NAMES[A_MISSION_TYPE])
			{
				for (size_t missionType = 0; missionType < NUM_LEVEL_MISSIONS; ++missionType)
				{
					if (attrib.value == MISSION_NAMES[missionType])
					{
						m_missionType = static_cast<LEVEL_MISSION_TYPE>(missionType);
					}
				}
			}
		}

		// Load the TileMap
		InitMap(numMapTile, viewWidth, viewHeight, tileSize, tileMapFilePath, meshList);
	}
}

void Level::InitMap(Vector2 numMapTile, int viewWidth, int viewHeight, float tileSize, string filePath, vector<Mesh*> &meshList)
{
	Vector2 numScreenTile(ceil(viewWidth / 64.f), ceil(viewHeight / 64.f));
	m__map = new TileMap(numMapTile, numScreenTile, tileSize);
	m__map->LoadTileMap(filePath, meshList);
}

void Level::Update(double dt)
{

}

void Level::Clear(void)
{
	if (m__map)
	{
		delete m__map;
		m__map = NULL;
	}
}

TileMap* Level::GetTileMap()
{
	return m__map;
}