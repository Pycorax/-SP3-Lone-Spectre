#include "MVC_Model_Spectre.h"
#include "SoundEngine.h"

const Vector2 MVC_Model_Spectre::S_M_MESSAGE_OFFSET(20.0f, 20.0f);
const float MVC_Model_Spectre::S_M_MAX_ALERT = 5.f;

MVC_Model_Spectre::MVC_Model_Spectre(string configSONFile) : MVC_Model(configSONFile)
	, m_appState(AS_MAIN_GAME)
	, m_currentLevelID(1)
	, m__currentLevel(NULL)
	, m__player(NULL)
	, m_enableShadow(true)
	, m_alertLevel(0.f)
	, m__alert(NULL)
	, m__spectreDive(NULL)
	, m__spectreHost(NULL)
	, m__spectreJump(NULL)
	, m__fKey(NULL)
	, m__kKey(NULL)
	, m_objective(NULL)
{
}


MVC_Model_Spectre::~MVC_Model_Spectre(void)
{
}

void MVC_Model_Spectre::processKeyAction(double dt)
{
	switch (m_appState)
	{
		case AS_MAIN_GAME:
		{
			if (m_bKeyPressed[MOVE_FORWARD_KEY])
			{
				m__player->SetMove(Direction::DIRECTIONS[Direction::DIR_UP]);
			}
			if (m_bKeyPressed[MOVE_BACKWARD_KEY])
			{
				m__player->SetMove(Direction::DIRECTIONS[Direction::DIR_DOWN]);
			}
			if (m_bKeyPressed[MOVE_LEFT_KEY])
			{
				m__player->SetMove(Direction::DIRECTIONS[Direction::DIR_LEFT]);
			}
			if (m_bKeyPressed[MOVE_RIGHT_KEY])
			{
				m__player->SetMove(Direction::DIRECTIONS[Direction::DIR_RIGHT]);
			}

			if (m_bKeyPressed[INTERACT_GENERIC_KEY])
			{
				if (m__player->Interact(Player::INTERACT_ESCAPE, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_ESCAPE
					&& m__currentLevel->GetObjectiveComplete() == true)
				{
					nextLevel();
				}
				if ((m__player->Interact(Player::INTERACT_ASSASSINATE, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_ASSASSINATE)
					|| (m__player->Interact(Player::INTERACT_COLLECT, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_COLLECT)
					|| (m__player->Interact(Player::INTERACT_DEFUSE, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_DEFUSE)
					|| (m__player->Interact(Player::INTERACT_SETBOMB, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_SETBOMB))
				{
					if (m__currentLevel->GetObjectiveComplete() == false && m__currentLevel->GetActiveObjective() == false)
					{
						m__currentLevel->ActivateObjective();
					}

				}
				if ((m__player->Interact(Player::INTERACT_DEFUSE, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_DEFUSE)
					|| (m__player->Interact(Player::INTERACT_SETBOMB, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_SETBOMB))
				{
					//GetActiveObjective - means objective is updating
					if (m__currentLevel->GetActiveObjective())
					{
						m__currentLevel->UpdateObjective(dt);
					}
				}
				if (m__currentLevel->GetObjectiveComplete() == true)
				{
					if ((m__player->Interact(Player::INTERACT_COLLECT, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_COLLECT)
						|| (m__player->Interact(Player::INTERACT_DEFUSE, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_DEFUSE)
						|| (m__player->Interact(Player::INTERACT_SETBOMB, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_SETBOMB))
					{
						// Force set tilemap
						Tile* _tileOnPlayer = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos());
						Tile* _tileInFrontPlayer = m__currentLevel->GetTileMap()->GetTileAt
							(m__player->GetMapPos() + m__player->GetLookDir() * m__currentLevel->GetTileMap()->GetTileSize());
						if (_tileOnPlayer->GetType() == Tile::TILE_BOMB || _tileOnPlayer->GetType() == Tile::TILE_DOCUMENT)
						{
							_tileOnPlayer->SetMesh(GetMeshResource("TILE_FLOOR"));
							_tileOnPlayer->SetType(Tile::TILE_FLOOR);
						}
						else if ((_tileInFrontPlayer->GetType() == Tile::TILE_BOMB || _tileInFrontPlayer->GetType() == Tile::TILE_DOCUMENT))
						{
							_tileInFrontPlayer->SetMesh(GetMeshResource("TILE_FLOOR"));
							_tileInFrontPlayer->SetType(Tile::TILE_FLOOR);
						}
						else if (_tileOnPlayer->GetType() == Tile::TILE_SETBOMBAREA)
						{
							_tileOnPlayer->SetMesh(GetMeshResource("TILE_BOMB"));
							_tileOnPlayer->SetType(Tile::TILE_BOMB);
						}
						else if (_tileInFrontPlayer->GetType() == Tile::TILE_SETBOMBAREA)
						{
							_tileInFrontPlayer->SetMesh(GetMeshResource("TILE_BOMB"));
							_tileInFrontPlayer->SetType(Tile::TILE_BOMB);
						}
					}
				}
			}
			else
			{
				//GetActiveObjective - means objective is updating
				if (m__currentLevel->GetActiveObjective())
				{
					m__currentLevel->UpdateObjective(dt);
				}
			}

			if (m_bKeyPressed[INTERACT_SKILL_2_KEY]) // Spectral Hax
			{
				if (m__player->Interact(Player::INTERACT_HAX, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_HAX)
				{
					m__player->SetState(Player::PS_SPECTRAL_HAX);
					startHackMode();
				}
			}

			if (m_bKeyPressed[INTERACT_SKILL_1_KEY] && m__player->Interact(Player::INTERACT_DIVE, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_DIVE) // Spectral Dive
			{
				m__player->SetDive();
				//if player currently not hosting
				if (m__player->GetHosting() == false)
				{
					for (vector<Enemy*>::iterator enemyIter = m_enemyList.begin(); enemyIter != m_enemyList.end(); ++enemyIter)
					{
						Enemy* _enemy = *enemyIter;
						//if player dives within a tile distance from player
						if ((m__player->GetMapPos() - _enemy->GetMapPos()).LengthSquared() <= m__currentLevel->GetTileMap()->GetTileSize() *  m__currentLevel->GetTileMap()->GetTileSize())
						{
							m__player->SetHostPTR(_enemy);
							break;
						}
					}
				}
			}

			if (m_bKeyPressed[MOVE_JUMP_KEY] && m__player->Interact(Player::INTERACT_JUMP, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_JUMP) // Spectral Jump
			{
				m__player->SetJump();
			}
			break;
		}

		case AS_HACK_GAME:
		{
			m_hackingGame.Move(m_bKeyPressed[MOVE_LEFT_KEY], m_bKeyPressed[MOVE_RIGHT_KEY], m_bKeyPressed[MOVE_FORWARD_KEY], m_bKeyPressed[MOVE_BACKWARD_KEY], dt);
			break;
		}
		
	}

	// Quitting the game
	if (m_bKeyPressed[GAME_EXIT_KEY])
	{
		// TODO: Open a pause menu and then quit by that instead. Do actual pausing or return to menus
		m_running = false;
	}
}

int MVC_Model_Spectre::findLevelFiles(string folderPath)
{
	m_levelFiles.clear();

	short numOfFiles = 0;
	ifstream files;

	//Gets all file names
	std::ostringstream command;
	command <<"dir \"" << folderPath << "\" /b /a-d > level_names";
	system(command.str().c_str());

	files.open("level_names");

	if (files.is_open())
	{
		string output;

		while (!files.eof())
		{
			getline(files, output);
			if (!(output == ""))
			{
				// Append the Folder
				output = folderPath + output;
				m_levelFiles.push_back(output);
				++numOfFiles;
			}
		}

		files.close();

		remove("level_names");
	}
	else
	{
		return -1;
	}

	return numOfFiles;
}

void MVC_Model_Spectre::loadLevel(string levelMapFile)
{
	Vector2 numScreenTile;
	// Delete the previous level
	if (m__currentLevel != NULL)
	{
		numScreenTile = m__currentLevel->GetTileMap()->GetNumScreenTile();
		m__currentLevel->Clear();
		delete m__currentLevel;
		m__currentLevel = NULL;
	}
	clearCameraList();

	// Initialize the level
	m__currentLevel = new Level();
	//m__currentLevel->InitMap(Vector2(64, 50), m_viewWidth, m_viewHeight, 64, "TileMap//Level1.csv", meshList);
	m__currentLevel->Load(levelMapFile, m_viewWidth, m_viewHeight, meshList, numScreenTile);
	SecurityCamera::InitCamMeshList(meshList);
	clearCameraList();
	loadToList(m__currentLevel->GetTileMap());
	
	// Setup camera view
	for (vector<SecurityCamera*>::iterator it = m_cameraList.begin(); it != m_cameraList.end(); ++it)
	{
		SecurityCamera* _camera = *it;
		if (_camera)
		{
			_camera->GenerateViewBox(m__currentLevel->GetTileMap());
		}
	}

	// Initialize the player
	int tileSize = m__currentLevel->GetTileMap()->GetTileSize();
	Vector2 playerSpawnPos = m__currentLevel->GetTileMap()->GetPlayerSpawnPos();
	Vector2 spawnScrollOffset = playerSpawnPos - m__currentLevel->GetTileMap()->GetScreenSize() * 0.5f;
	m__currentLevel->GetTileMap()->SetScrollOffset(spawnScrollOffset);
	m__player->SetMapPosition(playerSpawnPos, spawnScrollOffset, m__currentLevel->GetTileMap()->GetTileSize()); // Start at center with no scroll offset
	m__player->SetScale(Vector3(tileSize, tileSize));

	// Initialize the enemies
	vector<Enemy*> enemies = m__currentLevel->GetEnemyList();
	for (vector<Enemy*>::iterator enemyIT = enemies.begin(); enemyIT != enemies.end(); ++enemyIT)
	{
		Enemy* _enemy = new Enemy(**enemyIT);

		_enemy->InitPathFinder(m__currentLevel->GetTileMap());
		Vector2 mapPos = m__player->GetMapTilePos();
		_enemy->SetTarget(mapPos.x, mapPos.y);

		m_enemyList.push_back(_enemy);
	}

	// Initialize the messages
	// -- Clear any previous messages from any previous levels
	m_messenger.ClearMessages();
	// -- Load the messages for this map
	m_messenger.AddMessages(m__currentLevel->GetMessagesFile());
}

void MVC_Model_Spectre::nextLevel(void)
{
	m_currentLevelID = Math::Wrap(++m_currentLevelID, 0, static_cast<int>(m_levelFiles.size() - 1));

	loadLevel(m_levelFiles[m_currentLevelID]);
}

void MVC_Model_Spectre::prevLevel(void)
{
	m_currentLevelID = Math::Wrap(--m_currentLevelID, 0, static_cast<int>(m_levelFiles.size() - 1));

	loadLevel(m_levelFiles[m_currentLevelID]);
}

void MVC_Model_Spectre::pushMessageToRender(void)
{
	// Store the message objects into this
	m_messageObjects = m_messenger.GetMessageObjects(m_viewWidth, m_viewHeight);

	for (vector<GameObject2D*>::iterator messageObjects = m_messageObjects.begin(); messageObjects != m_messageObjects.end(); ++messageObjects)
	{
		m_renderList2D.push(*messageObjects);
	}
}

void MVC_Model_Spectre::resetTileMarkers(void)
{
	for (vector<GameObject2D*>::iterator tileMarker = m__tileMarkers.begin(); tileMarker != m__tileMarkers.end(); ++tileMarker)
	{
		(*tileMarker)->SetActive(false);
	}
}

GameObject2D * MVC_Model_Spectre::fetchTileMarker(TILE_MARKER_TYPE type)
{
	// Retrieve a light
	for (vector<GameObject2D*>::iterator tileMarker = m__tileMarkers.begin(); tileMarker != m__tileMarkers.end(); ++tileMarker)
	{
		if (!(*tileMarker)->GetActive())
		{
			(*tileMarker)->SetActive(true);
			(*tileMarker)->SetMesh(m__tileMarkerMesh[type]);
			return *tileMarker;
		}
	}

	// Generate some if unavailable
	const int BATCH_MAKE = 10;
	for (size_t tileMarker = 0; tileMarker < BATCH_MAKE; ++tileMarker)
	{
		PhysicalObject* _tileMarker = new PhysicalObject();
		_tileMarker->SetActive(false);
		m__tileMarkers.push_back(_tileMarker);
	}

	m__tileMarkers.back()->SetActive(true);
	m__tileMarkers.back()->SetMesh(m__tileMarkerMesh[type]);

	return m__tileMarkers.back();
}

void MVC_Model_Spectre::startHackMode(void)
{
	m_appState = AS_HACK_GAME;
	m_hackingGame.Reset(m_viewWidth, m_viewHeight);
}

void MVC_Model_Spectre::Init(void)
{
	MVC_Model::Init();

	// -- Load Shadow GameObject
	m__tileMarkerMesh[TM_SHADOW] = GetMeshResource("ShadowOverlay");
	m__tileMarkerMesh[TM_VIEWED] = GetMeshResource("LightOverlay");

	// Load the player
	initPlayer();

	// Init the hacking game
	m_hackingGame.Init
		(
			GetMeshResource("ShadowBall"), 
			GetMeshResource("PlayerBall"), 
			GetMeshResource("CircuitWall"), 
			GetMeshResource("DestroyedWall"), 
			GetMeshResource("RestrictedWall"), 
			GetMeshResource("LoseScreen"), 
			GetMeshResource("MinigameBG"),
			m_viewWidth, m_viewHeight);

	findLevelFiles("Levels//");
	loadLevel(m_levelFiles[m_currentLevelID]);
	float tileSize = m__currentLevel->GetTileMap()->GetTileSize();

	// Load HUD
	initHUD();

	// Init the MessageManager
	m_messenger.Init(GetMeshResource("MessageBG"), m_defaultFont, m_defaultFont, Vector2(m_viewWidth - S_M_MESSAGE_OFFSET.x, 200.0f), S_M_MESSAGE_OFFSET);
	m_messenger.AddMessages("Messages//Level1_Message.son");

	////Enemy
	//Enemy* _enemy = new Enemy;
	////_enemy->SetMapPosition(m__currentLevel->GetTileMap()->GetScreenSize() * 0.5f, m__currentLevel->GetTileMap()->GetScrollOffset(),tileSize);
	//_enemy->SetMapTilePosition(Vector2(4,5),Vector2(0, 32), tileSize);
	//_enemy->Init(_enemy->GetMapPos() , GetMeshResource("Enemy_ANIMATION"));
	//_enemy->SetScale(Vector2(tileSize, tileSize));
	//_enemy->initPathFinder(m__currentLevel->GetTileMap());
	//_enemy->SetTarget(m__player->GetMapPos(), tileSize);
	////patrol points per enemy
	//_enemy->AddPatrolPoint(_enemy->GetMapTilePos() + Vector2(0, -5));
	//_enemy->AddPatrolPoint(_enemy->GetMapTilePos() + Vector2(0,  2));
	//_enemy->AddPatrolPoint(_enemy->GetMapTilePos() + Vector2(7,  2));
	//_enemy->AddPatrolPoint(_enemy->GetMapTilePos() + Vector2(7, -5));
	//// Set up enemy's view
	//_enemy->InitViewer(1, 2);
	////_enemy->ForceSetEnemyState(Enemy::ES_CHASE);
	//m_enemyList.push_back(_enemy);

	//ObjectiveCollect;

	// TODO: Remove this. This is preventing a crash of sound engine
	SoundPlayer2D* _snd = SoundEngine::CreateSound2D(0);
	_snd->Play(true);
}

void MVC_Model_Spectre::initHUD(void)
{
	int hudCount = 0;
	const float HUD_OFFSET = static_cast<float>(m_viewHeight) * 0.15f;
	if (m__alert == NULL)
	{
		m__alert = new HUD_Cooldown();
	}
	const Vector2 ALERT_SCALE(static_cast<float>(m_viewWidth) * 0.2f, static_cast<float>(m_viewHeight) * 0.1f);
	const Vector2 ALERT_POS(static_cast<float>(m_viewWidth) - ALERT_SCALE.x, static_cast<float>(m_viewHeight) - ALERT_SCALE.y - (hudCount * HUD_OFFSET));
	m__alert->Init(GetMeshResource("AlertBar"), ALERT_POS, ALERT_SCALE, GetMeshResource("AlertCover"), ALERT_POS, ALERT_SCALE, true, false);
	++hudCount;

	if (m__spectreDive == NULL)
	{
		m__spectreDive = new HUD_Cooldown();
	}
	const Vector2 DIVE_SCALE(static_cast<float>(m_viewHeight) * 0.1f, static_cast<float>(m_viewHeight) * 0.1f);
	const Vector2 DIVE_POS(static_cast<float>(m_viewWidth) - DIVE_SCALE.x, static_cast<float>(m_viewHeight) - DIVE_SCALE.y - (hudCount * HUD_OFFSET));
	m__spectreDive->Init(GetMeshResource("Skill_DiveIcon"), DIVE_POS, DIVE_SCALE, GetMeshResource("Skill_Cover"), DIVE_POS, DIVE_SCALE, false, true);
	++hudCount;

	if (m__spectreJump == NULL)
	{
		m__spectreJump = new HUD_Cooldown();
	}
	const Vector2 JUMP_SCALE(static_cast<float>(m_viewHeight) * 0.1f, static_cast<float>(m_viewHeight) * 0.1f);
	const Vector2 JUMP_POS(static_cast<float>(m_viewWidth) - JUMP_SCALE.x, static_cast<float>(m_viewHeight) - JUMP_SCALE.y - (hudCount * HUD_OFFSET));
	m__spectreJump->Init(GetMeshResource("Skill_JumpIcon"), JUMP_POS, JUMP_SCALE, GetMeshResource("Skill_Cover"), JUMP_POS, JUMP_SCALE, false, true);
	++hudCount;

	if (m__spectreHost == NULL)
	{
		m__spectreHost = new HUD_Cooldown();
	}
	const Vector2 HOST_SCALE(static_cast<float>(m_viewHeight) * 0.1f, static_cast<float>(m_viewHeight) * 0.1f);
	const Vector2 HOST_POS(static_cast<float>(m_viewWidth) - JUMP_SCALE.x, static_cast<float>(m_viewHeight) - JUMP_SCALE.y - (hudCount * HUD_OFFSET));
	m__spectreHost->Init(GetMeshResource("Skill_HostIcon"), HOST_POS, HOST_SCALE, GetMeshResource("Skill_Cover"), HOST_POS, HOST_SCALE, false, true);
	++hudCount;

	// Action prompt HUD
	float actionKeySize = m__currentLevel->GetTileMap()->GetTileSize() * 0.5f;
	const Vector2 ACTION_KEY_SCALE(actionKeySize, actionKeySize);
	if (m__fKey == NULL)
	{
		m__fKey = new HUD();
	}
	m__fKey->Init(GetMeshResource("F_Key"), Vector2(0,0), ACTION_KEY_SCALE);
	m__fKey->SetActive(false);

	if (m__kKey == NULL)
	{
		m__kKey = new HUD();
	}
	m__kKey->Init(GetMeshResource("K_Key"), Vector2(0,0), ACTION_KEY_SCALE);
	m__kKey->SetActive(false);
}

void MVC_Model_Spectre::updateMainGame(double dt)
{
	// Updates player depending on actions queued.
	m__player->Update(dt, m__currentLevel->GetTileMap());

	// Update the messenger
	m_messenger.Update(dt);

	// Update camera list
	updateCamera(dt);

	// Player inside viewed tile
	static const double S_ALERT_SPEED = 4.0;
	static const double S_ALERT_HOSTILE = 2.0f;
	Tile* _tile = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos());
	if (_tile->IsViewed() && !m__player->GetInShadow()) // Player is viewed by some
	{
		_tile->NotifyViewer(m__player->GetMapTilePos(), m_alertLevel, dt * S_ALERT_SPEED);
	}

	//update enemies
	for (vector<Enemy*>::iterator enemyIter = m_enemyList.begin(); enemyIter != m_enemyList.end(); ++enemyIter)
	{
		Enemy* _enemy = (*enemyIter);
		_enemy->Update(dt, m__currentLevel->GetTileMap());
		Vector2 mapPos = m__player->GetMapPos();
		_enemy->SetTarget(mapPos.x, mapPos.y);
		_enemy->SetAlertLevel(m_alertLevel);
		if (_enemy->GetAlertLevel() > S_ALERT_HOSTILE)
		{
			if ((_enemy->GetMapTilePos() - _tile->GetMapTilePos()).LengthSquared() < (m__currentLevel->GetTileMap()->GetTileSize() * m__currentLevel->GetTileMap()->GetTileSize()))
			{
				_enemy->AttackingInView(m__player);
			}
			else
			{
				_enemy->ForceSetEnemyState(Enemy::ES_SCAN);
			}
		}

	}
	if (m__player->GetHealth() <= 0)
	{
		std::cout << "K.O." << std::endl;
	}
	// Update Lighting
	vector<Vector2> shadowCasters;
	// Give enemies a shadow
	for (vector<Enemy*>::iterator enemyIter = m_enemyList.begin(); enemyIter != m_enemyList.end(); ++enemyIter)
	{
		shadowCasters.push_back((*enemyIter)->GetMapTilePos());
	}
	//shadowCasters.push_back(m__player->GetMapTilePos());
	m__currentLevel->GetTileMap()->UpdateLighting(shadowCasters);

	// Update alert system
	updateHUD(dt);


	// Rendering
	tileMapToRender(m__currentLevel->GetTileMap());

	// -- Render Player
	m_renderList2D.push(m__player);

	// Render Enemies
	for (vector<Enemy*>::iterator enemyIter = m_enemyList.begin(); enemyIter != m_enemyList.end(); ++enemyIter)
	{
		m_renderList2D.push((*enemyIter));
	}

	// Render HUD
	// Alert
	m_renderList2D.push(m__alert);
	m_renderList2D.push(m__alert->GetDisplayCover());
	// Dive skill
	m_renderList2D.push(m__spectreDive);
	m_renderList2D.push(m__spectreDive->GetDisplayCover());
	// Jump skill
	m_renderList2D.push(m__spectreJump);
	m_renderList2D.push(m__spectreJump->GetDisplayCover());
	// Host skill
	m_renderList2D.push(m__spectreHost);
	m_renderList2D.push(m__spectreHost->GetDisplayCover());
	// Action prompts
	float tileSize = m__currentLevel->GetTileMap()->GetTileSize();
	Tile* _tileOnPlayer = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos());
	Tile::E_TILE_TYPE tileTypeOnPlayer = _tileOnPlayer->GetType();
	Vector2 frontTilePos = m__player->GetMapPos() + (m__player->GetLookDir() * tileSize);
	frontTilePos.x = floor(frontTilePos.x / tileSize);
	frontTilePos.y = floor(frontTilePos.y / tileSize);
	Tile* _tileInFrontPlayer = NULL;
	Tile::E_TILE_TYPE tileTypeInFrontPlayer;
	if (frontTilePos.x >= 0 && frontTilePos.x < m__currentLevel->GetTileMap()->GetNumScreenTile().x && frontTilePos.y >= 0 && frontTilePos.y < m__currentLevel->GetTileMap()->GetNumScreenTile().y)
	{
		_tileInFrontPlayer = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos() + (m__player->GetLookDir() * tileSize));
		tileTypeInFrontPlayer = _tileInFrontPlayer->GetType();
	}
	Vector2 playerDir = m__player->GetLookDir();
	Vector2 spawnDir;
	const Vector2 S_ACTION_HUD_OFFSET(tileSize * 0.25f, tileSize * 0.25f);
	if (tileTypeOnPlayer == Tile::TILE_OBJ_CAMERA_ON_1_1
		||
		tileTypeOnPlayer == Tile::TILE_OBJ_CAMERA_ON_1_2
		||
		tileTypeOnPlayer == Tile::TILE_OBJ_CAMERA_ON_1_3
		||
		tileTypeOnPlayer == Tile::TILE_OBJ_CAMERA_ON_1_4
		||
		(_tileInFrontPlayer && (tileTypeInFrontPlayer == Tile::TILE_OBJ_CAMERA_ON_1_1
			||
			tileTypeInFrontPlayer == Tile::TILE_OBJ_CAMERA_ON_1_2
			||
			tileTypeInFrontPlayer == Tile::TILE_OBJ_CAMERA_ON_1_3
			||
			tileTypeInFrontPlayer == Tile::TILE_OBJ_CAMERA_ON_1_4))) // Spawn K key if camera
	{
		// Spawn hud key to the right of where player is
		if (playerDir == Direction::DIRECTIONS[Direction::DIR_UP])			// If facing up, set spawn direction to right
		{
			spawnDir = Direction::DIRECTIONS[Direction::DIR_RIGHT];
		}
		else if (playerDir == Direction::DIRECTIONS[Direction::DIR_RIGHT])	// If facing right, set spawn direction to down
		{
			spawnDir = Direction::DIRECTIONS[Direction::DIR_DOWN];
		}
		else if (playerDir == Direction::DIRECTIONS[Direction::DIR_DOWN])	// If facing down, set spawn direction to left
		{
			spawnDir = Direction::DIRECTIONS[Direction::DIR_LEFT];
		}
		else if (playerDir == Direction::DIRECTIONS[Direction::DIR_LEFT])	// If facing left, set spawn direction to up
		{
			spawnDir = Direction::DIRECTIONS[Direction::DIR_UP];
		}
		m__kKey->SetActive(true);
		m__kKey->SetPos((m__player->GetMapPos() - m__currentLevel->GetTileMap()->GetScrollOffset()) + (spawnDir * tileSize) + S_ACTION_HUD_OFFSET);
		m_renderList2D.push(m__kKey);
	}
	else if (tileTypeOnPlayer == Tile::TILE_BOMB
		||
		tileTypeOnPlayer == Tile::TILE_DOCUMENT
		||
		tileTypeOnPlayer == Tile::TILE_SETBOMBAREA
		||
		tileTypeOnPlayer == Tile::TILE_EXTRACTION
		||
		(_tileInFrontPlayer && (tileTypeInFrontPlayer == Tile::TILE_BOMB
			||
			tileTypeInFrontPlayer == Tile::TILE_DOCUMENT
			||
			tileTypeInFrontPlayer == Tile::TILE_SETBOMBAREA))) // Spawn F if interactions
	{
		// Spawn hud key to the right of where player is
		if (playerDir == Direction::DIRECTIONS[Direction::DIR_UP])			// If facing up, set spawn direction to right
		{
			spawnDir = Direction::DIRECTIONS[Direction::DIR_RIGHT];
		}
		else if (playerDir == Direction::DIRECTIONS[Direction::DIR_RIGHT])	// If facing right, set spawn direction to down
		{
			spawnDir = Direction::DIRECTIONS[Direction::DIR_DOWN];
		}
		else if (playerDir == Direction::DIRECTIONS[Direction::DIR_DOWN])	// If facing down, set spawn direction to left
		{
			spawnDir = Direction::DIRECTIONS[Direction::DIR_LEFT];
		}
		else if (playerDir == Direction::DIRECTIONS[Direction::DIR_LEFT])	// If facing left, set spawn direction to up
		{
			spawnDir = Direction::DIRECTIONS[Direction::DIR_UP];
		}
		m__fKey->SetActive(true);
		m__fKey->SetPos((m__player->GetMapPos() - m__currentLevel->GetTileMap()->GetScrollOffset()) + (spawnDir * tileSize) + S_ACTION_HUD_OFFSET);
		m_renderList2D.push(m__fKey);
	}
	else
	{
		m__fKey->SetActive(false);
		m__kKey->SetActive(false);
	}

	// Render Messages
	pushMessageToRender();

	m_renderList2D.push(m_fpsCount);
}

void MVC_Model_Spectre::updateHackGame(double dt)
{
	m_hackingGame.Update(dt);

	if (m_hackingGame.IsVictory())
	{
		m_appState = AS_MAIN_GAME;
		m__player->SetState(Player::PS_IDLE_DOWN);
		// Deactivate camera from camera list and tilemap
		for (vector<SecurityCamera*>::iterator it = m_cameraList.begin(); it != m_cameraList.end(); ++it)
		{
			SecurityCamera* _camera = *it;
			Vector2 cameraTilePos = _camera->GetMapTilePos();
			Vector2 playerTilePos = m__player->GetMapTilePos();
			Vector2 playerFrontTilePos = playerTilePos + m__player->GetLookDir();
			if (cameraTilePos == playerTilePos || cameraTilePos == playerFrontTilePos) // Found camera
			{
				_camera->DestroyViewBox(m__currentLevel->GetTileMap());
				_camera->SetState(false);
				// Change mesh for camera and tile obj
				if (_camera->GetDir() == Direction::DIRECTIONS[Direction::DIR_UP])
				{
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_UP]);
					// Force set tilemap
					if (cameraTilePos == playerTilePos)
					{
						Tile* _tile = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos());
						_tile->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_UP]);
						_tile->SetType(Tile::TILE_OBJ_CAMERA_OFF_1_3);
					}
					else if (cameraTilePos == playerFrontTilePos)
					{
						Tile* _tile = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos() + (m__player->GetLookDir() * m__currentLevel->GetTileMap()->GetTileSize()));
						_tile->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_UP]);
						_tile->SetType(Tile::TILE_OBJ_CAMERA_OFF_1_3);
					}
				}
				else if (_camera->GetDir() == Direction::DIRECTIONS[Direction::DIR_DOWN])
				{
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_DOWN]);
					// Force set tilemap
					if (cameraTilePos == playerTilePos)
					{
						Tile* _tile = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos());
						_tile->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_DOWN]);
						_tile->SetType(Tile::TILE_OBJ_CAMERA_OFF_1_1);
					}
					else if (cameraTilePos == playerFrontTilePos)
					{
						Tile* _tile = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos() + (m__player->GetLookDir() * m__currentLevel->GetTileMap()->GetTileSize()));
						_tile->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_DOWN]);
						_tile->SetType(Tile::TILE_OBJ_CAMERA_OFF_1_1);
					}
				}
				else if (_camera->GetDir() == Direction::DIRECTIONS[Direction::DIR_LEFT])
				{
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_LEFT]);
					// Force set tilemap
					if (cameraTilePos == playerTilePos)
					{
						Tile* _tile = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos());
						_tile->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_LEFT]);
						_tile->SetType(Tile::TILE_OBJ_CAMERA_OFF_1_2);
					}
					else if (cameraTilePos == playerFrontTilePos)
					{
						Tile* _tile = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos() + (m__player->GetLookDir() * m__currentLevel->GetTileMap()->GetTileSize()));
						_tile->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_LEFT]);
						_tile->SetType(Tile::TILE_OBJ_CAMERA_OFF_1_2);
					}
				}
				else if (_camera->GetDir() == Direction::DIRECTIONS[Direction::DIR_RIGHT])
				{
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_RIGHT]);
					// Force set tilemap
					if (cameraTilePos == playerTilePos)
					{
						Tile* _tile = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos());
						_tile->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_RIGHT]);
						_tile->SetType(Tile::TILE_OBJ_CAMERA_OFF_1_4);
					}
					else if (cameraTilePos == playerFrontTilePos)
					{
						Tile* _tile = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos() + (m__player->GetLookDir() * m__currentLevel->GetTileMap()->GetTileSize()));
						_tile->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_RIGHT]);
						_tile->SetType(Tile::TILE_OBJ_CAMERA_OFF_1_4);
					}
				}
				break;
			}
		}
	}
	else if (m_hackingGame.IsLoss())
	{
		m_appState = AS_MAIN_GAME;
		m__player->SetState(Player::PS_IDLE_DOWN);
	}

	vector<GameObject2D*> minigameObjects = m_hackingGame.GetRenderObjects();
	for (vector<GameObject2D*>::iterator go = minigameObjects.begin(); go != minigameObjects.end(); ++go)
	{
		m_renderList2D.push(*go);
	}
}

