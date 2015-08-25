#include "TileMap.h"

const float TileMap::S_LIGHT_ACCURACY = 0.4f;

TileMap::TileMap(Vector2 numMapTile, Vector2 numScreenTile, float tileSize) : m_numMapTile(numMapTile), m_numScreenTile(numScreenTile), m_tileSize(tileSize), m_mapSize(numMapTile * m_tileSize), m_screenSize(numScreenTile * m_tileSize), m_scrollOffset(0,0), m_map(NULL)
{

}

TileMap::~TileMap(void)
{
}

void TileMap::LoadTileMap(const string &filePath, const vector<Mesh*>& meshList)
{
	if (loadFile(filePath, meshList))	// Loaded map
	{
		cout << filePath << " has been loaded successfully!" << endl;
	}
	else								// Error while loading map
	{
		cout << "Unable to load " << filePath << endl;
	}
}

void TileMap::UpdateLighting(vector<Vector2> shadowCasters)
{
	int countX = 0;
	int countY = 0;

	// Reset the lighting for this frame
	for (vector<vector<Tile*>*>::iterator row = m_map.begin(); row != m_map.end(); ++row, ++countY)
	{
		for (vector<Tile*>::iterator col = (*row)->begin(); col != (*row)->end(); ++col, ++countX)
		{
			// Make it bright
			Tile* _tile = GetTileAt(countX, countY);

			if (_tile != NULL)
			{
				_tile->ResetLighting();
			}
		}

		countX = 0;
	}

	// Reset count for this next loop
	countX = countY = 0;
	// Calculate the lighting this frame
	for (vector<vector<Tile*>*>::iterator row = m_map.begin(); row != m_map.end(); ++row, ++countY)
	{
		for (vector<Tile*>::iterator col = (*row)->begin(); col != (*row)->end(); ++col, ++countX)
		{
			// If this is a light...
			if ((*col)->GetType() == Tile::TILE_LIGHT)
			{
				calcLighting(countX, countY, shadowCasters);
			}
		}

		// Reset the counter to the start as it is the end of this row
		countX = 0;
	}
}

