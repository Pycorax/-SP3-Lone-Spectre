#ifndef MVC_MADDON_TILESYS_H
#define MVC_MADDON_TILESYS_H

#include "Map.h"
#include "MapLayer.h"
#include "Vector2.h"
#include "PlayerObject.h"
#include "BulletObject.h"
// Goodies
#include "GoodieObject.h"
#include "GoodieObjectFactory.h"

class MVC_MAddon_TileSys
{
	public:
		// Tilemap
		enum TILE_TYPE
		{
			TILE_EMPTY = 0,
			// 1 - 99: Walls
			TILE_GROUND = 1,
			TILE_STRUCTURE = 3,
			TILE_DARKBLOCK = 4,
			TILE_INVISIWALL = 99,
			// 100 - 199: Triggers
			TILE_EXIT = 100,
			// 900 - 999: MARKERS
			TILE_SPAWN = CMap::SPAWN_MARKER,
			NUM_TILE
		};

		PlayerObject* m_hero;

	protected:
		GameObject* m_background;
		GameObject* m_heroGO;

		// Tilemap
		Mesh* m_tileMesh[NUM_TILE];

		// Hero
		static const float SCROLL_SPEED;

		// Bullets
		vector<BulletObject*> m_bulletList;

		// Handle to Main Map Layer
		MapLayer* m_map;
		// Decorative Background Map Layers
		vector<MapLayer*> m_mapLayers;

		// Goodies
		GoodieObjectFactory goodiesFactory;
		vector<GoodieObject*> goodieList;

		// Output to View
		vector<GameObject*> m_renderList2DFirst;		// A list of stuff to render before the tile map is rendered

	public:
		MVC_MAddon_TileSys(void);
		virtual ~MVC_MAddon_TileSys(void);

		// Tile Maps
		Mesh* GetTileMesh(TILE_TYPE tileType);

		// MapLayers
		vector<MapLayer*> GetMapLayers(void);

		// Render Lists
		const vector<GameObject*>& Get2DFirstRenderList(void) const;

	protected:
		virtual void Update(void);
		virtual void Exit(void);
		
		// Update Functions
		void heroUpdate(double dt);
		void sidescrollUpdate(void);
		virtual void bulletUpdate(double dt);

		// For Player Attack
		BulletObject* FetchBullet();

		Vector2 screenToTileSpace(Vector2 pos);
};

#endif