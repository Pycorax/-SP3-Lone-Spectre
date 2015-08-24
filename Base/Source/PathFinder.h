#ifndef PATH_FINDER_H
#define PATH_FINDER_H

// STL Includes
#include <vector>

// Other Includes
#include "TileMap.h"
#include "AINodeGrid.h"

// Using Directives
using std::vector;

class PathFinder
{
private:
	static const int ADJ_TILE_DIST = 10;
	static const int DIAG_TILE_DIST = 14;

	AINodeGrid m_nodeGrid;
	AINode* m__target;								// The position in tile space of the target
	vector<AINode*> m_resultPath;

public:
	PathFinder(void);
	virtual ~PathFinder(void);

	// Function to build the node grid
	void initPathFinder(TileMap* _tileMap);	
	void UpdatePath(int tileSize);

	// Setters
	void SetTarget(Vector2 pos, int tileSize);

	// Getters
	vector<AINode*> GetPath(void);

private:
	// Function to retrace from the endNode to the starting node
	void retracePath(AINode* startNode, AINode* endNode);
	int getDistance(AINode* nodeA, AINode* nodeB);

protected:
	/*
	*	Abstract Functions
	*	These functions need to be defined in child classes and serves as the interface between child classes and this class.
	*/
	// Function to get the player's current position
	virtual Vector2 pathFinder_getTilePosition(void) = 0;
};

#endif