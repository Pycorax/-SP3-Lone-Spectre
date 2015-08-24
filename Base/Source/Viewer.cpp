#include "Viewer.h"

Viewer::Viewer()
	: m_viewSideGrowthX(0)
	, m_maxRangeY(0)
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