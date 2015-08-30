#include "PathFinder.h"

PathFinder::PathFinder()
{
}


PathFinder::~PathFinder()
{
	m_nodeGrid.Exit();
}

void PathFinder::InitPathFinder(MapGrid2D * _tileMap)
{
	m_nodeGrid.Init(_tileMap);
	m_nodeGrid.ResetNodeStates();
}

void PathFinder::UpdatePath(void)
{
	// Stop Search Conditions
	bool found = false;
	int openNodes = 0;

	// Find my position
	unsigned startPosX = 0, startPosY = 0;
	pathFinder_getTilePosition(startPosX, startPosY);
	// Set the Start Node
	AINode* _startNode = m_nodeGrid.GetNodeAt(startPosX, startPosY);
	// Set the Target Note
	AINode* _endNode = m__target;

	// Error Checking
	if (_startNode == NULL || _endNode == NULL)
	{
		// Don't do anything if start and end are not defined
		return;
	}

	// Set the current node to the start node
	AINode* _current = _startNode;
	// Set the current node to open
	_current->m_state = AINode::ANS_OPENED;
	// Increment openNodes
	++openNodes;

	while (openNodes > 0 && !found)	
	{
		if (_endNode == _current)
		{
			found = true;
			break;
		}

		// Open up the neighbouring nodes		
		vector<AINode*> neighbourNodes = m_nodeGrid.GetNeighboursOf(_current);
		// Increment openNodes
		openNodes += neighbourNodes.size();

#pragma region // Calculate their costs of these neighbours and get the cheapest
		// Use the first as a comparison
		AINode* _cheapest = NULL;

		// Search through all neighbours for the cheapest
		for (vector<AINode*>::iterator nodeIter = neighbourNodes.begin(); nodeIter != neighbourNodes.end(); ++nodeIter)
		{
			AINode* _neighbour = *nodeIter;

			if (_neighbour->m_isWall)
			{
				continue;
			}

			// Recalculate the costs of the neighbour
			int costToMoveTo = m_nodeGrid.GetDistance(_current, _neighbour);
			int totalMoveCost = _neighbour->m_GCost + costToMoveTo;

			// If the move cost by moving to this tile in this path is shorter OR is not open
			if ((totalMoveCost < _neighbour->m_GCost || _neighbour->m_state != AINode::ANS_OPENED))
			{
				// Recalculate the cost
				_neighbour->m_GCost = totalMoveCost;
				_neighbour->m_HCost = m_nodeGrid.GetDistance(_neighbour, _endNode);

				if (_neighbour->m_state == AINode::ANS_UNTOUCHED)
				{
					// We calculated it, so it's opened now; Only if it was untouched
					_neighbour->m_state = AINode::ANS_OPENED;
				}
			}

			// If FCost is Lower, this is cheaper
			if (_cheapest == NULL || _neighbour->GetFCost() < _cheapest->GetFCost())
			{
				_cheapest = _neighbour;
			}
			else if (_neighbour->GetFCost() == _cheapest->GetFCost())
			{
				// If both are equal, choose the one with the lower HCost
				if (_neighbour->m_HCost < _cheapest->m_HCost)
				{
					_cheapest = _neighbour;
				}
			}
		}
#pragma endregion

		// Set the current node to closed		
		_current->m_state = AINode::ANS_CLOSED;
		// Decrement openNodes
		--openNodes;

		// Set the cheapest node above's parent to the current node
		_cheapest->m__parentNode = _current;
		// Set the current node to the cheapest node above
		_current = _cheapest;
		/*
		* Debug Prints

		system("cls");

		vector<vector<AINode*>> grid = m_nodeGrid.GetGrid();
		for (vector<vector<AINode*>>::iterator rowIter = grid.begin(); rowIter != grid.end(); ++rowIter)
		{
			vector<AINode*> row = *rowIter;

			for (vector<AINode*>::iterator colIter = row.begin(); colIter != row.end(); ++colIter)
			{
				AINode* col = *colIter;

				if (col->m_isWall && col->m_state == AINode::ANS_UNTOUCHED)
				{
					std::cout << static_cast<unsigned char>(178);
				}
				else
				{
					std::cout << static_cast<int>(col->m_state);
				}
			}

			std::cout << std::endl;
		}

		system("pause");
		*/
	}

	if (found)
	{
		// Retrace
		m_resultPath.clear();
		while (_current != _startNode)
		{
			// Add the path into the m_resultPath
			m_resultPath.push_back(_current);
			_current = _current->m__parentNode;
		}

		/*
		 * Debug Prints
		 *
		system("cls");

		vector<vector<AINode*>> grid = m_nodeGrid.GetGrid();
		for (vector<vector<AINode*>>::iterator rowIter = grid.begin(); rowIter != grid.end(); ++rowIter)
		{
			vector<AINode*> row = *rowIter;

			for (vector<AINode*>::iterator colIter = row.begin(); colIter != row.end(); ++colIter)
			{
				AINode* col = *colIter;

				if (col == _startNode)
				{
					std::cout << static_cast<unsigned char>(248);
				}
				else if (col == _endNode)
				{
					std::cout << static_cast<unsigned char>(254);
				}
				else if (col->m_isWall && col->m_state == AINode::ANS_UNTOUCHED)
				{
					std::cout << static_cast<unsigned char>(178);
				}
				else
				{
					std::cout << static_cast<int>(col->m_state);
				}
			}

			std::cout << std::endl;
		}

		system("pause");*/
		
	}
	else
	{
		// Set a not found path flag
		m_resultPath.clear();
	}

	// After we're done with everything, reset the states of the nodes back to their original untouched states for the next time this is run
	m_nodeGrid.ResetNodeStates();
}

void PathFinder::SetTarget(unsigned tileXPos, unsigned tileYPos)
{
	m__target = m_nodeGrid.GetNodeAt(tileXPos, tileYPos);
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