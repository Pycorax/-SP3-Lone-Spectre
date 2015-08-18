#ifndef TILE_H
#define TILE_H

#include "Mesh.h"
#include "GameObject2D.h"
#include "MapEntity.h"

class Tile : public GameObject2D, public MapEntity
{
public:
	enum E_TILE_TYPE
	{
		TILE_FLOOR = 0,	// No tile
		TILE_SHADOW,
		TILE_WALL,
		TILE_CAMERA,
		NUM_TILE,
	};

	static const bool S_IS_TILE_SOLID[NUM_TILE];

private:
	E_TILE_TYPE m_type;
	// TODO: Add a vector of viewer

public:
	Tile(Vector2 mapPos = Vector2(0,0), E_TILE_TYPE type = TILE_FLOOR, Mesh* _mesh = NULL);
	~Tile(void);

	E_TILE_TYPE GetType();

protected:
	virtual void updateScreenPos(Vector2 pos, Vector2 scrollOffset);
};

#endif