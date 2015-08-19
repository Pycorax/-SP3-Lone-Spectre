#include "Level.h"

Level::Level(void)
{
}

Level::~Level(void)
{
}

void Level::InitMap(Vector2 numMapTile, Vector2 numScreenTile, float tileSize, string filePath, vector<Mesh*> &meshList)
{
	m__map = new TileMap(numMapTile, numScreenTile, tileSize);
	m__map->LoadTileMap(filePath, meshList);
}

void Level::Update(double dt)
{

}

void Level::Clear(void)
{
	if (m__map)
	{
		delete m__map;
		m__map = NULL;
	}
}

TileMap* Level::GetTileMap()
{
	return m__map;
}