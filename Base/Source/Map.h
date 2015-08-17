#ifndef MAP_H
#define MAP_H

// STL Includes
#include <vector>
#include <string>

// Other Includes
#include "Vector2.h"

using std::string;
using std::vector;

#include "Elemental.h"

class CMap
{
private:
	Vector2 m_spawnPosition;
	vector<Vector2> m_targetPositions;
	vector<Elemental::ELEMENT_TYPE> m_targetElements;

	vector<vector<int>> theScreenMap;

public:
	static const int SPAWN_MARKER = 900;
	static const int TARGET_MARKER[Elemental::NUM_ELEMENTS];

	CMap(void);
	~CMap(void);

	void Init(const int theScreen_Height, const int theScreen_Width, const int numOfTiles_MapHeight, const int numOfTilesMapWidth, const int mapHeight, const int mapWidth, const int theTileSize=25);
	bool LoadMap(const string mapName);
	int GetNumOfTiles_Height(void);		// Get the number of tiles for the height of the screen
	int GetNumOfTiles_Width(void);		// Get the number of tiles for the width of the screen
	int GetTileSize(void);

	int GetNumOfTiles_MapHeight(void);
	int GetNumOfTiles_MapWidth(void);

	int GetScreenHeight(void);
	int GetScreenWidth(void);

	Vector2 GetSpawnPos(void) const;
	vector<Vector2> GetTargetPositions(void) const;
	vector<Elemental::ELEMENT_TYPE> GetTargetElements(void) const;
	int GetMapAt(int x, int y) const;
	bool TileAtPosIsWall(int x, int y)const;
	

private:
	int theScreen_Height;
	int theScreen_Width;
	int theNumOfTiles_Height;		// Number of tiles in the screen's height
	int theNumOfTiles_Width;		// Number of tiles in the screen's width
	int theTileSize;

	int theMap_Height;					// The map's height
	int theMap_Width;					// The map's width
	int theNumOfTiles_MapHeight;			// Number of tiles in the map's height
	int theNumOfTiles_MapWidth;			// Number of tiles in the map's width

	bool LoadFile(const string mapName);
};

#endif