bool TileMap::loadFile(const string &filePath, const vector<Mesh*>& meshList)
{
	const string tileTypeName[Tile::NUM_TILE] = 
	{
		"TILE_FLOOR",
		"TILE_INVISIBLE_WALL",
		"TILE_WALL",
		"TILE_OBJ_CABINET",
		"TILE_OBJ_CHAIR_LEFT",
		"TILE_OBJ_CHAIR_RIGHT",
		"TILE_OBJ_COMPUTER_1_1", // Row 1 col 1
		"TILE_OBJ_COMPUTER_2_1", // Row 2 col 1
		"TILE_OBJ_CUPBOARD",
		"TILE_OBJ_SINK",
		"TILE_OBJ_TABLE_1_1", // Row 1 col 1
		"TILE_OBJ_TABLE_1_2", // Row 1 col 2
		"TILE_OBJ_TABLE_2_1", // Row 2 col 1
		"TILE_OBJ_TABLE_2_2", // Row 2 col 2
		"TILE_OBJ_CAMERA_ON_1_1", // Row 1 col 1
		"TILE_OBJ_CAMERA_ON_1_2", // Row 1 col 2
		"TILE_OBJ_CAMERA_ON_1_3", // Row 1 col 3
		"TILE_OBJ_CAMERA_ON_1_4", // Row 1 col 4
		"TILE_OBJ_CAMERA_OFF_1_1", // Row 1 col 1
		"TILE_OBJ_CAMERA_OFF_1_2", // Row 1 col 2
		"TILE_OBJ_CAMERA_OFF_1_3", // Row 1 col 3
		"TILE_OBJ_CAMERA_OFF_1_4", // Row 1 col 4
		"TILE_LIGHT"
	};
	Mesh* _tileMeshList[Tile::NUM_TILE];
	for (int name = 0; name < Tile::NUM_TILE; ++name)
	{
		_tileMeshList[name] = NULL;
	}
	for (int mesh = 0; mesh < meshList.size(); ++mesh)		// Loop through meshList to find mesh for tile and sort based on E_TILE_TYPE
	{
		for (int name = 0; name < Tile::NUM_TILE; ++name)	// Loop through the names of tile type to find the matching name with mesh list
		{
			if (meshList[mesh]->name == tileTypeName[name])
			{
				_tileMeshList[name] = meshList[mesh]; // Assign the proper tile mesh to a temp tile mesh list
				break; // Breaks the inner loop
			}
		}
	}
	int rowCounter = 0, colCounter = 0;
	string line = "";	// Line of text from file
	ifstream file(filePath);

	if (file.is_open())		// Only read if the file was able to be opened
	{
		while (!file.eof()) // While file has not ended
		{
			if (rowCounter >= m_numMapTile.y) // File has more rows
			{
				break; // Ignore the additional rows and return
			}

			getline(file, line); // Fetch a line of text from file

			if (line.find("//") == -1) // Not commented line
			{
				colCounter = 0; // Reset column counter for each row
				string token = "";	// Individual token from line
				istringstream iss(line);

				// Generate a new row
				if (rowCounter == 0) // First row
				{
					m_map.push_back(new vector<Tile*>);	// New row
				}
				else
				{
					m_map.insert(m_map.begin(), new vector<Tile*>); // Insert new row at the front
				}

				while (getline(iss, token, ',') && colCounter < m_numMapTile.x) // Load each column in a row
				{
					int tile = std::stoi(token.c_str()); // Convert a tile from string to int

					switch (tile)
					{
					case S_PLAYER_SPAWN_MARKER:
						{
							m_playerSpawnPos.Set(colCounter * m_tileSize, m_mapSize.y - ((rowCounter + 1) * m_tileSize));
							tile = Tile::TILE_FLOOR;
						}
						break;
					}

					m_map.front()->push_back(new Tile(Vector2(colCounter * m_tileSize, m_mapSize.y - ((rowCounter + 1) * m_tileSize)), Vector3(m_tileSize, m_tileSize), static_cast<Tile::E_TILE_TYPE>(tile), _tileMeshList[tile])); // Add tiles into row
					
					
					++colCounter;
				}

				for (; colCounter < m_numMapTile.x; ++colCounter) // Add empty tiles to fill up missing columns in file
				{
					int tile = 0;
					//m_map.front()->push_back(new Tile(Vector2(colCounter * m_tileSize, m_mapSize.y - ((rowCounter + 1) * m_tileSize)), Vector3(m_tileSize, m_tileSize)));
					m_map.front()->push_back(new Tile(Vector2(colCounter * m_tileSize, m_mapSize.y - ((rowCounter + 1) * m_tileSize)), Vector3(m_tileSize, m_tileSize), static_cast<Tile::E_TILE_TYPE>(tile), _tileMeshList[tile])); // Add tiles into row
				}
				++rowCounter;
			}
			else if (line == "") // Empty line
			{
				// Generate a new row
				if (rowCounter == 0) // First row
				{
					m_map.push_back(new vector<Tile*>);	// New row
				}
				else
				{
					m_map.insert(m_map.begin(), new vector<Tile*>); // Insert new row at the front
				}

				for (; colCounter < m_numMapTile.x; ++colCounter) // Loop number of tiles for width
				{
					int tile = 0;
					//m_map.front()->push_back(new Tile(Vector2(colCounter * m_tileSize, m_mapSize.y - ((rowCounter + 1) * m_tileSize)), Vector3(m_tileSize, m_tileSize)));
					m_map.front()->push_back(new Tile(Vector2(colCounter * m_tileSize, m_mapSize.y - ((rowCounter + 1) * m_tileSize)), Vector3(m_tileSize, m_tileSize), static_cast<Tile::E_TILE_TYPE>(tile), _tileMeshList[tile])); // Add tiles into row
				}
				++rowCounter;
			}
		}
		/*while (rowCounter < m_numMapTile.y) // Fill in the remaining empty rows
		{
		m_map.push_back(new vector<Tile*>);	// New row
		for (int col = 0; col < m_numMapTile.x; ++col) // Loops number of tiles for width
		{
		int tile = 0;
		//m_map.front()->push_back(new Tile(Vector2(colCounter * m_tileSize, m_mapSize.y - ((rowCounter + 1) * m_tileSize)), Vector3(m_tileSize, m_tileSize)));
		m_map.back()->push_back(new Tile(Vector2(colCounter * m_tileSize, rowCounter * m_tileSize), Vector3(m_tileSize, m_tileSize), static_cast<Tile::E_TILE_TYPE>(tile), _tileMeshList[tile])); // Add tiles into row
		++colCounter;
		}
		++rowCounter;
		}*/
		file.close(); // Close file
	}
	
	return true;
}

