#include "Level.h"

#include "SONIO.h"
#include "LoadResource.h"

#include "ObjectiveAssassinate.h"
#include "ObjectiveCollect.h"
#include "ObjectiveDefuse.h"
#include "ObjectiveSetBomb.h"
#include "ObjectiveHostage.h"
#include "ObjectiveEscape.h"

const Vector2 Level::S_M_SCREEN_SIZE(20, 12);

Level::Level(void) : m__map(NULL)
{
}

Level::~Level(void)
{
}

void Level::Load(string levelFilePath, int viewWidth, int viewHeight, vector<Mesh*>& meshList, Vector2 numScreenTile)
{
	enum ATTRIBUTE_TYPE
	{
		A_TILEMAP_FILE_PATH,
		A_TILE_SIZE,
		A_NUM_MAP_TILE,
		A_MAX_TIMER,
		A_MISSION_TYPE,
		A_OBJECTIVE_ACTIVE_TIMER,
		A_BOMB_TIMER,
		A_MESSAGES_FILE_PATH,
		A_BGM_NAME,
		// -- For Enemies
		A_MESH,
		A_NPC_TYPE,
		A_SPAWN_POSITION,
		A_VIEW_RANGE,
		A_SPAWN_TILE_POSITION,
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
		"ObjectiveActiveTimer",
		"BombTimer",
		"MessagesFilePath",
		"BackgroundMusic",
		// -- For Enemies
		"Mesh",
		"NPCType",
		"SpawnPosition",
		"ViewRange",
		"SpawnTilePosition",
		"PatrolPoint"
	};

	const string MISSION_NAMES[NUM_LEVEL_MISSIONS] =
	{
		"LM_ESCAPE",
		"LM_COLLECT",
		"LM_ASSASSINATE",
		"LM_DEFUSE_BOMB",
		"LM_PLANT_BOMB",
		"LM_HOSTAGE"
	};

	const string NPC_TYPE_NAMES[NPC::NUM_NPC_TYPE] =
	{
		"NT_ENEMY",
		"NT_HOSTAGE",
		"NT_TARGET"
	};

	// Variables for Initialization
	string tileMapFilePath;
	int tileSize;
	Vector2 numMapTile;
	double m_defuseTime = 0;
	double m_BombTime = 0;
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
					case LM_ESCAPE:
						{
							m__objective = new ObjectiveEscape;
							break;
						}
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
					case LM_HOSTAGE:
						{
							m__objective = new ObjectiveHostage;
							break;
						}
				}
			}
			else if (attrib.name == ATTRIBUTE_NAMES[A_MESSAGES_FILE_PATH])
			{
				m_messagesFile = attrib.value;
			}
			else if (attrib.name == ATTRIBUTE_NAMES[A_BGM_NAME])
			{
				m_bgmName = attrib.value;
			}
			else if(attrib.name == ATTRIBUTE_NAMES[A_OBJECTIVE_ACTIVE_TIMER])
			{
				m_defuseTime = stod(attrib.value);
			}
			else if(attrib.name == ATTRIBUTE_NAMES[A_BOMB_TIMER])
			{
				m_BombTime = stod(attrib.value);
				dynamic_cast<ObjectiveDefuse* >(m__objective)->Init(m_defuseTime , m_BombTime);
			}
		}

		// Load the TileMap
		/*if (numScreenTile == Vector2::ZERO_VECTOR)
		{
			numScreenTile = Vector2(viewWidth / tileSize, viewHeight / tileSize);
		}*/
		InitMap(numMapTile, viewWidth, viewHeight, tileSize, tileMapFilePath, meshList, S_M_SCREEN_SIZE);

		// For Each Branch
		tileSize = m__map->GetTileSize();
		for (vector<Branch>::iterator branchIT = level.childBranches.begin(); branchIT != level.childBranches.end(); ++branchIT)
		{
			Branch branch = *branchIT;

			if (branch.name == ENEMY_BRANCH_NAME)
			{
				// For each enemy's branch
				for (vector<Branch>::iterator enemyBranchIT = branch.childBranches.begin(); enemyBranchIT != branch.childBranches.end(); ++enemyBranchIT)
				{
					Branch enemyBranch = *enemyBranchIT;
					
					NPC* _enemy = new NPC;
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
						else if (attrib.name == ATTRIBUTE_NAMES[A_SPAWN_TILE_POSITION])
						{
							vector<float> values = StringToFloats(attrib.value);

							if (values.size() >= 2)			// If the size of the vector is greater or equal to 2 for the 2 values for positioning
							{
								_enemy->SetMapTilePosition(Vector2(values[0], values[1]), m__map->GetScrollOffset(), tileSize);
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
						else if (attrib.name == ATTRIBUTE_NAMES[A_NPC_TYPE])
						{
							for (size_t enemyType = 0; enemyType < NPC::NUM_NPC_TYPE; ++enemyType)
							{
								if (attrib.value == NPC_TYPE_NAMES[enemyType])
								{
									// Initialize special NPCs to the objective
									if(enemyType == NPC::NT_HOSTAGE)
									{
										ObjectiveHostage* _objHost = dynamic_cast<ObjectiveHostage*>(m__objective);

										// Only assign if the objective is a hostage objective
										if (_objHost != NULL)
										{
											_objHost->Init(_enemy);
										}
									}
									else if (enemyType == NPC::NT_TARGET)
									{
										ObjectiveAssassinate* _objAss = dynamic_cast<ObjectiveAssassinate*>(m__objective);
										
										// Only assign if the objective is an assassination objective
										if (_objAss != NULL)
										{
											_objAss->Init(_enemy);
										}
									}

									_enemy->SetNPCType(static_cast<NPC::E_NPC_TYPE>(enemyType));

									break;
								}
							}
						}
						else if (attrib.name == ATTRIBUTE_NAMES[A_PATROL_POINT])
						{
							vector<float> values = StringToFloats(attrib.value);

							if (values.size() >= 2)			// If the size of the vector is greater or equal to 2 for the 2 values for positioning
							{
								// Multiply by tile size to convert from tile coord to map coord in funct
								_enemy->AddPatrolPoint( _enemy->GetMapTilePos() + Vector2(values[0], values[1]));
							}							
						}
						else if (attrib.name == ATTRIBUTE_NAMES[A_VIEW_RANGE])
						{
							vector<float> values = StringToFloats(attrib.value);

							if (values.size() >= 2)			// If the size of the vector is greater or equal to 2 for the 2 values for positioning
							{
								// Multiply by tile size to convert from tile coord to map coord
								_enemy->InitViewer(values[0], values[1]);
							}	
						}
					}

					_enemy->SetMoveToDist(m__map->GetTileSize() );
					// Add the enemy to the enemy list
					m_enemyList.push_back(_enemy);
				}
			}
		}		
	}
}

void Level::InitMap(Vector2 numMapTile, int viewWidth, int viewHeight, float tileSize, string filePath, vector<Mesh*> &meshList, Vector2 numScreenTile)
{
	m__map = new TileMap(numMapTile, numScreenTile, viewWidth / numScreenTile.x);
	m__map->LoadTileMap(filePath, meshList);
}

void Level::Update(double dt)
{

}

void Level::UpdateObjective(double dt)
{
	m__objective->Update(dt);
}

void Level::ActivateObjective(void)
{
	m__objective->Activate();
}

Objective* Level::GetObjective(void) const
{
	return m__objective;
}

bool Level::GetObjectiveComplete(void) const
{
	return m__objective->IsCompleted();
}

bool Level::GetActiveObjective()const
{
	return m__objective->Active();
}

string Level::GetMessagesFile(void) const
{
	return m_messagesFile;
}

string Level::GetBGMName(void) const
{
	return m_bgmName;
}

void Level::Clear(void)
{
	if (m__map != NULL)
	{
		m__map->Clear();
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

vector<NPC*> Level::GetEnemyList(void)
{
	return m_enemyList;
}

void Level::ResetObjective()
{
	//reset the objective completed back to init
	m__objective->Reset();
}