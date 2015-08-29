/******************************************************************************/
/*!
\file	AINode.h
\author Tng Kah Wei
\brief
Class to define a AINodes in an AINodeGrid used for calculations in a Tile-
Based Path Finding System
*/
/******************************************************************************/
#ifndef AI_NODE_H
#define AI_NODE_H

#include <iostream>

struct AINode
{
	// Enums
	enum E_AI_NODE_STATE	// The possible states for a node in Path Finding
	{
		ANS_UNTOUCHED,
		ANS_OPENED,
		ANS_CLOSED,
		NUM_AI_NODE_STATE
	};

	// Variables
	E_AI_NODE_STATE m_state;// The state of this node. Whether is has been opened or closed or untouched.	
	int m_GCost;			// Movement cost from start to to this node
	int m_HCost;			// Movement cost from this node to the destination
	bool m_isWall;			// Controls if this node is traversable
	AINode* m__parentNode;	// A Pointer to the parent node

							// Grid Positioning
	int m_gridPosX;			// Position of this node in the grid's horizontal axis
	int m_gridPosY;			// Position of this node int he grid's vertical axis

	// Functions
	AINode(bool isWall = true, int gridPosX = 0, int gridPosY = 0);

	/*
	 * Getters
	 */
	int GetFCost(void);		// Calculates and returns the overall F-cost
};

#endif