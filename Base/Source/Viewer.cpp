#include "Viewer.h"

Viewer::Viewer(int maxRangeX, int maxRangeY)
	: m_viewSideGrowthX(maxRangeX)
	, m_maxRangeY(maxRangeY)
{
}


Viewer::~Viewer()
{
}

void Viewer::InitViewer(int maxRangeX, int maxRangeY)
{
	m_viewSideGrowthX = maxRangeX;
	m_maxRangeY = maxRangeY;
}

void Viewer::AddViewedTileIndex(Vector2 index)
{
	m_viewedTileIndexes.push_back(index);
}

void Viewer::ClearViewedTileIndex(void)
{
	m_viewedTileIndexes.clear();
}

int Viewer::GetViewSideGrowthX(void)
{
	return m_viewSideGrowthX;
}

int Viewer::GetMaxRangeY(void)
{
	return m_maxRangeY;
}

vector<Vector2> Viewer::GetViewedTileIndex(void)
{
	return m_viewedTileIndexes;
}
