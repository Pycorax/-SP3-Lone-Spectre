#include "Viewer.h"

Viewer::Viewer()
	: m_maxRangeX(0)
	, m_maxRangeY(0)
{
}


Viewer::~Viewer()
{
}

void Viewer::Init(int maxRangeX, int maxRangeY)
{
	m_maxRangeX = maxRangeX;
	m_maxRangeY = maxRangeY;
}

int Viewer::GetMaxRangeX(void)
{
	return m_maxRangeX;
}

int Viewer::GetMaxRangeY(void)
{
	return m_maxRangeY;
}
