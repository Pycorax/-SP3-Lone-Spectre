#ifndef TILE_H
#define TILE_H

#include "Mesh.h"
#include "GameObject2D.h"
#include "MapEntity.h"
#include "Viewer.h"

//STL
#include <vector>
using std::vector;

class Tile : public GameObject2D, public MapEntity
{
public:
	enum E_TILE_TYPE
	{
		TILE_FLOOR = 0,	// Floor tile
		TILE_SHADOW,
		TILE_WALL,
		TILE_CAMERA,
		NUM_TILE,
	};

	static const bool S_IS_TILE_SOLID[NUM_TILE];

private:
	E_TILE_TYPE m_type;
	vector<Viewer *> m__ViewerList;

public:
	Tile(Vector2 mapPos = Vector2(0,0), Vector3 scale = Vector3(1,1,1), E_TILE_TYPE type = TILE_FLOOR, Mesh* _mesh = NULL);
	~Tile(void);

	void AddViewer(Vector2 LookDir, int ViewDist_NumOfTiles);

	E_TILE_TYPE GetType();
protected:
	virtual void updateScreenPos(Vector2 pos);
};

#endif