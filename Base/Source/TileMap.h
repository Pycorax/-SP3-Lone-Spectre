#ifndef TILE_MAP_H
#define TILE_MAP_H

#include "Tile.h"
#include "Vector2.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using std::vector;
using std::string;
using std::getline;
using std::ifstream;
using std::istringstream;
using std::cout;
using std::endl;

class TileMap
{

private:
	vector<vector<Tile*>*> m_map;			// Tile data for map
	float m_tileSize;						// Size of a tile in pixel

	// Map
	Vector2 m_numMapTile, m_numScreenTile;	// Number of tiles for map and screen
	Vector2 m_mapSize, m_screenSize;		// Size of map and screen in pixels
	Vector2 m_scrollOffset;					// Distance between start of map to start of screen in pixels
	
public:
	TileMap(Vector2 numMapTile, Vector2 numScreenTile, float tileSize);
	~TileMap(void);

	void LoadTileMap(const string &filePath, const vector<Mesh*>& meshList);
	void UpdateLighting(void);

	Tile* GetTileAt(int xPos, int yPos);
	bool CheckCollision(Vector2 pos);			// Check position with tile map for collision
	Tile::E_TILE_TYPE GetTileType(Vector2 pos);	// Get the tile type for a position
	void Clear(void);

	void SetScrollOffset(Vector2 scrollOffset);
	Vector2 GetScrollOffset(void);
	void AddToScrollOffset(Vector2 addScrollOffset);

	vector<vector<Tile*>*>& GetMap();

	Vector2 GetNumMapTile();
	Vector2 GetNumScreenTile();
	Vector2 GetMapSize();
	Vector2 GetScreenSize();

	float GetTileSize();

private:
	bool loadFile(const string &filePath, const vector<Mesh*>& meshList);
	void calcLighting(int x, int y);
};

#endif