void MVC_Model_Spectre::initPlayer(void)
{
	m__player = Player::GetInstance();
	m__player->Init(GetMeshResource("Player_ANIMATION"));
	//player mesh and states
	//SpriteAnimation* _sa = dynamic_cast<SpriteAnimation* >(GetMeshResource("Player_ANIMATION"));
	//m__player->SetMesh(_sa);
	Animation* _a;

	//walking south
	_a = new Animation();
	_a->Set(0, 2, 0, 0.4f);
	m__player->AddAnimation(_a, Player::PS_WALK_DOWN);
	//walking right
	_a = new Animation();
	_a->Set(3, 5, 0, 0.4f); 
	m__player->AddAnimation(_a, Player::PS_WALK_RIGHT);
	//walk left
	_a = new Animation();
	_a->Set(6, 8, 0, 0.4f);
	m__player->AddAnimation(_a, Player::PS_WALK_LEFT);
	//walk north
	_a = new Animation();
	_a->Set(9, 11, 0, 0.4f);
	m__player->AddAnimation(_a, Player::PS_WALK_UP);
	//idle south
	_a = new Animation();
	_a->Set(1,1,0,0.f);
	m__player->AddAnimation(_a , Player::PS_IDLE_DOWN);
	//idle right
	_a = new Animation();
	_a->Set(5,5,0,0.f);
	m__player->AddAnimation(_a, Player::PS_IDLE_RIGHT);
	//idle left
	_a = new Animation();
	_a->Set(7,7, 0, 0.f);
	m__player->AddAnimation(_a, Player::PS_IDLE_LEFT);
	//idle north
	_a = new Animation();
	_a->Set(10, 10, 0, 0.f);
	m__player->AddAnimation(_a , Player::PS_IDLE_UP);
	
	
	//diving South
	_a = new Animation();
	_a->Set(12, 17, 1, 0.3f);
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVING_DOWN);
	//diving right
	_a = new Animation();
	_a->Set(18, 23, 1, 0.3f);
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVING_RIGHT);
	//diving left
	_a = new Animation();
	_a->Set(24, 29, 1, 0.3f);
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVING_LEFT);
	//diving north
	_a = new Animation();
	_a->Set(30, 35, 1, 0.3f);
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVING_UP);

	//shadow form north
	_a = new Animation();
	_a->Set(36, 36, 0, 0.f);
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVE_UP);
	// must set a animation or else cannot use and dont need to change much code
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVE); 
	//shadow form south
	_a = new Animation();
	_a->Set(37, 37, 0, 0.f);
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVE_DOWN);
	//shadow form left
	_a = new Animation();
	_a->Set(38, 38, 0, 0.f);
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVE_LEFT);
	//shadow form right
	_a = new Animation();
	_a->Set(39, 39, 0, 0.f);
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVE_RIGHT);

	//set player health
	m__player->SetMaxHealth(1);
	m__player->SetHealth(1);
	//m__player->SetMapPosition(m__currentLevel->GetTileMap()->GetScreenSize() * 0.5f, Vector2(0,0), m__currentLevel->GetTileMap()->GetTileSize()); // Start at center with no scroll offset
	//m__player->SetScale(Vector3(m__currentLevel->GetTileMap()->GetTileSize(), m__currentLevel->GetTileMap()->GetTileSize()));
}

