#include "MVC_Model_Spectre.h"


MVC_Model_Spectre::MVC_Model_Spectre(string configSONFile) : MVC_Model(configSONFile), m__testLevel(NULL)
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
	if(m_bKeyPressed[INTERACT_NEXT_KEY]) // 'E'
	{
		m__player->SetActions(m__player->PA_INTERACT, true);
	}
	//updates player depending on actions queued.
	m__player->Update(dt,m__testLevel->GetTileMap());
	
	// Quitting the game
	if (m_bKeyPressed[GAME_EXIT_KEY])
	{
		// TODO: Open a pause menu and then quit by that instead. Do actual pausing or return to menus
		m_running = false;
	}
}

void MVC_Model_Spectre::Init(void)
{
	MVC_Model::Init();

	m__testLevel = new Level();
	m__testLevel->InitMap(Vector2(64,50), Vector2(32,25), 32, "TileMap//Level1.csv", meshList);
	m_viewWidth = m__testLevel->GetTileMap()->GetScreenSize().x;
	m_viewHeight = m__testLevel->GetTileMap()->GetScreenSize().y;

	m__testGO = new GameObject2D;
	m__testGO->SetMesh(GetMeshResource("Quad"));
	m__testGO->SetPos(Vector2(100.0f, m_viewHeight * 0.5));
	m__testGO->SetScale(Vector2(50.0f, 50.0f));
}

void MVC_Model_Spectre::Update(double dt)
{
	MVC_Model::Update(dt);

	Vector3 pos = m__testGO->GetTransform().Translation;

	pos += Vector3(50.0f * dt);

	// Rendering
	TileMapToRender(m__testLevel->GetTileMap());

	m__testGO->SetPos(pos);

	m_renderList2D.push(m__testGO);

}

void MVC_Model_Spectre::Exit(void)
{
	if (m__testGO)
	{
		delete m__testGO;
		m__testGO = NULL;
	}

	MVC_Model::Exit();
}

void MVC_Model_Spectre::TileMapToRender(TileMap* _ToRender)
{
	vector<vector<Tile*>*> _map = _ToRender->GetMap();

	// Calc the starting tile to render and round down any decimal as it is still seen
	Vector2 tileStart
		( 
			floor(_ToRender->GetScrollOffset().x / _ToRender->GetTileSize()),
			floor(_ToRender->GetScrollOffset().y / _ToRender->GetTileSize()) 
		); 

	for (int row = 0; row < _ToRender->GetNumScreenTile().y + 1; ++row)			// Loop for rows
	{
		for (int col = 0; col < _ToRender->GetNumScreenTile().x + 1; ++col)	// Loop for columns (+1 for the offset)
		{
			if (row >= _ToRender->GetNumMapTile().y || col >= _ToRender->GetNumMapTile().x) // Stop the rendering if row and col goes out of map
			{
				break;
			}
			Tile* _tile = (*_map[tileStart.y + row])[tileStart.x + col]; // Get the tile data based on loop
			_tile->SetMapPosition(_tile->GetMapPos(), _ToRender->GetScrollOffset()); // Calculate screen position based on map position for rendering
			m_renderList2D.push(_tile); // Add to queue for rendering
		}
	}
}