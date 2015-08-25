#include "MVC_Model_Spectre.h"

MVC_Model_Spectre::MVC_Model_Spectre(string configSONFile) : MVC_Model(configSONFile)
	, m__testLevel(NULL)
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
				m__player->SetMove(Character::directions[Character::DIR_UP]);
			}
			if (m_bKeyPressed[MOVE_BACKWARD_KEY])
			{
				m__player->SetMove(Character::directions[Character::DIR_DOWN]);
			}
			if (m_bKeyPressed[MOVE_LEFT_KEY])
			{
				m__player->SetMove(Character::directions[Character::DIR_LEFT]);
			}
			if (m_bKeyPressed[MOVE_RIGHT_KEY])
			{
				m__player->SetMove(Character::directions[Character::DIR_RIGHT]);
			}

			if (m_bKeyPressed[INTERACT_SKILL_2_KEY]) // Spectral Hax
			{
				switch (m__player->Interact(Player::INTERACT_HAX, m__testLevel->GetTileMap()))
				{
				case Player::PS_SPECTRAL_HAX:
					// TODO: Get a pointer to the camera so as to change it
					m__player->SetState(Player::PS_SPECTRAL_HAX);
					startHackMode();
					break;
				}
			}

			if (m_bKeyPressed[INTERACT_SKILL_1_KEY] && m__player->Interact(Player::INTERACT_DIVE, m__testLevel->GetTileMap()) == Player::PS_SPECTRAL_DIVE) // Spectral Dive
			{
				m__player->SetDive();
			}

			if (m_bKeyPressed[MOVE_JUMP_KEY] && m__player->Interact(Player::INTERACT_JUMP, m__testLevel->GetTileMap()) == Player::PS_SPECTRAL_JUMP) // Spectral Jump
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

void MVC_Model_Spectre::resetLightResources(void)
{
	for (vector<GameObject2D*>::iterator light = m__tileMarkers.begin(); light != m__tileMarkers.end(); ++light)
	{
		(*light)->SetActive(false);
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

	// Load the map
	m__testLevel = new Level();
	m__testLevel->InitMap(Vector2(64, 50), Vector2(ceil(m_viewWidth / 64.f), ceil(m_viewHeight / 64.f)), 64, "TileMap//Level1.csv", meshList);
	int tileSize = m__testLevel->GetTileMap()->GetTileSize();
	// -- Load Shadow GameObject
	m__tileMarkerMesh[TM_SHADOW] = GetMeshResource("ShadowOverlay");
	m__tileMarkerMesh[TM_VIEWED] = GetMeshResource("LightOverlay");

	// Load the player
	m__player = Player::GetInstance();
	m__player->Init(GetMeshResource("Player"));
	m__player->SetMapPosition(m__testLevel->GetTileMap()->GetScreenSize() * 0.5f, Vector2(0,0), m__testLevel->GetTileMap()->GetTileSize()); // Start at center with no scroll offset
	m__player->SetScale(Vector3(tileSize, tileSize));

	// Load debugging test object
	m__testGO = new GameObject2D;
	m__testGO->SetMesh(GetMeshResource("Quad"));
	m__testGO->SetPos(Vector2(100.0f, m_viewHeight * 0.5));
	m__testGO->SetScale(Vector2(32.f, 32.f));
	m__colliderList.push_back(dynamic_cast<Collider2D *>(m__testGO));

	// Init the hacking game
	m_hackingGame.Init(GetMeshResource("ShadowBall"), GetMeshResource("PlayerBall"), GetMeshResource("CircuitWall"), GetMeshResource("DestroyedWall"), GetMeshResource("RestrictedWall"), GetMeshResource("LoseScreen"), GetMeshResource("MinigameBG"), m_viewWidth, m_viewHeight);

	// Physics Testing
	m__po1 = new PhysicalObject;
	m__po1->SetPos(Vector2(300.0f, 100.0f));
	m__po1->SetScale(Vector2(50.f, 50.0f));
	m__po1->InitPhysics2D(1.0f, false, Vector2(0.0f, 50.0f));
	m__po1->SetMesh(GetMeshResource("Quad"));

	m__po2 = new PhysicalObject;
	m__po2->SetPos(Vector2(300.0f, 300.0f));
	m__po2->SetScale(Vector2(150.0f, 50.0f));
	m__po2->InitPhysics2D(1.0f, true, Vector2::ZERO_VECTOR, Vector2(0.0f, 1.0f));
	m__po2->SetMesh(GetMeshResource("Quad"));

	//Enemy
	m__testEnemy = new Enemy;
	m__testEnemy->SetMesh(GetMeshResource("ShadowBall"));
	m__testEnemy->SetMapPosition(Vector2 (500, 200), m__testLevel->GetTileMap()->GetScrollOffset(), m__testLevel->GetTileMap()->GetTileSize());
	m__testEnemy->SetScale(Vector2(32.f, 32.f));
	m__testEnemy->initPathFinder(m__testLevel->GetTileMap());
	m__testEnemy->SetTarget(m__player->GetMapPos(), m__testLevel->GetTileMap()->GetTileSize());//m__player->GetTransform().Translation);
	m__testEnemy->AddPatrolPoint(m__testEnemy->GetMapPos() - Vector2(0,20) );
	m__testEnemy->AddPatrolPoint(m__testEnemy->GetMapPos() + Vector2(0,60) );
	m__testEnemy->AddPatrolPoint(m__testEnemy->GetMapPos() + Vector2(40,20) );
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
			m__player->SetState(Player::PS_IDLE);
			// TODO: Do an action for when the mini game ends in a win
		}
		else if (m_hackingGame.IsLoss())
		{
			m_hackMode = false;
			m__player->SetState(Player::PS_IDLE);
			// TODO: Do an action for when the mini game ends in a loss
		}

		vector<GameObject2D*> minigameObjects = m_hackingGame.GetRenderObjects();
		for (vector<GameObject2D*>::iterator go = minigameObjects.begin(); go != minigameObjects.end(); ++go)
		{
			m_renderList2D.push(*go);
		}
	}
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
		m__player->Update(dt, m__testLevel->GetTileMap());

		//update enemy;
		m__testEnemy->Update(dt, m__testLevel->GetTileMap());

		Vector3 pos = m__testGO->GetTransform().Translation;
		pos += Vector3(50.0f * dt);
		m__testGO->SetPos(pos);

		//po1->SetColliderType(Collider2D::CT_AABB);
		m__po1->UpdatePhysics(dt);
		m__po2->UpdatePhysics(dt);

		if (m__po1->CollideWith(m__po2, dt))
		{
			m__po1->CollideRespondTo(m__po2);
		}

		// Rendering
		m__testLevel->GetTileMap()->UpdateLighting();
		tileMapToRender(m__testLevel->GetTileMap());
		m_renderList2D.push(m__testGO);
		if (m__player->GetInShadow())
		{
			m__player->SetMesh(GetMeshResource("ShadowPlayer"));
		}
		else
		{
			m__player->SetMesh(GetMeshResource("Player"));
		}
		m_renderList2D.push(m__player);
		m_renderList2D.push(m__po1);
		m_renderList2D.push(m__po2);
		m_renderList2D.push(m__testEnemy);
	}
}