void MVC_Model_Spectre::Update(double dt)
{
	MVC_Model::Update(dt);

	switch (m_appState)
	{
		case AS_MAIN_GAME:
		{
			updateMainGame(dt);
			break;
		}
		case AS_HACK_GAME:
		{
			updateHackGame(dt);
			break;
		}
	}
}
void MVC_Model_Spectre::Exit(void)
{
	clearCameraList();

	m_hackingGame.Exit();

	Player::Clear();

	// Delete the messenger
	m_messenger.Exit();

	//clearing list
	while(m__colliderList.size() > 0)
	{
		Collider2D *go = m__colliderList.back();
		delete go;
		m__colliderList.pop_back();
	}

	while (m_enemyList.size() > 0)
	{
		if (m_enemyList.back() != NULL)
		{
			delete m_enemyList.back();
		}

		m_enemyList.pop_back();
	}

	if (m_objective != NULL)
	{
		delete m_objective;
	}

	// Clear the level
	if (m__currentLevel != NULL)
	{
		m__currentLevel->Clear();
		delete m__currentLevel;
		m__currentLevel = NULL;
	}

	// Clear the tile marker list
	while (m__tileMarkers.size() > 0)
	{
		GameObject2D* go = m__tileMarkers.back();

		if (go != NULL)
		{
			delete go;
			m__tileMarkers.pop_back();
		}
	}

	// Clear alert hud
	if (m__alert)
	{
		m__alert->Clear();
		delete m__alert;
		m__alert = NULL;
	}

	if (m__spectreDive)
	{
		m__spectreDive->Clear();
		delete m__spectreDive;
		m__spectreDive = NULL;
	}

	if (m__spectreJump)
	{
		m__spectreJump->Clear();
		delete m__spectreJump;
		m__spectreJump = NULL;
	}

	if (m__spectreHost)
	{
		m__spectreHost->Clear();
		delete m__spectreHost;
		m__spectreHost = NULL;
	}
	
	MVC_Model::Exit();
}

