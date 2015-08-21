#include "TileMap.h"


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

Vector2 TileMap::GetNumMapTile()
{
	return m_numMapTile;
}

Vector2 TileMap::GetNumScreenTile()
{
	return m_numScreenTile;
}

Vector2 TileMap::GetMapSize()
{
	return m_mapSize;
}

Vector2 TileMap::GetScreenSize()
{
	return m_screenSize;
}

float TileMap::GetTileSize()
{
	return m_tileSize;
}