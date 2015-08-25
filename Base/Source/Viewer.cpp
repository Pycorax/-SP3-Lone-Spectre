#include "Viewer.h"

Viewer::Viewer(int maxRangeX, int maxRangeY)
	: m_viewSideGrowthX(maxRangeX)
	, m_maxRangeY(maxRangeY)
{
}


Viewer::~Viewer()
{
}

void Viewer::Init(int maxRangeX, int maxRangeY)
{
	m_viewSideGrowthX = maxRangeX;
	m_maxRangeY = maxRangeY;
}

int Viewer::GetViewSideGrowthX(void)
{
	return m_viewSideGrowthX;
}

int Viewer::GetMaxRangeY(void)
{
	return m_maxRangeY;
}