void MVC_Model_Spectre::tileMapToRender(TileMap* _ToRender)
{
	const Vector2 MAX_SCROLL_SIZE_TILE = _ToRender->GetNumMapTile(); // Max tile difference between map and screen
	vector<vector<Tile*>*> _map = _ToRender->GetMap();

	// Reset the lights from the previous frame rendered
	resetTileMarkers();

	// Calc the starting tile to render and round down any decimal as it is still seen
	Vector2 tileStart
		( 
			floor(_ToRender->GetScrollOffset().x / _ToRender->GetTileSize()),
			floor(_ToRender->GetScrollOffset().y / _ToRender->GetTileSize()) 
		);

	for (int row = 0; row < ceil(_ToRender->GetNumScreenTile().y) + 1; ++row)			// Loop for rows
	{
		for (int col = 0; col < ceil(_ToRender->GetNumScreenTile().x) + 1; ++col)	// Loop for columns (+1 for the offset)
		{
			if (row >= _ToRender->GetNumMapTile().y || col >= _ToRender->GetNumMapTile().x) // Stop the rendering if row and col goes out of map
			{
				break;
			}
			if (tileStart.y + row < 0 || tileStart.y + row >= MAX_SCROLL_SIZE_TILE.y || tileStart.x + col < 0 || tileStart.x + col >= MAX_SCROLL_SIZE_TILE.x)
			{
				continue;
			}
			Tile* _tile = (*_map[tileStart.y + row])[tileStart.x + col]; // Get the tile data based on loop
			_tile->SetMapPosition(_tile->GetMapPos(), _ToRender->GetScrollOffset(), m__currentLevel->GetTileMap()->GetTileSize()); // Calculate screen position based on map position for rendering
			if (m__player->GetInShadow() && _tile->GetLightLevel() > Player::S_SPECTRE_DIVE_LIGHT_LIMIT)
			{
				continue;
			}
			else
			{
				_tile->SetMapPosition(_tile->GetMapPos(), _ToRender->GetScrollOffset(), m__currentLevel->GetTileMap()->GetTileSize()); // Calculate screen position based on map position for rendering
				m_renderList2D.push(_tile); // Add to queue for rendering

				/*
				* Shadow Portion
				*/
				if (m_enableShadow && _tile->GetType() != Tile::TILE_INVISIBLE_WALL)
				{
					int numShadows = Tile::MAX_LIGHT_LEVEL - _tile->GetLightLevel();
					for (size_t lightLevel = 0; lightLevel < numShadows; lightLevel += TileMap::S_LIGHT_ATTENUATION)
					{
						Transform tileT = _tile->GetTransform();
						GameObject2D* _shadow = fetchTileMarker(TM_SHADOW);
						_shadow->SetPos(tileT.Translation);
						_shadow->SetScale(tileT.Scale);
						m_renderList2D.push(_shadow);
					}
				}

				if (_tile->IsViewed())
				{
					Transform tileT = _tile->GetTransform();
					GameObject2D* light = fetchTileMarker(TM_VIEWED);
					light->SetPos(tileT.Translation);
					light->SetScale(tileT.Scale);
					m_renderList2D.push(light);
				}
			}
		}	
	}
}

