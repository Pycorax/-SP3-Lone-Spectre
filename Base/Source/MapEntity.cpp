#include "MapEntity.h"


MapEntity::MapEntity(Vector2 mapPos) : m_mapPos(mapPos)
{
	if (true)
	{

	}
}

MapEntity::~MapEntity(void)
{
}

void MapEntity::SetMapPosition(Vector2 pos, Vector2 scrollOffset)
{
	m_mapPos = pos;

	updateScreenPos(pos, scrollOffset);
}

Vector2 MapEntity::GetMapPos()
{
	return m_mapPos;
}

Vector2 MapEntity::calcScreenPos(Vector2 scrollOffset)
{
	return	Vector2(m_mapPos - scrollOffset);
}