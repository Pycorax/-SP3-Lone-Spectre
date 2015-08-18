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
	};
	Mesh* _tileMeshList[Tile::NUM_TILE];
	for (int mesh = 0; mesh < meshList.size(); ++mesh)		// Loop through meshList to find mesh for tile and sort based on E_TILE_TYPE
	{
		_tileMeshList[mesh] = NULL;
		for (int name = 0; name < Tile::NUM_TILE; ++name)	// Loop through the names of tile type to find the matching name with mesh list
		{
			if (meshList[mesh]->name == tileTypeName[name])
			{
				_tileMeshList[Tile::TILE_FLOOR] = meshList[mesh]; // Assign the proper tile mesh to a temp tile mesh list
				break; // Breaks the inner loop
			}
		}
	}
	int rowCounter = 0, colCounter = 0;
	string line = "";	// Line of text from file
	ifstream file(filePath);

	while (!file.eof()) // While file has not ended
	{
		if (rowCounter >= m_numMapTile.y) // File has more rows
		{
			return true; // Ignore the additional rows and return
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
				m_map.front()->push_back(new Tile(Vector2(colCounter * m_tileSize, rowCounter * m_tileSize), static_cast<Tile::E_TILE_TYPE>(tile), _tileMeshList[tile])); // Add tiles into row
				++colCounter;
			}
			for (; colCounter < m_numMapTile.x; ++colCounter) // Add empty tiles to fill up missing columns in file
			{
				m_map.front()->push_back(new Tile(Vector2(colCounter * m_tileSize, rowCounter * m_tileSize)));
			}
			++rowCounter;
		}
		else if (line == "") // Empty line
		{
			for (int col = 0; col < m_numMapTile.x; ++col) // Loop number of tiles for width
			{
				m_map.front()->push_back(new Tile(Vector2(colCounter * m_tileSize, rowCounter * m_tileSize))); // Create empty tiles
			}
			++rowCounter;
		}
	}
	while (colCounter < m_numMapTile.x) // Fill in the remaining empty rows
	{
		m_map.insert(m_map.begin(), new vector<Tile*>); // Insert new row at the front
		for (int col = 0; col < m_numMapTile.x; ++col) // Loops number of tiles for width
		{
			m_map.front()->push_back(new Tile(Vector2(colCounter * m_tileSize, rowCounter * m_tileSize))); // Create empty tiles
		}
	}
	file.close(); // Close file
	return true;
}

bool TileMap::CheckCollision(Vector2 pos)
{
	Vector2 tilePos = pos * (1.f / m_tileSize);
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