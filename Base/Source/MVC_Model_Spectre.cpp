#include "MVC_Model_Spectre.h"

MVC_Model_Spectre::MVC_Model_Spectre(string configSONFile) : MVC_Model(configSONFile)
	, m_currentLevelID(0)
	, m__currentLevel(NULL)
	, m_hackMode(false)
	, m__player(NULL)
	, m_enableShadow(true)
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
				m__player->SetMove(Character::S_DIRECTION[Character::DIR_UP]);
			}
			if (m_bKeyPressed[MOVE_BACKWARD_KEY])
			{
				m__player->SetMove(Character::S_DIRECTION[Character::DIR_DOWN]);
			}
			if (m_bKeyPressed[MOVE_LEFT_KEY])
			{
				m__player->SetMove(Character::S_DIRECTION[Character::DIR_LEFT]);
			}
			if (m_bKeyPressed[MOVE_RIGHT_KEY])
			{
				m__player->SetMove(Character::S_DIRECTION[Character::DIR_RIGHT]);
			}

			if (m_bKeyPressed[INTERACT_SKILL_2_KEY]) // Spectral Hax
			{
				switch (m__player->Interact(Player::INTERACT_HAX, m__currentLevel->GetTileMap()))
				{
				case Player::PS_SPECTRAL_HAX:
					// TODO: Get a pointer to the camera so as to change it
					m__player->SetState(Player::PS_SPECTRAL_HAX);
					startHackMode();
					break;
				}
			}

			if (m_bKeyPressed[INTERACT_SKILL_1_KEY] && m__player->Interact(Player::INTERACT_DIVE, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_DIVE) // Spectral Dive
			{
				m__player->SetDive();
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

	resolution.Set(m_viewWidth, m_viewHeight);
	
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

	//Enemy
	//Enemy* _enemy = new Enemy;
	//_enemy->SetMesh(GetMeshResource("ShadowBall"));
	//_enemy->SetMapPosition(Vector2 (500, 200), m__currentLevel->GetTileMap()->GetScrollOffset(), m__currentLevel->GetTileMap()->GetTileSize());
	//_enemy->SetScale(Vector2(64.f, 64.f));
	//_enemy->initPathFinder(m__currentLevel->GetTileMap());
	//_enemy->SetTarget(m__player->GetMapPos(), m__currentLevel->GetTileMap()->GetTileSize());//m__player->GetTransform().Translation);
	//_enemy->AddPatrolPoint(_enemy->GetMapPos() - Vector2(0,20));
	//_enemy->AddPatrolPoint(_enemy->GetMapPos() + Vector2(0,60));
	//_enemy->AddPatrolPoint(_enemy->GetMapPos() + Vector2(40,20));
	//m_enemyList.push_back(_enemy);
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
	_a->Set(12, 17, 1, 0.2f);
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVING_DOWN);
	//diving right
	_a = new Animation();
	_a->Set(18, 23, 1, 0.2f);
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVING_RIGHT);
	//diving left
	_a = new Animation();
	_a->Set(24, 29, 1, 0.2f);
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVING_LEFT);
	//diving north
	_a = new Animation();
	_a->Set(30, 35, 1, 0.2f);
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

	// Update tile size to fit screen resolution
	if (resolution.x != m_viewWidth || resolution.y != m_viewHeight)
	{
		TileMap* _tilemap = m__currentLevel->GetTileMap();
		vector<vector<Tile*>*> _map = _tilemap->GetMap();
		float tileSize = _tilemap->GetTileSize();
		Vector2 playerTilePos(floor(m__player->GetMapPos().x / tileSize), floor(m__player->GetMapPos().y / tileSize));
		Vector2 mapScrollOffset(ceil(_tilemap->GetScrollOffset().x / tileSize), ceil(_tilemap->GetScrollOffset().y / tileSize));
		if (resolution.x < m_viewWidth) // Scale up screen
		{
			++mapScrollOffset.y;
			_tilemap->SetTileSize(m_viewWidth / _tilemap->GetNumScreenTile().x);
		}
		else if (resolution.x > m_viewWidth) // Scale down screen
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
		resolution.Set(m_viewWidth, m_viewHeight);
	}

	if (m_hackMode)
	{
		m_hackingGame.Update(dt);

		if (m_hackingGame.IsVictory())
		{
			m_hackMode = false;
			m__player->SetState(Player::PS_IDLE_DOWN);
			// TODO: Do an action for when the mini game ends in a win
		}
		else if (m_hackingGame.IsLoss())
		{
			m_hackMode = false;
			m__player->SetState(Player::PS_IDLE_DOWN);
			// TODO: Do an action for when the mini game ends in a loss
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
		// Update tile size to fit screen resolution
		if (resolution.x != m_viewWidth || resolution.y != m_viewHeight)
		{
			resizeScreen();
		}

		//updates sprite animation
		/*SpriteAnimation* _sa = dynamic_cast<SpriteAnimation* >(m__player->GetMesh());
		{
			if(_sa)
			{
				_sa->Update(dt);
			}
		}*/
		//Updates player depending on actions queued.
		m__player->Update(dt, m__currentLevel->GetTileMap());

		//update enemies
		for (vector<Enemy*>::iterator enemyIter = m_enemyList.begin(); enemyIter != m_enemyList.end(); ++enemyIter)
		{
			(*enemyIter)->Update(dt, m__currentLevel->GetTileMap());
		}

		// Update Lighting
		vector<Vector2> shadowCasters;
		shadowCasters.push_back(m__player->GetMapTilePos()); 
		m__currentLevel->GetTileMap()->UpdateLighting(shadowCasters);

		// Rendering
		tileMapToRender(m__currentLevel->GetTileMap());
		// -- Render Player
		m_renderList2D.push(m__player);

		// -- MiniGame
		if (m_hackMode)
		{
			if (m_hackingGame.IsVictory())
			{
				m_hackMode = false;
				// TODO: Do an action for when the mini game ends in a win
			}
			else if (m_hackingGame.IsLoss())
			{
				m_hackMode = false;
				// TODO: Do an action for when the mini game ends in a loss
			}

			vector<GameObject2D*> minigameObjects = m_hackingGame.GetRenderObjects();
			for (vector<GameObject2D*>::iterator go = minigameObjects.begin(); go != minigameObjects.end(); ++go)
			{				
				m_renderList2D.push(*go);
			}
		}

		// Render Enemies
		for (vector<Enemy*>::iterator enemyIter = m_enemyList.begin(); enemyIter != m_enemyList.end(); ++enemyIter)
		{
			m_renderList2D.push((*enemyIter));
		}
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

void MVC_Model_Spectre::resizeScreen()
{
	TileMap* _tilemap = m__currentLevel->GetTileMap();
	vector<vector<Tile*>*> _map = _tilemap->GetMap();
	float tileSize = _tilemap->GetTileSize();
	Vector2 playerTilePos(floor(m__player->GetMapPos().x / tileSize), floor(m__player->GetMapPos().y / tileSize));
	Vector2 mapScrollOffset(ceil(_tilemap->GetScrollOffset().x / tileSize), ceil(_tilemap->GetScrollOffset().y / tileSize));
	if (resolution.x < m_viewWidth) // Scale up screen
	{
		++mapScrollOffset.y;
		_tilemap->SetTileSize(m_viewWidth / _tilemap->GetNumScreenTile().x);
	}
	else if (resolution.x > m_viewWidth) // Scale down screen
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
	resolution.Set(m_viewWidth, m_viewHeight);
}
