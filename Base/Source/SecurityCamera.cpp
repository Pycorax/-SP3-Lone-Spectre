#include "SecurityCamera.h"

const float SecurityCamera::S_M_CAMERA_MAX_TIMER = 30.f;
Mesh* SecurityCamera::s_camMeshList[NUM_CAM_MESH];

void SecurityCamera::InitCamMeshList(const vector<Mesh*>& meshList)
{
	for (int i = 0; i < NUM_CAM_MESH; ++i)
	{
		s_camMeshList[i] = NULL;
	}
	for (int mesh = 0; mesh < meshList.size(); ++mesh)
	{
		if (meshList[mesh]->name == "TILE_OBJ_CAMERA_ON_1_1")
		{
			s_camMeshList[CAM_ON_DOWN] = meshList[mesh];
		}
		else if (meshList[mesh]->name == "TILE_OBJ_CAMERA_ON_1_2")
		{
			s_camMeshList[CAM_ON_LEFT] = meshList[mesh];
		}
		else if (meshList[mesh]->name == "TILE_OBJ_CAMERA_ON_1_3")
		{
			s_camMeshList[CAM_ON_UP] = meshList[mesh];
		}
		else if (meshList[mesh]->name == "TILE_OBJ_CAMERA_ON_1_4")
		{
			s_camMeshList[CAM_ON_RIGHT] = meshList[mesh];
		}
		else if (meshList[mesh]->name == "TILE_OBJ_CAMERA_OFF_1_1")
		{
			s_camMeshList[CAM_OFF_DOWN] = meshList[mesh];
		}
		else if (meshList[mesh]->name == "TILE_OBJ_CAMERA_OFF_1_2")
		{
			s_camMeshList[CAM_OFF_LEFT] = meshList[mesh];
		}
		else if (meshList[mesh]->name == "TILE_OBJ_CAMERA_OFF_1_3")
		{
			s_camMeshList[CAM_OFF_UP] = meshList[mesh];
		}
		else if (meshList[mesh]->name == "TILE_OBJ_CAMERA_OFF_1_4")
		{
			s_camMeshList[CAM_OFF_RIGHT] = meshList[mesh];
		}
	}
}

SecurityCamera::SecurityCamera(Vector2 mapPos, Vector2 dir, bool state, float tileSize, int maxRangeX, int maxRangeY) : MapEntity(mapPos), m_dir(dir), m_state(state), Viewer(maxRangeX, maxRangeY), m_timer(S_M_CAMERA_MAX_TIMER)
{
	SetScale(Vector2(tileSize, tileSize));
}

SecurityCamera::~SecurityCamera(void)
{
}

void SecurityCamera::SetDir(Vector2 dir)
{
	this->m_dir = dir;
}

void SecurityCamera::SetState(bool state)
{
	this->m_state = state;
}

Vector2 SecurityCamera::GetDir()
{
	return m_dir;
}

bool SecurityCamera::GetState()
{
	return m_state;
}

Vector2 SecurityCamera::viewer_GetTilePos(void)
{
	return GetMapTilePos();
}

Vector2 SecurityCamera::viewer_GetDirection(void)
{
	return m_dir;
}

void SecurityCamera::SpottedTarget(Vector2 targetTilePos, float &alertLevel, double dt)
{
	alertLevel += dt;
	cout << "Spotted" << endl;
}

void SecurityCamera::updateScreenPos(Vector2 pos)
{
	m_transforms.Translation = pos;
}

void SecurityCamera::Init(Vector2 dir, bool state, int maxRangeX, int maxRangeY)
{
	Viewer::Init(maxRangeX, maxRangeY);
	this->m_dir = dir;
	this->m_state = state;
	this->m_timer = S_M_CAMERA_MAX_TIMER;
}

void SecurityCamera::Update(double dt, TileMap* _map)
{
	if (!m_state)
	{
		m_timer -= dt;
		if (m_timer <= 0.f) // Activate camera and reset timer for next use
		{
			m_state = true;
			if (m_dir == Direction::DIRECTIONS[Direction::DIR_UP])
			{
				SetMesh(s_camMeshList[CAM_ON_UP]);
				// Force set tilemap
				Tile* _tile = _map->GetTileAt(GetMapPos());
				_tile->SetMesh(s_camMeshList[CAM_ON_UP]);
				_tile->SetType(Tile::TILE_OBJ_CAMERA_ON_1_3);
			}
			else if (m_dir == Direction::DIRECTIONS[Direction::DIR_DOWN])
			{
				SetMesh(s_camMeshList[CAM_ON_DOWN]);
				// Force set tilemap
				Tile* _tile = _map->GetTileAt(GetMapPos());
				_tile->SetMesh(s_camMeshList[CAM_ON_DOWN]);
				_tile->SetType(Tile::TILE_OBJ_CAMERA_ON_1_1);
			}
			else if (m_dir == Direction::DIRECTIONS[Direction::DIR_LEFT])
			{
				SetMesh(s_camMeshList[CAM_ON_LEFT]);
				// Force set tilemap
				Tile* _tile = _map->GetTileAt(GetMapPos());
				_tile->SetMesh(s_camMeshList[CAM_ON_LEFT]);
				_tile->SetType(Tile::TILE_OBJ_CAMERA_ON_1_2);
			}
			else if (m_dir == Direction::DIRECTIONS[Direction::DIR_RIGHT])
			{
				SetMesh(s_camMeshList[CAM_ON_RIGHT]);
				// Force set tilemap
				Tile* _tile = _map->GetTileAt(GetMapPos());
				_tile->SetMesh(s_camMeshList[CAM_ON_RIGHT]);
				_tile->SetType(Tile::TILE_OBJ_CAMERA_ON_1_4);
			}
			GenerateViewBox(_map);
			m_timer = S_M_CAMERA_MAX_TIMER;
		}
	}
}

void SecurityCamera::GenerateViewBox(TileMap* _map)
{
	if (m_state)
	{
		CreateViewBox(this, _map);
	}
}

void SecurityCamera::DestroyViewBox(TileMap* _map)
{
	ClearViewBox(this, _map);
}