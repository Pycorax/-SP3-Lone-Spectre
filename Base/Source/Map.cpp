#include "Map.h"

#include <fstream>
#include <sstream>
#include <exception>

using std::ifstream;
using std::ostringstream;
using std::istringstream;

const int CMap::TARGET_MARKER[Elemental::NUM_ELEMENTS] = { 901, 902, 903, 904 };

CMap::CMap(void)
: theScreen_Height(0)
, theScreen_Width(0)
, theNumOfTiles_Height(0)
, theNumOfTiles_Width(0)
, theTileSize(0)
, theMap_Height(0)
, theMap_Width(0)
, theNumOfTiles_MapHeight(0)
, theNumOfTiles_MapWidth(0)
{
	theScreenMap.clear();
}

CMap::~CMap(void)
{
	theScreenMap.clear();
}

void CMap::Init(const int theScreen_Height, const int theScreen_Width, const int numOfTiles_MapHeight, const int numOfTilesMapWidth, const int mapHeight, const int mapWidth, int theTileSize)
{
	this->theScreen_Height		= theScreen_Height;
	this->theScreen_Width		= theScreen_Width;
	this->theNumOfTiles_Height	= theScreen_Height / theTileSize;
	this->theNumOfTiles_Width	= theScreen_Width / theTileSize;
	this->theTileSize			= theTileSize;
	this->theMap_Height			= mapHeight;
	this->theMap_Width			= mapWidth;

	theNumOfTiles_MapHeight = numOfTiles_MapHeight;
	theNumOfTiles_MapWidth = numOfTilesMapWidth;

	theScreenMap.resize(theNumOfTiles_MapHeight);
	for (int i = 0; i < theNumOfTiles_MapHeight; ++i)
	{
		theScreenMap[i].resize(theNumOfTiles_MapWidth);
	}
}

bool CMap::LoadMap(const string mapName)
{
	if (LoadFile(mapName) == true)
	{
		printf("Map (%s) has been successfully loaded!\n", mapName.c_str());
		return true;
	}

	return false;
}

bool CMap::LoadFile(const string mapName)
{
	int theLineCounter = 0;
	int theMaxNumOfColumns = 0;
	bool readFirstLine = false;

	ifstream file(mapName.c_str());
	if (file.is_open())
	{
		int i = 0, k = 0;
	
		// Empty the target storage
		m_targetPositions.clear();
		m_targetElements.clear();

		// Reset the spawn position
		m_spawnPosition = Vector2::ZERO_VECTOR;
		
		while (file.good())
		{
			string aLineOfText = "";
			getline(file, aLineOfText);

			if (theLineCounter >= theNumOfTiles_MapHeight)
				break;

			// If this line is not a comment line, then process it
			if (!(aLineOfText.find("//*") == NULL) && aLineOfText != "")
			{
				if (theLineCounter == 0 && !readFirstLine)
				{
					// This is the first line of the map data file
					string token;
					istringstream iss(aLineOfText);
					while (getline(iss, token, ','))
					{
						// Count the number of columns
						theMaxNumOfColumns = atoi(token.c_str());
					}
					if (theMaxNumOfColumns != theNumOfTiles_MapWidth)
						return false;
				}
				else
				{
					int theRowCounter = theLineCounter - 1;
					int theColumnCounter = 0;

					string token;
					istringstream iss(aLineOfText);
					while (getline(iss, token, ',') && (theColumnCounter<theNumOfTiles_MapWidth))
					{
						theScreenMap[theRowCounter][theColumnCounter] = std::stoi(token.c_str());

						if (theScreenMap[theRowCounter][theColumnCounter] == SPAWN_MARKER)
						{
							m_spawnPosition.Set(theColumnCounter * theTileSize, theScreen_Height - ((theRowCounter) * theTileSize));
							theScreenMap[theRowCounter][theColumnCounter] = 0;
						}
						else
						{
							for (size_t i = 0; i < Elemental::NUM_ELEMENTS; ++i)
							{
								if (theScreenMap[theRowCounter][theColumnCounter] == TARGET_MARKER[i])
								{
									m_targetPositions.push_back(Vector2(theColumnCounter * theTileSize, theScreen_Height - ((theRowCounter)* theTileSize)));
									m_targetElements.push_back(static_cast<Elemental::ELEMENT_TYPE>(i));
									theScreenMap[theRowCounter][theColumnCounter] = 1;
								}
							}
						}

						theColumnCounter++;
					}
				}
			}

			theLineCounter++;
		}
	}
	return true;
}

int CMap::GetNumOfTiles_Height(void)
{
	return theNumOfTiles_Height;
}

int CMap::GetNumOfTiles_Width(void)
{
	return theNumOfTiles_Width;
}

int CMap::GetTileSize(void)
{
	return theTileSize;
}

int CMap::GetNumOfTiles_MapHeight(void)
{
	return theNumOfTiles_MapHeight;
}

int CMap::GetNumOfTiles_MapWidth(void)
{
	return theNumOfTiles_MapWidth;
}

int CMap::GetScreenHeight()
{
	return theScreen_Height;
}

int CMap::GetScreenWidth()
{
	return theScreen_Width;
}

Vector2 CMap::GetSpawnPos(void) const
{
	return m_spawnPosition;
}

vector<Vector2> CMap::GetTargetPositions(void) const
{
	return m_targetPositions;
}

vector<Elemental::ELEMENT_TYPE> CMap::GetTargetElements(void) const
{
	return m_targetElements;
}

int CMap::GetMapAt(int x, int y) const
{
	if (y < 0 || y >= theScreenMap.size())
	{
		return 1;
	}
	if (x < 0 || x >= theScreenMap[y].size())
	{
		return 1;
	}

	return theScreenMap[y][x];
}

bool CMap::TileAtPosIsWall(int x, int y) const
{
	int tile = GetMapAt(x, y);

	return (tile > 0 && tile < 100);
}