void MVC_Model_Spectre::onResolutionChanged(int oldViewWidth, int oldViewHeight)
{
	TileMap* _tilemap = m__currentLevel->GetTileMap();
	float oldTileSize = _tilemap->GetTileSize();
	_tilemap->SetTileSize(m_viewWidth / _tilemap->GetNumScreenTile().x);
	float newTileSize = _tilemap->GetTileSize();
	// Update the tilemap
	resizeTileMap(oldTileSize, newTileSize);

	// Update camera list position
	for (vector<SecurityCamera*>::iterator cameraIter = m_cameraList.begin(); cameraIter != m_cameraList.end(); ++cameraIter)
	{
		SecurityCamera* _camera = *cameraIter;
		Vector2 newPosition(floor(_camera->GetMapPos().x / oldTileSize) * newTileSize, floor(_camera->GetMapPos().y / oldTileSize) * newTileSize);
		_camera->SetMapPosition(newPosition, _tilemap->GetScrollOffset(), newTileSize);
	}

	// Update HUD position
	initHUD();

	// Update enemy position
	for (vector<Enemy*>::iterator enemyIter = m_enemyList.begin(); enemyIter != m_enemyList.end(); ++enemyIter)
	{
		Enemy* _enemy = *enemyIter;
		if (_enemy)
		{
			Vector2 enemyTilePos(floor(_enemy->GetMapPos().x / oldTileSize), floor(_enemy->GetMapPos().y / oldTileSize));
			_enemy->SetMapPosition(enemyTilePos * newTileSize, _tilemap->GetScrollOffset(), newTileSize);
			_enemy->SetScale(Vector2(newTileSize, newTileSize));
		}
	}

	// Update the Messenger
	m_messenger.SetMessageBGScale(Vector2(m_viewWidth - S_M_MESSAGE_OFFSET.x * 2, m_messenger.GetMessageBGScale().y));
}

