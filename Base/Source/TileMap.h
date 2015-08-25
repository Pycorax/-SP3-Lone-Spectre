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
public:
	static const int S_LIGHT_RANGE = 5;
	static const int S_LIGHT_ATTENUATION = 2;

private:
	static const float S_LIGHT_ACCURACY;
	static const int S_PLAYER_SPAWN_MARKER = -1;

private:
	vector<vector<Tile*>*> m_map;			// Tile data for map
	float m_tileSize;						// Size of a tile in pixel

	// Map
	Vector2 m_numMapTile, m_numScreenTile;	// Number of tiles for map and screen
	Vector2 m_mapSize, m_screenSize;		// Size of map and screen in pixels
	Vector2 m_scrollOffset;					// Distance between start of map to start of screen in pixels
	
	// Map Markers
	Vector2 m_playerSpawnPos;				// Stores the position of the player's spawn as defined by the tilemap

public:
	TileMap(Vector2 numMapTile, Vector2 numScreenTile, float tileSize);
	~TileMap(void);

	void LoadTileMap(const string &filePath, const vector<Mesh*>& meshList);
	void UpdateLighting(vector<Vector2> shadowCasters);

	Tile* GetTileAt(int xPos, int yPos);		// Get tile info based on index location
	Tile* GetTileAt(Vector2 pos);				// Get tile info based on actual map pos
	bool CheckCollision(Vector2 pos);			// Check position with tile map for collision
	Tile::E_TILE_TYPE GetTileType(Vector2 pos);	// Get the tile type for a position
	void Clear(void);

	void SetScrollOffset(Vector2 scrollOffset);
	Vector2 GetScrollOffset(void);
	void AddToScrollOffset(Vector2 addScrollOffset);

	vector<vector<Tile*>*>& GetMap();

	void SetNumMapTile(Vector2 numMapTile);
	Vector2 GetNumMapTile();

	void SetNumScreenTile(Vector2 numScreenTile);
	Vector2 GetNumScreenTile();

	void SetMapSize(Vector2 mapSize);
	Vector2 GetMapSize();

	void SetScreenSize(Vector2 screenSize);
	Vector2 GetScreenSize();

	void SetTileSize(float tileSize);
	float GetTileSize();

	Vector2 GetPlayerSpawnPos(void);

private:
	bool loadFile(const string &filePath, const vector<Mesh*>& meshList);
	void calcLighting(int x, int y, vector<Vector2> shadowCasters);
	Vector2 posRoundingForLight(Vector2 pos, Vector2 dir);				// Adds the dir to the pos and rounds up/down to the int based on positive or negative dir
};

#endif