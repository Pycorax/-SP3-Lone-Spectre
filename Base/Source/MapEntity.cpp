#include "MapEntity.h"


MapEntity::MapEntity(Vector2 mapPos) : m_mapPos(mapPos)
{
}


MapEntity::~MapEntity(void)
{
}

void MapEntity::SetMapPosition(Vector2 pos, Vector2 scrollOffset)
{
	m_mapPos = pos;

	updateScreenPos(pos, scrollOffset);
}

Vector2 MapEntity::getScreenPos()
{
	// TODO: Calculate screen position and return it

	return Vector2();
}