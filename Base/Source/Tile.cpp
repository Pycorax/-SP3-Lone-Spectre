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
	true,
	true,
	true,
	true
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

void Tile::updateScreenPos(Vector2 pos)
{
	m_transforms.Translation = pos;
}

void Tile::AddViewer(Vector2 LookDir, int ViewDist_NumOfTiles)
{
	Viewer* _view;
	float distViewY = 1;
	float distViewX = 1;
	// set the maximum of the horizontal view
	if(LookDir.x != 0)
	{
		distViewY += (ViewDist_NumOfTiles * 2); 
		distViewX = ViewDist_NumOfTiles;
	}
	else if(LookDir.y != 0)
	{
		distViewX += (ViewDist_NumOfTiles * 2);
		distViewY = ViewDist_NumOfTiles;
		/*		**NOTE: if facing up [m_lookDir == (0,1) ]**
		*
		*		| | | | |  < distViewX
		*		  | | |
		*			|
		*			^
		*		distViewY
		*/
	}
	//set the direction and view distance of viewer's View
	_view->Init(distViewX ,distViewY);
	//add into list
	m__ViewerList.push_back(_view);
}