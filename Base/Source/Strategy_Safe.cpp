#include "Strategy_Safe.h"

#include "MyMath.h"

Strategy_Safe::Strategy_Safe()
{
}


Strategy_Safe::~Strategy_Safe()
{
}

void Strategy_Safe::Update(double dt)
{
	Vector2 movement;

	if (m_dest.x - m_enemyPos.x > Math::EPSILON)
	{
		movement.x = -5.0f;
	}
	else if (m_dest.x - m_enemyPos.x < Math::EPSILON)
	{
		movement.x = 5.0f;
	}

	m_enemyPos += movement;
}
