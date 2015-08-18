#include "MapEntity.h"


MapEntity::MapEntity(void)
{
}


MapEntity::~MapEntity(void)
{
}

void MapEntity::SetMapPosition(Vector2 pos)
{
	m_mapPos = pos;

	updateScreenPos();
}

Vector2 MapEntity::getScreenPos()
{
	// TODO: Calculate screen position and return it

	return Vector2();
}