void MVC_Model_Spectre::resizeTileMap(float oldTileSize, float newTileSize)
{
	TileMap* _tilemap = m__currentLevel->GetTileMap();
	vector<vector<Tile*>*> _map = _tilemap->GetMap();
	Vector2 playerTilePos(floor(m__player->GetMapPos().x / oldTileSize), floor(m__player->GetMapPos().y / oldTileSize));
	Vector2 mapScrollOffset(ceil(_tilemap->GetScrollOffset().x / oldTileSize), ceil(_tilemap->GetScrollOffset().y / oldTileSize));
	/*if (oldViewWidth < m_viewWidth) // Scale up screen
	{
		++mapScrollOffset.y;
		_tilemap->SetTileSize(m_viewWidth / _tilemap->GetNumScreenTile().x);
	}
	else if (oldViewWidth > m_viewWidth) // Scale down screen
	{
		--mapScrollOffset.y;
		_tilemap->SetTileSize(m_viewWidth / _tilemap->GetNumScreenTile().x);
	}*/
	_tilemap->SetScrollOffset(mapScrollOffset * newTileSize);
	for (int row = 0; row < _tilemap->GetNumMapTile().y; ++row)
	{
		for (int col = 0; col < _tilemap->GetNumMapTile().x; ++col)
		{
			_tilemap->SetMapSize(_tilemap->GetNumMapTile() * newTileSize);
			(*_map[row])[col]->SetMapPosition(Vector2(col * newTileSize, row * newTileSize), _tilemap->GetScrollOffset(), newTileSize);
			(*_map[row])[col]->SetScale(Vector2(newTileSize, newTileSize));
		}
	}
	m__player->SetMapPosition(playerTilePos * newTileSize, _tilemap->GetScrollOffset(), newTileSize);
	m__player->SetScale(Vector2(newTileSize, newTileSize));
}

