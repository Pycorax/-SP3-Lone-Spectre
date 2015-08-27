#include "PathFinder.h"

#include <unordered_set>

using std::unordered_set;

PathFinder::PathFinder()
{
}


PathFinder::~PathFinder()
{
}

void PathFinder::initPathFinder(TileMap * _tileMap)
{
	m_nodeGrid.Init(_tileMap);
}

void PathFinder::UpdatePath(int tileSize)
{
	Vector2 startPos = pathFinder_getTilePosition();
	AINode* startNode = m_nodeGrid.GetNodeAt(startPos.x, startPos.y);

	vector<AINode*> openSet;
	unordered_set<AINode*> closedSet;
	openSet.push_back(startNode);

	while (openSet.size() > 0) 
	{
		AINode* currentNode = openSet.front();

		// Set current node to the node with the lowest FCost in openSet
		for (vector<AINode*>::iterator node = openSet.begin(); node != openSet.end(); ++node) 
		{
			int thisFCost = (*node)->GetFCost();
			int currentFCost = currentNode->GetFCost();

			if (thisFCost < currentFCost || thisFCost == currentFCost && (*node)->m_HCost < currentNode->m_HCost)
			{
				currentNode = (*node);
			}
		}

		// Move current from open to closed
		for (vector<AINode*>::iterator openNode = openSet.begin(); openNode != openSet.end(); ++openNode)
		{
			if ((*openNode) == currentNode)
			{
				openSet.erase(openNode);
				closedSet.insert(currentNode);

				break;
			}
		}

		// We found the path!!
		if (currentNode == m__target) 
		{
			retracePath(startNode, m__target);
			return;
		}

		vector<AINode*> neighbourList = m_nodeGrid.GetNeighboursOf(currentNode);

		for (vector<AINode*>::iterator neighbourIter = neighbourList.begin(); neighbourIter != neighbourList.end(); ++neighbourIter)
		{
			AINode* neighbour = *neighbourIter;

			// Check if this neighbour is accessible
			// THEN 
			// Check if this neighbour was closed (logically, this should go before the wall check but this is more expensive so we're checking it afterwards)
			if (neighbour->m_isWall || closedSet.find(neighbour) == closedSet.end())
			{
				continue;
			}

			// Calculate the movement cost
			int movementCost = currentNode->m_GCost + getDistance(currentNode, neighbour);

			// Check if neighbour is not in open
			bool neighbourInOpen = false;
			for (vector<AINode*>::iterator openNode = openSet.begin(); openNode != openSet.end(); ++openNode)
			{
				if (neighbour == *openNode)
				{
					neighbourInOpen = true;
					break;
				}
			}

			// If new path to the neighbour is shorter OR neighbour hasn't been opened yet
			if (movementCost < neighbour->m_GCost || !neighbourInOpen)
			{
				neighbour->m_GCost = movementCost;
				neighbour->m_HCost = getDistance(neighbour, m__target);
				neighbour->m__parentNode = currentNode;

				if (!neighbourInOpen)
				{
					openSet.push_back(neighbour);
				}
			}
		}
	}
}

void PathFinder::SetTarget(Vector2 pos, int tileSize)
{
	pos.x = pos.x / tileSize;
	pos.y = pos.y / tileSize;
	m__target = m_nodeGrid.GetNodeAt(pos.x, pos.y);
}

vector<AINode*> PathFinder::GetPath(void)
{
	return m_resultPath;
}

void PathFinder::retracePath(AINode* startNode, AINode* endNode)
{
	vector<AINode*> path;
	AINode* currentNode = endNode;

	while (currentNode != startNode)
	{
		path.push_back(currentNode);
		currentNode = currentNode->m__parentNode;
	}

	m_resultPath = path;
}

int PathFinder::getDistance(AINode * nodeA, AINode * nodeB)
{
	Vector2 dist;

	dist.x = abs(nodeA->m_gridPosX - nodeB->m_gridPosX);
	dist.y = abs(nodeA->m_gridPosY - nodeB->m_gridPosY);

	/* For Diagonal
	if (dist.x > dist.y)
	{
		return DIAG_TILE_DIST * dist.y + ADJ_TILE_DIST * (dist.x - dist.y);
	}
	else
	{
		return DIAG_TILE_DIST * dist.x + ADJ_TILE_DIST * (dist.y - dist.x);
	}
	*/

	return ADJ_TILE_DIST * (dist.x + dist.y);
}