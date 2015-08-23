#include "AINodeGrid.h"

#include "TileMap.h"
#include "Vector2.h"

AINodeGrid::AINodeGrid()
{
}


AINodeGrid::~AINodeGrid()
{
}

void AINodeGrid::Init(TileMap * _tileMap)
{
	Vector2 mapSize = _tileMap->GetMapSize();

	// Build the Node Grid
	for (size_t row = 0; row < mapSize.y; ++row)
	{
		vector<AINode*>* newNodeRow = new vector<AINode*>;

		for (size_t col = 0; col < mapSize.x; ++col)
		{
			AINode* newNode = new AINode;

			// Mark this node as a wall if it is a wall in the tile map
			Tile* tile = _tileMap->GetTileAt(col, row);
			if (Tile::S_IS_TILE_SOLID[tile->GetType()])
			{
				newNode->m_isWall = true;
			}

			// Add this node to the row
			newNodeRow->push_back(newNode);
		}

		// Add this row to the grid
		m_nodeGrid.push_back(newNodeRow);
	}
}

void AINodeGrid::Exit(void)
{
	while (m_nodeGrid.size() > 0)
	{
		if (m_nodeGrid.back() != NULL)
		{
			// Delete all the nodes in this row
			while (m_nodeGrid.back()->size() > 0)
			{
				// Delete this node
				if (m_nodeGrid.back()->back() != NULL)
				{
					delete m_nodeGrid.back()->back();
					m_nodeGrid.back()->pop_back();
				}
			}
		}
		
		// Delete this row
		delete m_nodeGrid.back();
		m_nodeGrid.pop_back();
	}
}

AINode * AINodeGrid::GetNodeAt(int xPos, int yPos)
{
	if (
		yPos >= 0 && yPos < m_nodeGrid.size()
		&&
		xPos >= 0 && xPos < m_nodeGrid.at(yPos)->size()
		)
	{
		// Get the neighbour
		return m_nodeGrid.at(yPos)->at(xPos);
	}
	else
	{
		return NULL;
	}
}

vector<AINode*> AINodeGrid::GetNeighboursOf(AINode * node)
{
	vector<AINode*> neighbours;
	AINode* neighbour = NULL;

	for (int y = -1; y <= 1; ++y)
	{
		for (int x = -1; x <= 1; ++x)
		{
			if (y == 0)		// Add the left and right
			{
				neighbour = GetNodeAt(node->m_gridPosX + x, node->m_gridPosY);
			}
			else if (x == 0)		// Add the up and down
			{
				neighbour = GetNodeAt(node->m_gridPosX, node->m_gridPosY + 1);
			}

			if (neighbour != NULL)
			{
				// Add the neighbour into the vector of neighbours
				neighbours.push_back(neighbour);
			}
		}
	}

	return neighbours;
}
