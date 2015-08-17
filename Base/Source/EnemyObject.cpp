#include "EnemyObject.h"



EnemyObject::EnemyObject()
	: m_strat(NULL)
{
}


EnemyObject::~EnemyObject()
{
}

void EnemyObject::Update(double dt)
{
	if (m_strat != NULL)
	{
		m_strat->Update(dt);
		m_transforms.Translation = m_strat->GetEnemyPosition();
	}
}

void EnemyObject::SetTarget(Vector2 dest)
{
	m_target = dest;

	// Update the strategy when updating the enemy
	if (m_strat != NULL)
	{
		m_strat->SetDestination(m_target);
	}
}

void EnemyObject::ChangeStrategy(Strategy * newStrat, bool deleteOld)
{
	if (deleteOld)
	{
		if (m_strat != NULL)
		{
			delete m_strat;
			m_strat = NULL;
		}
	}

	m_strat = newStrat;

	if (m_strat != NULL)
	{
		m_strat->SetEnemyPosition(m_transforms.Translation.ToVector2());
	}
}

Vector2 EnemyObject::GetTarget(void)
{
	return m_target;
}

