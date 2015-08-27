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
		A_MESH,
		A_SPAWN_POSITION,
		A_PATROL_POINT,
		NUM_ATTRIBUTE
	};

	const string ROOT_NAME = "LevelContainer";
	const string ENEMY_BRANCH_NAME = "EnemyContainer";

	const string ATTRIBUTE_NAMES[NUM_ATTRIBUTE] =
	{
		"TileMapFilePath",
		"TileSize",
		"NumMapTile",
		"MaxTimer",
		"MissionType",
		"Mesh",
		"SpawnPosition",
		"PatrolPoint"
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
				//setting the objective ptr to the correct objective type
				switch (m_missionType)
				{
				case LM_COLLECT:
				{
								   m__objective = new ObjectiveCollect;
								   break;
				}
				case LM_PLANT_BOMB:
				{
									  m__objective = new ObjectiveSetBomb;
									  break;
				}
				case LM_DEFUSE_BOMB:
				{
									   m__objective = new ObjectiveDefuse;
									   break;
				}
				case LM_ASSASSINATE:
				{
									   m__objective = new ObjectiveAssassinate;
									   break;
				}
				}
			}
		}

		// Load the TileMap
		InitMap(numMapTile, viewWidth, viewHeight, tileSize, tileMapFilePath, meshList);

		// For Each Branch
		for (vector<Branch>::iterator branchIT = level.childBranches.begin(); branchIT != level.childBranches.end(); ++branchIT)
		{
			Branch branch = *branchIT;

			if (branch.name == ENEMY_BRANCH_NAME)
			{
				// For each enemy's branch
				for (vector<Branch>::iterator enemyBranchIT = branch.childBranches.begin(); enemyBranchIT != branch.childBranches.end(); ++enemyBranchIT)
				{
					Branch enemyBranch = *enemyBranchIT;
					
					Enemy* _enemy = new Enemy;
					_enemy->Init(_enemy->GetMapPos(), NULL);
					_enemy->SetScale(Vector2(tileSize, tileSize));
					
					// For each of the enemy's attribute
					for (vector<Attribute>::iterator enemyAttribIT = enemyBranch.attributes.begin(); enemyAttribIT != enemyBranch.attributes.end(); ++enemyAttribIT)
					{
						Attribute attrib = *enemyAttribIT;

						if (attrib.name == ATTRIBUTE_NAMES[A_SPAWN_POSITION])
						{
							vector<float> values = StringToFloats(attrib.value);

							if (values.size() >= 2)			// If the size of the vector is greater or equal to 2 for the 2 values for positioning
							{
								_enemy->SetMapPosition(Vector2(values[0], values[1]), m__map->GetScrollOffset(), tileSize);
							}
						}
						else if (attrib.name == ATTRIBUTE_NAMES[A_MESH])
						{
							for (vector<Mesh*>::iterator meshIT = meshList.begin(); meshIT != meshList.end(); ++meshIT)
							{
								Mesh* mesh = *meshIT;

								if (mesh->name == attrib.value)
								{
									_enemy->SetMesh(mesh);
									break;
								}
							}
						}
						else if (attrib.name == ATTRIBUTE_NAMES[A_PATROL_POINT])
						{
							vector<float> values = StringToFloats(attrib.value);

							if (values.size() >= 2)			// If the size of the vector is greater or equal to 2 for the 2 values for positioning
							{
								// Multiply by tile size to convert from tile coord to map coord
								_enemy->AddPatrolPoint(Vector2(values[0] * tileSize, values[1] * tileSize));
							}							
						}
					}

					// Add the enemy to the enemy list
					m_enemyList.push_back(_enemy);
				}
			}
		}		
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

void Level::ActivateObjective(void)
{
	m__objective->Activate();
}

bool Level::GetObjectiveComplete(void) const
{
	return m__objective->IsCompleted();
}

void Level::Clear(void)
{
	if (m__map)
	{
		delete m__map;
		m__map = NULL;
	}

	if (m__objective != NULL)
	{
		delete m__objective;
	}

	while (m_enemyList.size() > 0)
	{
		if (m_enemyList.back() != NULL)
		{
			delete m_enemyList.back();
			m_enemyList.pop_back();
		}
	}	
}

TileMap* Level::GetTileMap()
{
	return m__map;
}

vector<Enemy*> Level::GetEnemyList(void)
{
	return m_enemyList;
}
