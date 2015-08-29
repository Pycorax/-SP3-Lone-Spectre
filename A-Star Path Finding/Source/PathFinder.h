#ifndef PATH_FINDER_H
#define PATH_FINDER_H

// STL Includes
#include <vector>

// Other Includes
#include "AINodeGrid.h"

// Using Directives
using std::vector;

class PathFinder
{
private:
	AINodeGrid m_nodeGrid;
	AINode* m__target;								// The position in tile space of the target
	vector<AINode*> m_resultPath;

public:
	PathFinder(void);
	virtual ~PathFinder(void);

	// Function to build the node grid
	void InitPathFinder(MapGrid2D* _tileMap);	
	void UpdatePath(void);

	// Setters
	void SetTarget(unsigned tileXPos, unsigned tileYPos);

	// Getters
	vector<AINode*> GetPath(void);

private:
	// Function to retrace from the endNode to the starting node
	void retracePath(AINode* startNode, AINode* endNode);

protected:
	/*
	*	Abstract Functions
	*	These functions need to be defined in child classes and serves as the interface between child classes and this class.
	*/
	// Function to allow PathFinder to obtain the tile position of the child via reference
	virtual void pathFinder_getTilePosition(unsigned& tileXPos, unsigned& tileYPos) const = 0;
};

#endif