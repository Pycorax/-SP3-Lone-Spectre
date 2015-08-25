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
	m_mapTilePos = Vector2(floor(pos.x / tileSize), floor(pos.y / tileSize));

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