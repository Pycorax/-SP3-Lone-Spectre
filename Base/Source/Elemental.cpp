#include "Elemental.h"



Elemental::Elemental() : m_element(NORMAL_TYPE)
{
}


Elemental::~Elemental()
{
}

Elemental::ELEMENT_TYPE Elemental::GetElement(void)
{
	return m_element;
}

bool Elemental::IsNeutralTo(ELEMENT_TYPE element)
{
	if (m_element == NORMAL_TYPE || element == NORMAL_TYPE || m_element == element)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Elemental::IsVulnerableTo(ELEMENT_TYPE element)
{
	if (m_element == LIGHTNING_TYPE && element == EARTH_TYPE)
	{
		return true;
	}
	else if (m_element == WATER_TYPE && element == LIGHTNING_TYPE)
	{
		return true;
	}
	else if (m_element == EARTH_TYPE && element == WATER_TYPE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Elemental::IsInvulnerableTo(ELEMENT_TYPE element)
{
	if (m_element == LIGHTNING_TYPE && element == WATER_TYPE)
	{
		return true;
	}
	else if (m_element == WATER_TYPE && element == EARTH_TYPE)
	{
		return true;
	}
	else if (m_element == EARTH_TYPE && element == LIGHTNING_TYPE)
	{
		return true;
	}
	else
	{
		return false;
	}
}
