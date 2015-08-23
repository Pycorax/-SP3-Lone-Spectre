#ifndef AI_NODE_GRID_H
#define AI_NODE_GRID_H

// STL Includes
#include <iostream>

// Other includes
#include "TileMap.h"

struct AINode
{
	int m_GCost;			// Movement cost from start to to this node
	int m_HCost;			// Movement cost from this node to the destination
	bool m_isWall;			// Controls if this node is traversable
	AINode* m__parentNode;	// A Pointer to the parent node

	// Grid Positioning
	int m_gridPosX;			// Position of this node in the grid's horizontal axis
	int m_gridPosY;			// Position of this node int he grid's vertical axis

	AINode(int gCost = 0, int hCost = 0, bool isWall = true, int gridPosX = 0, int gridPosY = 0)
		: m_GCost(gCost)
		, m_HCost(hCost)
		, m_isWall(isWall)
		, m_gridPosX(gridPosX)
		, m_gridPosY(gridPosY)
		, m__parentNode(NULL)
	{
	}

	void Set(int gCost = 0, int hCost = 0, bool isWall = true, int gridPosX = 0, int gridPosY = 0)
	{
		m_GCost = gCost;
		m_HCost = hCost;

		m_isWall = isWall;

		m_gridPosX = gridPosX;
		m_gridPosY = gridPosY;
	}

	int GetFCost(void)
	{
		return m_GCost + m_HCost;
	}
};

class AINodeGrid
{
	vector<vector<AINode*>*> m_nodeGrid;				// A grid of Nodes for A* Path Finding
	vector<AINode*> m_path;

public:
	AINodeGrid();
	~AINodeGrid();

	// Function to build the node grid
	void Init(TileMap* _tileMap);
	
	// Function to destroy the node grid
	void Exit(void);

	// Function to get a node
	AINode* GetNodeAt(int xPos, int yPos);

	// Function to get the neighbouring AINodes of a Node provided
	vector<AINode*> GetNeighboursOf(AINode* node);
};

#endif