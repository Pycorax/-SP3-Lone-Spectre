#include "Tile.h"

const bool Tile::S_IS_TILE_SOLID[NUM_TILE] =
{
	false
};

Tile::Tile(Vector2 mapPos, Vector3 scale, E_TILE_TYPE type, Mesh* _mesh) : m_type(type), MapEntity(mapPos)
{
	m__mesh = _mesh;
	SetScale(scale);
}


Tile::~Tile(void)
{
	if (m__mesh)
	{
		delete m__mesh;
		m__mesh = NULL;
	}
}

Tile::E_TILE_TYPE Tile::GetType()
{
	return m_type;
}

void Tile::updateScreenPos(Vector2 pos, Vector2 scrollOffset)
{
	m_transforms.Translation = calcScreenPos(scrollOffset);
}