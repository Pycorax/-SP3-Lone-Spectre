#include "MVC_Model_Spectre.h"

MVC_Model_Spectre::MVC_Model_Spectre(string configSONFile) : MVC_Model(configSONFile)
	, m__testLevel(NULL)
	, m_hackMode(false)
{
	m__player = Player::GetInstance();
	m__player->Init();
}


MVC_Model_Spectre::~MVC_Model_Spectre(void)
{
}

void MVC_Model_Spectre::processKeyAction(double dt)
{
	if(m_bKeyPressed[MOVE_FORWARD_KEY]) //'W'
	{
		//queues player action 
		m__player->SetActions(m__player->PA_MOVE_UP, true);
	}
	if(m_bKeyPressed[MOVE_BACKWARD_KEY])//'S'
	{
		m__player->SetActions(m__player->PA_MOVE_DOWN, true);
	}
	if(m_bKeyPressed[MOVE_LEFT_KEY] ) //'A'
	{
		m__player->SetActions(m__player->PA_MOVE_LEFT, true);
	}
	if(m_bKeyPressed[MOVE_RIGHT_KEY] ) //'D'
	{
		m__player->SetActions(m__player->PA_MOVE_RIGHT, true);
	}

	// Scrolling map
	static const Vector2 S_MAX_SCROLL_SIZE = m__testLevel->GetTileMap()->GetMapSize() - m__testLevel->GetTileMap()->GetScreenSize();
	static const float S_OFFSET = 0.01;
	static const float S_SCROLL_SPEED = 500.f;
	if (m_bKeyPressed[LOOK_UP_KEY])
	{
		Vector2 scrollOffset = m__testLevel->GetTileMap()->GetScrollOffset() + Vector2(0, S_SCROLL_SPEED * dt);
		if (scrollOffset.y > S_MAX_SCROLL_SIZE.y)
		{
			//scrollOffset.y = S_MAX_SCROLL_SIZE.y - S_OFFSET;
		}
		m__testLevel->GetTileMap()->SetScrollOffset(scrollOffset);
	}
	else if (m_bKeyPressed[LOOK_DOWN_KEY])
	{
		Vector2 scrollOffset = m__testLevel->GetTileMap()->GetScrollOffset() + Vector2(0, -S_SCROLL_SPEED * dt);
		if (scrollOffset.y < 0)
		{
			//scrollOffset.y = 0;
		}
		m__testLevel->GetTileMap()->SetScrollOffset(scrollOffset);
	}
	if (m_bKeyPressed[LOOK_LEFT_KEY])
	{
		Vector2 scrollOffset = m__testLevel->GetTileMap()->GetScrollOffset() + Vector2(-S_SCROLL_SPEED * dt, 0);
		if (scrollOffset.x < 0)
		{
			//scrollOffset.x = 0;
		}
		m__testLevel->GetTileMap()->SetScrollOffset(scrollOffset);
	}
	else if (m_bKeyPressed[LOOK_RIGHT_KEY])
	{
		Vector2 scrollOffset = m__testLevel->GetTileMap()->GetScrollOffset() + Vector2(S_SCROLL_SPEED * dt, 0);
		if (scrollOffset.x > S_MAX_SCROLL_SIZE.x)
		{
			//scrollOffset.x = S_MAX_SCROLL_SIZE.x - S_OFFSET;
		}
		m__testLevel->GetTileMap()->SetScrollOffset(scrollOffset);
	}
	
	//updates player depending on actions queued.
	m__player->Update(dt,m__testLevel->GetTileMap());
	


	// Controls for Spectre HexTech minigame
	if (m_hackMode)
	{
		m_hackingGame.Move(m_bKeyPressed[MOVE_LEFT_KEY], m_bKeyPressed[MOVE_RIGHT_KEY], m_bKeyPressed[MOVE_FORWARD_KEY], m_bKeyPressed[MOVE_BACKWARD_KEY], dt);
	}

	// Quitting the game
	if (m_bKeyPressed[GAME_EXIT_KEY])
	{
		// TODO: Open a pause menu and then quit by that instead. Do actual pausing or return to menus
		m_running = false;
	}
}

void MVC_Model_Spectre::updateHackMode(const double DT)
{
	if (m_hackMode)
	{
		m_hackingGame.Update(DT);
	}
}

void MVC_Model_Spectre::Init(void)
{
	MVC_Model::Init();

	m__testLevel = new Level();
	m__testLevel->InitMap(Vector2(64,50), Vector2(40,22.5), 32, "TileMap//Level1.csv", meshList);
	//m_viewWidth = m__testLevel->GetTileMap()->GetScreenSize().x;
	//m_viewHeight = m__testLevel->GetTileMap()->GetScreenSize().y;

	m__testGO = new GameObject2D;
	m__testGO->SetMesh(GetMeshResource("Quad"));
	m__testGO->SetPos(Vector2(100.0f, m_viewHeight * 0.5));
	m__testGO->SetScale(Vector2(50.0f, 50.0f));

	// Init the hacking game
	m_hackingGame.Init(GetMeshResource("ShadowBall"), GetMeshResource("CircuitWall"), m_viewWidth, m_viewHeight);
}

void MVC_Model_Spectre::Update(double dt)
{
	MVC_Model::Update(dt);
	if (m_hackMode)
	{
		m_hackingGame.Update(dt);
	}

	Vector3 pos = m__testGO->GetTransform().Translation;
	pos += Vector3(50.0f * dt);
	m__testGO->SetPos(pos);

	// Rendering
	TileMapToRender(m__testLevel->GetTileMap());
	m_renderList2D.push(m__testGO);

	// -- MiniGame
	if (m_hackMode)
	{
		vector<GameObject2D*> minigameObjects = m_hackingGame.GetRenderObjects();
		for (vector<GameObject2D*>::iterator go = minigameObjects.begin(); go != minigameObjects.end(); ++go)
		{
			m_renderList2D.push(*go);
		}
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

	MVC_Model::Exit();
}

void MVC_Model_Spectre::TileMapToRender(TileMap* _ToRender)
{
	static const Vector2 S_MAX_SCROLL_SIZE_TILE = _ToRender->GetNumMapTile(); // Max tile difference between map and screen
	vector<vector<Tile*>*> _map = _ToRender->GetMap();

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
			_tile->SetMapPosition(_tile->GetMapPos(), _ToRender->GetScrollOffset()); // Calculate screen position based on map position for rendering
			m_renderList2D.push(_tile); // Add to queue for rendering
		}
	}
}