void MVC_Model_Spectre::updateHUD(double dt)
{
	if (m_alertLevel > S_M_MAX_ALERT)
	{
		m_alertLevel = S_M_MAX_ALERT;
	}
	if (m_alertLevel > 0.f)
	{
		m_alertLevel -= dt * 0.2f;
		if (m_alertLevel < 0.f)
		{
			m_alertLevel = 0.f;
		}
	}
	m__alert->Update(dt, m_alertLevel, S_M_MAX_ALERT);
	m__spectreDive->Update(dt, Player::S_SPECTRE_DIVE_COOLDOWN - m__player->GetDiveTimer(), Player::S_SPECTRE_DIVE_COOLDOWN);
	m__spectreJump->Update(dt, Player::S_SPECTRE_JUMP_COOLDOWN - m__player->GetJumpTimer(), Player::S_SPECTRE_JUMP_COOLDOWN);
	if (!m__player->GetInShadow())
	{
		m__spectreHost->Update(dt, Player::S_SPECTRE_DIVE_COOLDOWN - m__player->GetDiveTimer(), Player::S_SPECTRE_DIVE_COOLDOWN);
	}
	else
	{
		m__spectreHost->Update(dt, 0.f, Player::S_SPECTRE_DIVE_COOLDOWN);
	}
}

