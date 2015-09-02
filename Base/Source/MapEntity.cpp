#include "MapEntity.h"


MapEntity::MapEntity(Vector2 mapPos) : m_mapPos(mapPos)
{
}

MapEntity::~MapEntity(void)
{
}

void MapEntity::SetMapPosition(Vector2 pos, Vector2 scrollOffset, float tileSize)
{
	static const float S_DEGREE_OF_ACCURACY = 0.001f; // Degree of accuracy
	
	float mapTileX = pos.x / tileSize;
	float mapTileY = pos.y / tileSize;
	m_mapPos = pos;
	m_mapTilePos = Vector2(floor(mapTileX), floor(mapTileY));
	float ceilTileX = ceil(mapTileX);
	float ceilTileY = ceil(mapTileY);
	if (mapTileX + S_DEGREE_OF_ACCURACY > ceilTileX)
	{
		m_mapTilePos.x = ceilTileX;
		m_mapPos.x = ceilTileX * tileSize + S_DEGREE_OF_ACCURACY;
	}
	if (mapTileY + S_DEGREE_OF_ACCURACY > ceilTileY)
	{
		m_mapTilePos.y = ceilTileY;
		m_mapPos.y = ceilTileY * tileSize + S_DEGREE_OF_ACCURACY;
	}

	updateScreenPos(calcScreenPos(scrollOffset));
}

void MapEntity::SetMapTilePosition(Vector2 tilePos, Vector2 scrollOffset, float tileSize)
{
	m_mapTilePos = tilePos;
	m_mapPos = Vector2(tilePos.x * tileSize, tilePos.y * tileSize);

	updateScreenPos(calcScreenPos(scrollOffset));
}

Vector2 MapEntity::GetMapPos() const
{
	return m_mapPos;
}

Vector2 MapEntity::GetMapTilePos() const
{
	return m_mapTilePos;
}

Vector2 MapEntity::calcScreenPos(Vector2 scrollOffset)
{
	return Vector2(m_mapPos - scrollOffset);
}