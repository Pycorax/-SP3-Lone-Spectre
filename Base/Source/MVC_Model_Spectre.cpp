#include "MVC_Model_Spectre.h"

MVC_Model_Spectre::MVC_Model_Spectre(string configSONFile) : MVC_Model(configSONFile)
	, m_currentLevelID(0)
	, m__currentLevel(NULL)
	, m_hackMode(false)
	, m__player(NULL)
	, m_enableShadow(true)
	, m_alertLevel(0.f)
	, m_objective(NULL)
{
}


MVC_Model_Spectre::~MVC_Model_Spectre(void)
{
}

void MVC_Model_Spectre::processKeyAction(double dt) 
{
	// Controls for Spectre HexTech minigame
	if (m_hackMode)
	{
		m_hackingGame.Move(m_bKeyPressed[MOVE_LEFT_KEY], m_bKeyPressed[MOVE_RIGHT_KEY], m_bKeyPressed[MOVE_FORWARD_KEY], m_bKeyPressed[MOVE_BACKWARD_KEY], dt);
	}
	else
	{
		#pragma region Player Controls
		
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

			if (m_bKeyPressed[INTERACT_SKILL_2_KEY]) // Spectral Hax
			{
				if (m__player->Interact(Player::INTERACT_HAX, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_HAX)
				{
					m__player->SetState(Player::PS_SPECTRAL_HAX);
					startHackMode();
				}
				else if (m__player->Interact(Player::INTERACT_ASSASSINATE, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_ASSASSINATE)
				{
					m_objective->Activate();
				}
				else if (m__player->Interact(Player::INTERACT_COLLECT, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_COLLECT)
				{
					m_objective->Activate();
				}
				else if (m__player->Interact(Player::INTERACT_DEFUSE, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_DEFUSE)
				{
					m_objective->Activate();
				}
				else if (m__player->Interact(Player::INTERACT_SETBOMB, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_SETBOMB)
				{
					if (m_objective != NULL)
					{
						m_objective->Activate();
					}
				}
			}

			if (m_bKeyPressed[INTERACT_SKILL_1_KEY] && m__player->Interact(Player::INTERACT_DIVE, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_DIVE) // Spectral Dive
			{
				m__player->SetDive();
				//if player currently not hosting
				if(m__player->GetHosting() == false)
				{
					for (vector<Enemy*>::iterator enemyIter = m_enemyList.begin(); enemyIter != m_enemyList.end(); ++enemyIter)
					{
						Enemy* _enemy = *enemyIter;
						//if player dives within a tile distance from player
						if((m__player->GetMapPos() - _enemy->GetMapPos()).LengthSquared() <= m__currentLevel->GetTileMap()->GetTileSize() *  m__currentLevel->GetTileMap()->GetTileSize())
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

		#pragma endregion
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
	// Delete the previous level
	if (m__currentLevel != NULL)
	{
		delete m__currentLevel;
	}

	// Initialize the level
	m__currentLevel = new Level();
	//m__currentLevel->InitMap(Vector2(64, 50), m_viewWidth, m_viewHeight, 64, "TileMap//Level1.csv", meshList);
	m__currentLevel->Load(levelMapFile, m_viewWidth, m_viewHeight, meshList);
	SecurityCamera::InitCamMeshList(meshList);
	loadToList(m__currentLevel->GetTileMap());
	
	// Setup camera view
	for (vector<SecurityCamera*>::iterator it = m_cameraList.begin(); it != m_cameraList.end(); ++it)
	{
		SecurityCamera* _camera = (SecurityCamera*)*it;
		if (_camera)
		{
			_camera->GenerateViewBox(m__currentLevel->GetTileMap());
		}
	}
	

	// Initialize the player
	int tileSize = m__currentLevel->GetTileMap()->GetTileSize();
	m__player->SetMapPosition(m__currentLevel->GetTileMap()->GetScreenSize() * 0.5f, Vector2(0, 0), m__currentLevel->GetTileMap()->GetTileSize()); // Start at center with no scroll offset
	m__player->SetScale(Vector3(tileSize, tileSize));

	// Initialize the enemies
	vector<Enemy*> enemies = m__currentLevel->GetEnemyList();
	for (vector<Enemy*>::iterator enemyIT = enemies.begin(); enemyIT != enemies.end(); ++enemyIT)
	{
		Enemy* _enemy = new Enemy(**enemyIT);

		_enemy->initPathFinder(m__currentLevel->GetTileMap());
		_enemy->SetTarget(m__player->GetMapPos(), m__currentLevel->GetTileMap()->GetTileSize());

		m_enemyList.push_back(_enemy);
	}
}

void MVC_Model_Spectre::updateMessenger(double dt)
{
	// Timer for displaying 
	static const double MESSAGE_DELAY = 5.0f;			// TODO: Move this into message struct
	static double timer = MESSAGE_DELAY;
	timer += dt;

	if (timer > MESSAGE_DELAY)
	{
		// Store the message objects into this
		m_messageObjects = m_messenger.GetMessageObjects(m_viewWidth, m_viewHeight);
		timer = 0.0f;
	}
}

void MVC_Model_Spectre::pushMessageToRender(void)
{
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

void MVC_Model_Spectre::updateHackMode(const double DT)
{
	if (m_hackMode)
	{
		m_hackingGame.Update(DT);
	}
}

void MVC_Model_Spectre::startHackMode(void)
{
	m_hackMode = true;
	m_hackingGame.Reset(m_viewWidth, m_viewHeight);
}

void MVC_Model_Spectre::Init(void)
{
	MVC_Model::Init();

	// -- Load Shadow GameObject
	m__tileMarkerMesh[TM_SHADOW] = GetMeshResource("ShadowOverlay");
	m__tileMarkerMesh[TM_VIEWED] = GetMeshResource("LightOverlay");

	// Load the player
	InitPlayer();

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

	// Init the MessageManager
	m_messenger.Init(GetMeshResource("MessageBG"), m_defaultFont, m_defaultFont, Vector2(600.0f, 200.0f), Vector2(20.0f, 20.0f));
	m_messenger.AddMessages("Messages//Level1_Message.son");

	//Enemy
	Enemy* _enemy = new Enemy;
	//_enemy->SetMapPosition(m__currentLevel->GetTileMap()->GetScreenSize() * 0.5f, m__currentLevel->GetTileMap()->GetScrollOffset(),tileSize);
	_enemy->SetMapTilePosition(Vector2(4,5),Vector2(0, 32), tileSize);
	_enemy->Init(_enemy->GetMapPos() , GetMeshResource("Enemy_ANIMATION"));
	_enemy->SetScale(Vector2(tileSize, tileSize));
	_enemy->initPathFinder(m__currentLevel->GetTileMap());
	_enemy->SetTarget(m__player->GetMapPos(), tileSize);
	//patrol points per enemy
	_enemy->AddPatrolPoint(_enemy->GetMapTilePos() + Vector2(0, -5));
	_enemy->AddPatrolPoint(_enemy->GetMapTilePos() + Vector2(0,  2));
	_enemy->AddPatrolPoint(_enemy->GetMapTilePos() + Vector2(7,  2));
	_enemy->AddPatrolPoint(_enemy->GetMapTilePos() + Vector2(7, -5));
	// Set up enemy's view
	_enemy->InitViewer(1, 2);
	m_enemyList.push_back(_enemy);
}

void MVC_Model_Spectre::InitPlayer(void)
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

	//m__player->SetMapPosition(m__currentLevel->GetTileMap()->GetScreenSize() * 0.5f, Vector2(0,0), m__currentLevel->GetTileMap()->GetTileSize()); // Start at center with no scroll offset
	//m__player->SetScale(Vector3(m__currentLevel->GetTileMap()->GetTileSize(), m__currentLevel->GetTileMap()->GetTileSize()));
}

void MVC_Model_Spectre::Update(double dt)
{
	MVC_Model::Update(dt);

	if (m_hackMode)
	{
		m_hackingGame.Update(dt);

		if (m_hackingGame.IsVictory())
		{
			m_hackMode = false;
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
			m_hackMode = false;
			m__player->SetState(Player::PS_IDLE_DOWN);
		}

		vector<GameObject2D*> minigameObjects = m_hackingGame.GetRenderObjects();
		for (vector<GameObject2D*>::iterator go = minigameObjects.begin(); go != minigameObjects.end(); ++go)
		{
			m_renderList2D.push(*go);
		}
	}
	//not in mini game - update normally
	else
	{	
		// Updates player depending on actions queued.
		m__player->Update(dt, m__currentLevel->GetTileMap());

		// Update messages
		updateMessenger(dt);

		// Update camera list
		updateCamera(dt);

		// Player inside viewed tile
		Tile* _tile = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos());
		if (_tile->IsViewed() && !m__player->GetInShadow()) // Player is viewed by some
		{
			_tile->NotifyViewer(m__player->GetMapTilePos(), m_alertLevel, dt);
		}

		//update enemies
		for (vector<Enemy*>::iterator enemyIter = m_enemyList.begin(); enemyIter != m_enemyList.end(); ++enemyIter)
		{
			(*enemyIter)->Update(dt, m__currentLevel->GetTileMap());
			(*enemyIter)->SetTarget(m__player->GetMapPos(), m__currentLevel->GetTileMap()->GetTileSize());
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

		// Rendering
		tileMapToRender(m__currentLevel->GetTileMap());
		
		// -- Render Player
		m_renderList2D.push(m__player);

		// Render Enemies
		for (vector<Enemy*>::iterator enemyIter = m_enemyList.begin(); enemyIter != m_enemyList.end(); ++enemyIter)
		{
			m_renderList2D.push((*enemyIter));
		}

		// Render Messages
		pushMessageToRender();

		m_renderList2D.push(m_fpsCount);
	}
}
void MVC_Model_Spectre::Exit(void)
{
	m_hackingGame.Exit();

	Player::Clear();

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

	// Clear the level
	if (m__currentLevel != NULL)
	{
		delete m__currentLevel;
	}
	
	MVC_Model::Exit();
}

void MVC_Model_Spectre::tileMapToRender(TileMap* _ToRender)
{
	static const Vector2 S_MAX_SCROLL_SIZE_TILE = _ToRender->GetNumMapTile(); // Max tile difference between map and screen
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
			if (tileStart.y + row < 0 || tileStart.y + row >= S_MAX_SCROLL_SIZE_TILE.y || tileStart.x + col < 0 || tileStart.x + col >= S_MAX_SCROLL_SIZE_TILE.x)
			{
				continue;
			}
			Tile* _tile = (*_map[tileStart.y + row])[tileStart.x + col]; // Get the tile data based on loop
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

void MVC_Model_Spectre::onResolutionChanged(int oldViewWidth, int oldViewHeight)
{
	resizeTileMap(oldViewWidth);
}

void MVC_Model_Spectre::resizeTileMap(int oldViewWidth)
{
	TileMap* _tilemap = m__currentLevel->GetTileMap();
	vector<vector<Tile*>*> _map = _tilemap->GetMap();
	float tileSize = _tilemap->GetTileSize();
	Vector2 playerTilePos(floor(m__player->GetMapPos().x / tileSize), floor(m__player->GetMapPos().y / tileSize));
	Vector2 mapScrollOffset(ceil(_tilemap->GetScrollOffset().x / tileSize), ceil(_tilemap->GetScrollOffset().y / tileSize));
	if (oldViewWidth < m_viewWidth) // Scale up screen
	{
		++mapScrollOffset.y;
		_tilemap->SetTileSize(m_viewWidth / _tilemap->GetNumScreenTile().x);
	}
	else if (oldViewWidth > m_viewWidth) // Scale down screen
	{
		--mapScrollOffset.y;
		_tilemap->SetTileSize(m_viewWidth / _tilemap->GetNumScreenTile().x);
	}
	tileSize = _tilemap->GetTileSize();
	for (int row = 0; row < _tilemap->GetNumMapTile().y; ++row)
	{
		for (int col = 0; col < _tilemap->GetNumMapTile().x; ++col)
		{
			_tilemap->SetScrollOffset(mapScrollOffset * tileSize);
			_tilemap->SetMapSize(_tilemap->GetNumMapTile() * tileSize);
			(*_map[row])[col]->SetMapPosition(Vector2(col * tileSize, row * tileSize), _tilemap->GetScrollOffset(), _tilemap->GetTileSize());
			(*_map[row])[col]->SetScale(Vector2(tileSize, tileSize));
			m__player->SetMapPosition(playerTilePos * tileSize, _tilemap->GetScrollOffset(), _tilemap->GetTileSize());
			m__player->SetScale(Vector2(tileSize, tileSize));
		}
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
