#include "Strategy.h"



Strategy::Strategy()
{
}


Strategy::~Strategy()
{
}

void Strategy::SetDestination(Vector2 dest)
{
	m_dest = dest;
}

void Strategy::SetEnemyPosition(Vector2 enemyPos)
{
	m_enemyPos = enemyPos;
}

Vector2 Strategy::GetEnemyPosition() const
{
	return m_enemyPos;
}

int Strategy::CalculateDistanceSquared(void)
{
	return (m_dest - m_enemyPos).LengthSquared();
}