void TileMap::calcLighting(const int LIGHT_POS_X, const int LIGHT_POS_Y, vector<Vector2> shadowCasters)
{
	/*
	 * Calculating the brightness of each tile
	 */
	// Origin is at top left
	for (int yTile = LIGHT_POS_Y - S_LIGHT_RANGE; yTile <= LIGHT_POS_Y + S_LIGHT_RANGE; ++yTile)
	{
		for (int xTile = LIGHT_POS_X - S_LIGHT_RANGE; xTile <= LIGHT_POS_X + S_LIGHT_RANGE; ++xTile)
		{
		
			// If this is outside the range of the map
			if (xTile < 0 || yTile < 0 || yTile >= m_map.size() || xTile >= m_map.at(yTile)->size())
			{
				// Don't read or write to prevent out of range errors
				continue;
			}

			// If this is the light source we are calculating for
			if (xTile == LIGHT_POS_X && yTile == LIGHT_POS_Y)
			{
				// Make it bright
				Tile* tile = GetTileAt(xTile, yTile);
				tile->AddLight(Tile::MAX_LIGHT_LEVEL);
			}
			else
			{
				// Calculate Distance Away
				const Vector2 LIGHT_POS(LIGHT_POS_X, LIGHT_POS_Y);
				const Vector2 TILE_POS(xTile, yTile);
				Vector2 deltaPos = LIGHT_POS - TILE_POS;			// Vector2 from the light to this tile
				Vector2 dir = deltaPos.Normalized();													// Unit Vector2 direction from the light to this tile

				// Check if it is blocked
				bool blocked = false;
				Vector2 searchStartPos = TILE_POS + Vector2(0.5, 0.5) /* To move into the center if the tile (need not be tied to tile size as all the calculations are done to relative sizing */;
				Vector2 midTilePos = searchStartPos;

				// Check through all the tiles on the way to our tile
				//for (vector<Vector2>::iterator tilePos = tilePositions.begin(); tilePos != tilePositions.end(); ++tilePos)
				while(true)
				{
					// For optimization: Prevent rechecks
					static Vector2 prevMidTilePosInt;

					// Move to the next block, towards our tile
					midTilePos += S_LIGHT_ACCURACY * dir;

					// For decimal round up or down according to the dir being positive or negative
					Vector2 midTilePosInt;
					midTilePosInt.x = static_cast<int>(midTilePos.x);
					midTilePosInt.y = static_cast<int>(midTilePos.y);

					// Prevent rechecking the same spot over and over again due to the high accuracy
					if (midTilePosInt != prevMidTilePosInt)
					{
						// If we reached the source, staph it
						if (midTilePosInt == LIGHT_POS)
						{
							break;
						}

						// Get a pointer to the tile to check
						Tile* midTile = GetTileAt(midTilePosInt.x, midTilePosInt.y);

						// Check if the mid tile is out of the map or is solid
						
						bool isWall = false;

						if (midTile == NULL)
						{
							isWall = true;
						}
						else
						{
							if (Tile::S_IS_TILE_SOLID[midTile->GetType()])
							{
								isWall = true;
							}
							else
							{
								for (vector<Vector2>::iterator shadowCastIter = shadowCasters.begin(); shadowCastIter != shadowCasters.end(); ++shadowCastIter)
								{
									Vector2 pos(floor((*shadowCastIter).x), floor((*shadowCastIter).y));

									if (midTilePosInt.x == pos.x && midTilePosInt.y == pos.y)
									{
										isWall = true;
										break;
									}
								}
							}
						}

						if (isWall)
						{
							// No Lighting. Don't add anything. Just break.
							blocked = true;
							break;
						}

						prevMidTilePosInt = midTilePosInt;
					}
				}

				// If it is not blocked
				if (!blocked)
				{
					// Get the tile to light up
					Tile* _tile = GetTileAt(xTile, yTile);

					if (_tile != NULL && _tile->GetType() != Tile::TILE_INVISIBLE_WALL)
					{
						// Calculate the light level
						int furthestDist = deltaPos.Length();

						int lightLevel = Tile::MAX_LIGHT_LEVEL - abs(furthestDist) * S_LIGHT_ATTENUATION;

						// Assign the light level
						_tile->AddLight(lightLevel);
					}
				}
				
				// else		// Don't add any light at all
			}
		}
	}
}

