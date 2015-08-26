#include "MapEntity.h"


MapEntity::MapEntity(Vector2 mapPos) : m_mapPos(mapPos)
{
}

MapEntity::~MapEntity(void)
{
}

void MapEntity::SetMapPosition(Vector2 pos, Vector2 scrollOffset, float tileSize)
{
	m_mapPos = pos;
	m_mapTilePos = pos *(1 / tileSize);

	// Round down
	m_mapTilePos.x = floor(m_mapTilePos.x);
	m_mapTilePos.y = floor(m_mapTilePos.y);

	updateScreenPos(calcScreenPos(scrollOffset));
}

Vector2 MapEntity::GetMapPos()
{
	return m_mapPos;
}

Vector2 MapEntity::GetMapTilePos()
{
	return m_mapTilePos;
}

Vector2 MapEntity::calcScreenPos(Vector2 scrollOffset)
{
	return Vector2(m_mapPos - scrollOffset);
}