#ifndef MAP_LAYER_H
#define MAP_LAYER_H

#include "Map.h"
#include "Vector2.h"

struct MapLayer
{
	// Tile Map for this Layer
	CMap* map;

	// Scrolling Variables
	Vector2 mapOffset;
	Vector2 tileOffset;
	Vector2 mapFineOffset;

	// Scrolling Speed
	// -- This controls the speed relative to the main map. If this is the main map, it should be defaulted to 1.0;
	float scrollModifier;

	// If it is the main map level
	bool isLevelMap;

	MapLayer(CMap* _map = NULL, float speedMod = 1.0f)
	{
		map = _map;
		scrollModifier = speedMod;
		isLevelMap = false;
	}

	void Update(MapLayer* levelMap = NULL)
	{
		/*
		* Side Scrolling Code
		*/
		// -- Foreground
		// ---- Scrolling X
		if (isLevelMap == false && levelMap != NULL)
		{
			mapOffset.x = levelMap->mapOffset.x * scrollModifier;
			mapOffset.y = levelMap->mapOffset.y * scrollModifier;
		}
		
		// Update the TileOffset
		tileOffset.x = static_cast<int>(mapOffset.x / map->GetTileSize());

		if (tileOffset.x + map->GetNumOfTiles_Width() > map->GetNumOfTiles_MapWidth())
		{
			tileOffset.x = map->GetNumOfTiles_MapWidth() - map->GetNumOfTiles_Width();
		}

		mapFineOffset.x = static_cast<int>(mapOffset.x) % map->GetTileSize();

		// ---- Scrolling Y
		/*tileOffset.y = static_cast<int>(mapOffset.y / map->GetTileSize());

		if (tileOffset.y + map->GetNumOfTiles_Height() > map->getNumOfTiles_Map_Height())
		{
			tileOffset.y = map->getNumOfTiles_Map_Height() + map->GetNumOfTiles_Height();
		}

		mapFineOffset.y = static_cast<int>(mapOffset.y) % map->GetTileSize();*/

	}
};

#endif