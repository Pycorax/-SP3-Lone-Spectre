#include "PlayerObject.h"

PlayerObject* PlayerObject::instances = NULL;

PlayerObject::PlayerObject() : CharObject()
{
}

PlayerObject* PlayerObject::GetInstance(int instance)
{
	if (instances == NULL)
	{
		instances = new PlayerObject[MAX_INSTANCES];
	}

	if (instance >= 0 && instance < MAX_INSTANCES)
	{
		return instances + instance;
	}
	else
	{
		return NULL;
	}
}

PlayerObject::~PlayerObject()
{
}

void PlayerObject::Update(MapLayer * map, double dt)
{
	CharObject::Update(map, dt);

	int tileSize = map->map->GetTileSize();
	constrainHero(map, 1.0f);
}

void PlayerObject::constrainHero(MapLayer* map, double dt)
{
	static const int SCROLL_SPEED = 5;
	const int EDGE_OFFSET = 5;

	int tileSize = map->map->GetTileSize();
	int leftBorder = tileSize * EDGE_OFFSET;
	int rightBorder = map->map->GetScreenWidth() - tileSize * 2 * EDGE_OFFSET;
	int topBorder = map->map->GetScreenHeight() - tileSize;
	int bottomBorder = tileSize;

	if (map->mapOffset.x < Math::EPSILON)
	{
		leftBorder = 0;
	}

	if (map->mapOffset.x >= map->map->GetScreenWidth() || map->map->GetScreenWidth() == map->map->GetNumOfTiles_MapWidth() * tileSize)
	{
		rightBorder = map->map->GetScreenWidth() - tileSize;
	}

	if (m_transforms.Translation.x < leftBorder)
	{
		m_transforms.Translation.x = leftBorder;

		// For Scrolling
		if (m_type == SIDE_SCROLL_PLAYER)
		{
			map->mapOffset.x = map->mapOffset.x - static_cast<int>(SCROLL_SPEED * dt);
			// -- For Special Cases at the edge of the map, do not scroll and leave the map
			if (map->mapOffset.x < map->map->GetTileSize())
			{
				map->mapOffset.x = 0;
			}
		}
	}
	else if (m_transforms.Translation.x > rightBorder)
	{
		m_transforms.Translation.x = rightBorder;

		// For Scrolling
		if (m_type == SIDE_SCROLL_PLAYER)
		{
			map->mapOffset.x = map->mapOffset.x + static_cast<int>(SCROLL_SPEED * dt);
			// -- For Special Cases at the edge of the map, do not scroll and leave the map
			if (map->mapOffset.x > map->map->GetScreenWidth())
			{
				map->mapOffset.x = map->map->GetScreenWidth();
			}
		}
	}

	if (m_transforms.Translation.y > topBorder)
	{
		m_transforms.Translation.y = topBorder;

		// For Scrolling
		if (m_type == SIDE_SCROLL_PLAYER)
		{
			map->mapOffset.y = map->mapOffset.y - static_cast<int>(SCROLL_SPEED * dt);
			// -- For Special Cases at the edge of the map, do not scroll and leave the map
			if (map->mapOffset.y < map->map->GetTileSize())
			{
				map->mapOffset.y = 0;
			}
		}
	}
	else if (m_transforms.Translation.y < bottomBorder)
	{
		m_transforms.Translation.y = bottomBorder;

		// For Scrolling
		if (m_type == SIDE_SCROLL_PLAYER)
		{
			map->mapOffset.y = map->mapOffset.y + static_cast<int>(SCROLL_SPEED * dt);
			// -- For Special Cases at the edge of the map, do not scroll and leave the map
			if (map->mapOffset.y > map->map->GetScreenHeight())
			{
				map->mapOffset.y = map->map->GetScreenHeight();
			}
		}
	}
}