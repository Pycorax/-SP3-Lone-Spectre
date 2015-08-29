#include "AINode.h"


AINode::AINode(bool isWall, int gridPosX, int gridPosY)
	: m_GCost(0)
	, m_HCost(0)
	, m_isWall(isWall)
	, m_gridPosX(gridPosX)
	, m_gridPosY(gridPosY)
	, m__parentNode(NULL)
	, m_state(ANS_UNTOUCHED)
{
}

int AINode::GetFCost(void)
{
	return m_GCost + m_HCost;
}