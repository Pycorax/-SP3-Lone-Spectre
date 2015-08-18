#include "Tile.h"

const bool Tile::S_IS_TILE_SOLID[NUM_TILE] =
{
	false
};

Tile::Tile(E_TILE_TYPE type, Mesh* _mesh) : m_type(type), m__mesh(_mesh)
{
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