void MVC_Model_Spectre::updateCamera(double dt)
{
	for (vector<SecurityCamera*>::iterator it = m_cameraList.begin(); it != m_cameraList.end(); ++it)
	{
		SecurityCamera* _camera = (SecurityCamera*)*it;
		if (_camera) // If camera exists, update it
		{
			_camera->Update(dt, m__currentLevel->GetTileMap());
			_camera->SetMapPosition(_camera->GetMapPos(), m__currentLevel->GetTileMap()->GetScrollOffset(), m__currentLevel->GetTileMap()->GetTileSize());
		}
	}
}

void MVC_Model_Spectre::clearCameraList()
{
	while (m_cameraList.size() > 0)
	{
		SecurityCamera* _camera = m_cameraList.back();
		if (_camera)
		{
			delete _camera;
			m_cameraList.pop_back();
		}
	}
}

void MVC_Model_Spectre::loadToList(TileMap* _map)
{
	float tileSize = _map->GetTileSize();
	for (int row = 0; row < _map->GetNumMapTile().y; ++row)
	{
		for (int col = 0; col < _map->GetNumMapTile().x; ++col)
		{
			Tile::E_TILE_TYPE tileType = (*_map->GetMap()[row])[col]->GetType();
			SecurityCamera* _camera = NULL;
			switch (tileType)
			{
			case Tile::TILE_OBJ_CAMERA_ON_1_1:
				{
					_camera = new SecurityCamera(Vector2(col * tileSize, row * tileSize), Direction::DIRECTIONS[Direction::DIR_DOWN], true, _map->GetTileSize());
					_camera->SetMapPosition(_camera->GetMapPos(), _map->GetScrollOffset(), _map->GetTileSize());
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_ON_DOWN]);
					m_cameraList.push_back(_camera);
				}
				break;
			case Tile::TILE_OBJ_CAMERA_ON_1_2:
				{
					_camera = new SecurityCamera(Vector2(col * tileSize, row * tileSize), Direction::DIRECTIONS[Direction::DIR_LEFT], true, _map->GetTileSize());
					_camera->SetMapPosition(_camera->GetMapPos(), _map->GetScrollOffset(), _map->GetTileSize());
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_ON_LEFT]);
					m_cameraList.push_back(_camera);
				}
				break;
			case Tile::TILE_OBJ_CAMERA_ON_1_3:
				{
					_camera = new SecurityCamera(Vector2(col * tileSize, row * tileSize), Direction::DIRECTIONS[Direction::DIR_UP], true, _map->GetTileSize());
					_camera->SetMapPosition(_camera->GetMapPos(), _map->GetScrollOffset(), _map->GetTileSize());
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_ON_UP]);
					m_cameraList.push_back(_camera);
				}
				break;
			case Tile::TILE_OBJ_CAMERA_ON_1_4:
				{
					_camera = new SecurityCamera(Vector2(col * tileSize, row * tileSize), Direction::DIRECTIONS[Direction::DIR_RIGHT], true, _map->GetTileSize());
					_camera->SetMapPosition(_camera->GetMapPos(), _map->GetScrollOffset(), _map->GetTileSize());
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_ON_RIGHT]);
					m_cameraList.push_back(_camera);
				}
				break;
			case Tile::TILE_OBJ_CAMERA_OFF_1_1:
				{
					_camera = new SecurityCamera(Vector2(col * tileSize, row * tileSize), Direction::DIRECTIONS[Direction::DIR_DOWN], false, _map->GetTileSize());
					_camera->SetMapPosition(_camera->GetMapPos(), _map->GetScrollOffset(), _map->GetTileSize());
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_DOWN]);
					m_cameraList.push_back(_camera);
				}
				break;
			case Tile::TILE_OBJ_CAMERA_OFF_1_2:
				{
					_camera = new SecurityCamera(Vector2(col * tileSize, row * tileSize), Direction::DIRECTIONS[Direction::DIR_LEFT], false, _map->GetTileSize());
					_camera->SetMapPosition(_camera->GetMapPos(), _map->GetScrollOffset(), _map->GetTileSize());
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_LEFT]);
					m_cameraList.push_back(_camera);
				}
				break;
			case Tile::TILE_OBJ_CAMERA_OFF_1_3:
				{
					_camera = new SecurityCamera(Vector2(col * tileSize, row * tileSize), Direction::DIRECTIONS[Direction::DIR_UP], false, _map->GetTileSize());
					_camera->SetMapPosition(_camera->GetMapPos(), _map->GetScrollOffset(), _map->GetTileSize());
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_UP]);
					m_cameraList.push_back(_camera);
				}
				break;
			case Tile::TILE_OBJ_CAMERA_OFF_1_4:
				{
					_camera = new SecurityCamera(Vector2(col * tileSize, row * tileSize), Direction::DIRECTIONS[Direction::DIR_RIGHT], false, _map->GetTileSize());
					_camera->SetMapPosition(_camera->GetMapPos(), _map->GetScrollOffset(), _map->GetTileSize());
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_RIGHT]);
					m_cameraList.push_back(_camera);
				}
				break;
			}
		}
	}
}
