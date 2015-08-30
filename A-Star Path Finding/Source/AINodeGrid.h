/******************************************************************************/
/*!
\file	AINodeGrid.h
\author Tng Kah Wei
\brief
Class to define a 2D grid of AINodes for calculation of a Tile Based Path 
Finding System
*/
/******************************************************************************/
#ifndef AI_NODE_GRID_H
#define AI_NODE_GRID_H

// STL Includes
#include <vector>

// Other Includes
#include "AINode.h"
#include "MapGrid2D.h"

// Using Directives
using std::vector;

class AINodeGrid
{
	static const int ADJ_TILE_DIST = 10;
	static const int DIAG_TILE_DIST = 30;

	vector<vector<AINode*>> m_grid;					// A grid of Nodes for A* Path Finding

public:
	AINodeGrid();
	~AINodeGrid();

	// Function to build the node grid
	void Init(MapGrid2D* _gridMap);

	// Function to destroy the node grid
	void Exit(void);

	/*
	 * Setters
	 */
	// Function to reset the m_state of all nodes to ANS_UNTOUCHED
	void ResetNodeStates(void);
	//void Calculate

	/*
	 * Getters
	 */
	// Function to get a const grid for reference
	const vector<vector<AINode*>>& GetGrid(void) const;
	// Function to get a node
	AINode* GetNodeAt(int xPos, int yPos);
	// Function to get the neighbouring AINodes of a Node provided
	vector<AINode*> GetNeighboursOf(AINode* _node);
	// Function to get the neighbour AINode* with the lowest FCost of a Node provided
	//AINode* GetCheapestNeighboursOf(AINode* _node);
	// Function to calculate the distance from nodeA to nodeB
	int GetDistance(AINode * nodeA, AINode * nodeB);
};

#endif