void MVC_Model_Spectre::Exit(void)
{
	m_hackingGame.Exit();

	if (m__testGO)
	{
		delete m__testGO;
		m__testGO = NULL;
	}

	Player::Clear();

	//clearing list
	while(m__colliderList.size() > 0)
	{
		Collider2D *go = m__colliderList.back();
		delete go;
		m__colliderList.pop_back();
	}
	delete m__testEnemy;
	m__testEnemy = NULL;
	MVC_Model::Exit();
}

void MVC_Model_Spectre::tileMapToRender(TileMap* _ToRender)
{
	static const Vector2 S_MAX_SCROLL_SIZE_TILE = _ToRender->GetNumMapTile(); // Max tile difference between map and screen
	vector<vector<Tile*>*> _map = _ToRender->GetMap();

	// Reset the lights from the previous frame rendered
	resetLightResources();

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
			_tile->SetMapPosition(_tile->GetMapPos(), _ToRender->GetScrollOffset(), m__testLevel->GetTileMap()->GetTileSize()); // Calculate screen position based on map position for rendering
			m_renderList2D.push(_tile); // Add to queue for rendering

			/*
			 * Shadow Portion
			 */
			if (m_enableShadow)
			{
				int numShadows = Tile::MAX_LIGHT_LEVEL - _tile->GetLightLevel();
				for (size_t lightLevel = 0; lightLevel < numShadows; lightLevel += 3/*TODO: Remove hardcode. Should be attenuation value.*/)
				{
					Transform tileT = _tile->GetTransform();
					GameObject2D* light = fetchTileMarker(TM_SHADOW);
					light->SetPos(tileT.Translation);
					light->SetScale(tileT.Scale);
					m_renderList2D.push(light);
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
	TileMap* _tilemap = m__testLevel->GetTileMap();
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
