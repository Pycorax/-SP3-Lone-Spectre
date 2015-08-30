#include "AINodeGrid.h"



AINodeGrid::AINodeGrid()
{
}


AINodeGrid::~AINodeGrid()
{
}

void AINodeGrid::Init(MapGrid2D * _gridMap)
{
	// Vars to store the dimensions for the map grid
	unsigned mapWidth = 0, mapHeight = 0;

	// Get the dimensions of the map grid
	_gridMap->mapGrid2D_GetMapSize(mapWidth, mapHeight);

	// Start Building the Map Grid
	for (size_t row = 0; row < mapHeight; ++row)
	{
		// Construct the Rows
		vector<AINode*> newNodeRow;

		for (size_t col = 0; col < mapWidth; ++col)
		{
			// Check if the node at this portion of the grid is blocked
			bool block = _gridMap->mapGrid2D_TileIsBlocked(col, row);

			// Construct the Node at this Column
			AINode* newNode = new AINode(block, col, row);

			// Add this node to the row
			newNodeRow.push_back(newNode);
		}

		// Add this row to the grid
		m_grid.push_back(newNodeRow);
	}
}

void AINodeGrid::Exit(void)
{
	while (m_grid.size() > 0)
	{
		// Delete all the nodes in this row
		while (m_grid.back().size() > 0)
		{
			// Delete this node
			if (m_grid.back().back() != NULL)
			{
				delete m_grid.back().back();
				m_grid.back().pop_back();
			}
		}

		// Delete this row
		m_grid.pop_back();
	}
}

void AINodeGrid::ResetNodeStates(void)
{
	for (vector<vector<AINode*>>::iterator rowIter = m_grid.begin(); rowIter != m_grid.end(); ++rowIter)
	{
		vector<AINode*> row = *rowIter;

		for (vector<AINode*>::iterator colIter = row.begin(); colIter != row.end(); ++colIter)
		{
			AINode* col = *colIter;

			col->m_state = AINode::ANS_UNTOUCHED;
			col->m_GCost = 0;
			col->m_HCost = 0;
			col->m__parentNode = NULL;
		}
	}
}

const vector<vector<AINode*>>& AINodeGrid::GetGrid(void) const
{
	return m_grid;
}

AINode * AINodeGrid::GetNodeAt(int xPos, int yPos)
{
	if (
		yPos >= 0 && yPos < m_grid.size()
		&&
		xPos >= 0 && xPos < m_grid.at(yPos).size()
		)
	{
		// Get the neighbour
		return m_grid.at(yPos).at(xPos);
	}
	else
	{
		return NULL;
	}
}

vector<AINode*> AINodeGrid::GetNeighboursOf(AINode * _node)
{
	vector<AINode*> neighbours;

	for (int y = -1; y <= 1; ++y)
	{
		for (int x = -1; x <= 1; ++x)
		{
			// Ignore myself
			if (x == 0 && y == 0)
			{
				continue;
			}

			int tileX = _node->m_gridPosX + x;
			int tileY = _node->m_gridPosY + y;

			// Error checking
			if (tileY >= 0 && tileY < m_grid.size() && tileX >= 0 && tileX < m_grid.at(tileY).size())
			{
				AINode* _neighbour = m_grid.at(tileY).at(tileX);

				// Send it into the neighbour's list
				neighbours.push_back(_neighbour);
			}
		}
	}

	return neighbours;
}

int AINodeGrid::GetDistance(AINode * nodeA, AINode * nodeB)
{
	unsigned distX = 0, distY = 0;

	distX = abs(nodeA->m_gridPosX - nodeB->m_gridPosX);
	distY = abs(nodeA->m_gridPosY - nodeB->m_gridPosY);

	if (distX > distY)
	{
		return DIAG_TILE_DIST * distY + ADJ_TILE_DIST * (distX - distY);
	}
	else
	{
		return DIAG_TILE_DIST * distX + ADJ_TILE_DIST * (distY - distX);
	}
}
