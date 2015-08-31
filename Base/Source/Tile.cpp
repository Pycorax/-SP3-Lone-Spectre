#include "Tile.h"

const bool Tile::S_IS_TILE_SOLID[NUM_TILE] =
{
	false,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	true,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	false,
	true,
	false,
	false,
	false,
	false
};

Tile::Tile(Vector2 mapPos, Vector3 scale, E_TILE_TYPE type, Mesh* _mesh)
	: MapEntity(mapPos)
	, m_type(type)
	, m_lightLevel(0)
{
	m__mesh = _mesh;
	SetScale(scale);
}


Tile::~Tile(void)
{
}

void Tile::SetType(E_TILE_TYPE type)
{
	this->m_type = type;
}

Tile::E_TILE_TYPE Tile::GetType()
{
	return m_type;
}

int Tile::GetLightLevel(void)
{
	return m_lightLevel;
}

void Tile::updateScreenPos(Vector2 pos)
{
	m_transforms.Translation = pos;
}

void Tile::AddViewer(Viewer * _viewer)
{
	m__viewerList.push_back(_viewer);
}

void Tile::RemoveViewer(Viewer* _viewer)
{
	for (std::vector<Viewer*>::iterator it = m__viewerList.begin(); it != m__viewerList.end(); ++it)
	{
		Viewer* _v = (Viewer*)*it;
		if (_v == _viewer) // Same viewer
		{
			m__viewerList.erase(it);
			break;
		}
	}
}

void Tile::ResetViewers(void)
{
	m__viewerList.clear();
}

bool Tile::IsViewed(void)
{
	return m__viewerList.size() > 0;
}

void Tile::NotifyViewer(Vector2 targetTilePos, float &alertLevel, double dt)
{
	for (vector<Viewer*>::iterator it = m__viewerList.begin(); it != m__viewerList.end(); ++it)
	{
		Viewer* _viewer = (Viewer*)*it;
		if (_viewer)
		{
			_viewer->SpottedTarget(targetTilePos, alertLevel, dt);
		}
	}
}

void Tile::AddLight(int lightValue)
{
	m_lightLevel = Math::Clamp(m_lightLevel + lightValue, 0, MAX_LIGHT_LEVEL);
}

void Tile::ResetLighting(void)
{
	m_lightLevel = 0;
}
