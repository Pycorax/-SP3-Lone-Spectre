#ifndef TILE_H
#define TILE_H

#include "Mesh.h"

class Tile
{
public:
	enum E_TILE_TYPE
	{
		TILE_FLOOR = 0,	// No tile
		NUM_TILE,
	};

	static const bool S_IS_TILE_SOLID[NUM_TILE];

	Tile(E_TILE_TYPE type = TILE_FLOOR, Mesh* _mesh = NULL);
	~Tile(void);

	E_TILE_TYPE GetType();

private:
	E_TILE_TYPE m_type;
	Mesh* m__mesh;
	// TODO: Add a vector of viewer
};

#endif