Vector2 TileMap::posRoundingForLight(Vector2 pos, Vector2 dir)
{
	// If position is not a whole number
	if (pos.x != static_cast<int>(pos.x))
	{
		if (dir.x > Math::EPSILON)
		{
			pos.x = static_cast<int>(pos.x + 0.5);
		}
		else if (dir.x < -Math::EPSILON)
		{
			pos.x = static_cast<int>(pos.x - 0.5);
		}
	}

	// If position is not a whole number
	if (pos.y != static_cast<int>(pos.y))
	{
		if (dir.x > Math::EPSILON)
		{
			pos.y = static_cast<int>(pos.y + 0.5);
		}
		else if (dir.x < -Math::EPSILON)
		{
			pos.y = static_cast<int>(pos.y - 0.5);
		}
	}

	return pos;
}

Tile* TileMap::GetTileAt(int xPos, int yPos)
{
	if (xPos < 0 || yPos < 0 || yPos >= m_map.size() || xPos >= m_map.at(yPos)->size())
	{
		// Don't read or write to prevent out of range errors
		return NULL;
	}
	else
	{
		return (*m_map[yPos])[xPos];
	}
}

Tile* TileMap::GetTileAt(Vector2 pos)
{
	Vector2 tilePos(floor(pos.x / m_tileSize), floor(pos.y / m_tileSize)); // Round down decimal number to get tile position
	if (tilePos.x < 0 || tilePos.x >= m_numMapTile.x || tilePos.y < 0 || tilePos.y >= m_numMapTile.y)
	{
		return NULL;
	}
	return (*m_map[tilePos.y])[tilePos.x];
}

bool TileMap::CheckCollision(Vector2 pos)
{
	//Vector2 tilePos = pos * (1.f / m_tileSize);
	Vector2 tilePos(floor(pos.x / m_tileSize), floor(pos.y / m_tileSize)); // Round down decimal number to get tile position
	if (tilePos.x < 0 || tilePos.x >= m_numMapTile.x || tilePos.y < 0 || tilePos.y >= m_numMapTile.y)
	{
		return true;
	}
	Tile::E_TILE_TYPE tempType = (*m_map[tilePos.y])[tilePos.x]->GetType();
	if (Tile::S_IS_TILE_SOLID[tempType])
	{
		return true;
	}
	return false;
}

Tile::E_TILE_TYPE TileMap::GetTileType(Vector2 pos)
{
	Vector2 tilePos = pos * (1.f / m_tileSize);
	return (*m_map[tilePos.y])[tilePos.x]->GetType();
}

void TileMap::Clear(void)
{
	while (m_map.size() > 0)
	{
		vector<Tile*>* _backRow = m_map.back();
		if (_backRow)
		{
			while (_backRow->size() > 0)
			{
				Tile* _backTile = _backRow->back();
				if (_backTile)
				{
					delete _backTile;
					_backRow->pop_back();
				}
			}
			delete _backRow;
			m_map.pop_back();
		}
	}
}

void TileMap::SetScrollOffset(Vector2 scrollOffset)
{
	this->m_scrollOffset = scrollOffset;
}

Vector2 TileMap::GetScrollOffset(void)
{
	return m_scrollOffset;
}

void TileMap::AddToScrollOffset(Vector2 addScrollOffset)
{
	m_scrollOffset += addScrollOffset;
}

vector<vector<Tile*>*>& TileMap::GetMap()
{
	return m_map;
}

void TileMap::SetNumMapTile(Vector2 numMapTile)
{
	this->m_numMapTile = numMapTile;
}

Vector2 TileMap::GetNumMapTile()
{
	return m_numMapTile;
}

void TileMap::SetNumScreenTile(Vector2 numScreenTile)
{
	this->m_numScreenTile = numScreenTile;
}

Vector2 TileMap::GetNumScreenTile()
{
	return m_numScreenTile;
}

void TileMap::SetMapSize(Vector2 mapSize)
{
	this->m_mapSize = mapSize;
}

Vector2 TileMap::GetMapSize()
{
	return m_mapSize;
}

void TileMap::SetScreenSize(Vector2 screenSize)
{
	this->m_screenSize = screenSize;
}

Vector2 TileMap::GetScreenSize()
{
	return m_screenSize;
}

void TileMap::SetTileSize(float tileSize)
{
	this->m_tileSize = tileSize;
}

float TileMap::GetTileSize()
{
	return m_tileSize;
}

Vector2 TileMap::GetPlayerSpawnPos(void)
{
	return m_playerSpawnPos;
}
