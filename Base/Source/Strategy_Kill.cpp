#include "Strategy_Kill.h"

#include "MyMath.h"

Strategy_Kill::Strategy_Kill()
	: m_aiState(IDLE_STATE)
	, m_idleTimer(0.0)
	, m_patrolDist(0.0f)
	, m_patrolLeft(false)
{
}


Strategy_Kill::~Strategy_Kill()
{
}

void Strategy_Kill::Update(double dt)
{
	int distHeroToEnemy = CalculateDistanceSquared();

	if (distHeroToEnemy < 10000.0f)
	{
		if (distHeroToEnemy < 625.0f)
		{
			m_aiState = REPEL_STATE;
		}
		else
		{
			m_aiState = ATTACK_STATE;
		}
	}
	else
	{
		if (m_idleTimer > 3.0)
		{
			// If just switching to patrol
			if (m_aiState != PATROL_STATE)
			{
				m_patrolDist = 0;
				m_patrolLeft = true;
			}

			m_aiState = PATROL_STATE;
		}
		else
		{
			// If just switching to idle
			if (m_aiState != IDLE_STATE)
			{
				m_idleTimer = 0.0;
			}

			m_aiState = IDLE_STATE;
			m_idleTimer += dt;
		}
	}

	Vector2 movement;

	switch (m_aiState)
	{
		case ATTACK_STATE:
			if ((m_dest.x - m_enemyPos.x) > Math::EPSILON)
			{
				movement.x = 5.0f;
			}
			else if ((m_dest.x - m_enemyPos.x) < -Math::EPSILON)
			{
				movement.x = -5.0f;
			}
			break;
		case REPEL_STATE:
			if ((m_dest.x - m_enemyPos.x) > Math::EPSILON)
			{
				movement.x = -5.0f;
			}
			else if ((m_dest.x - m_enemyPos.x) < -Math::EPSILON)
			{
				movement.x = 5.0f;
			}
			break;
		case PATROL_STATE:
			{
				const float DIST_PATROL = 250.0f;

				if (m_patrolDist < Math::EPSILON)
				{
					m_patrolLeft = false;
				}
				else if (m_patrolDist > DIST_PATROL)
				{
					m_patrolLeft = true;
				}

				if (m_patrolLeft)
				{
					movement.x = -5.0f;
					m_patrolDist += -5.0f;
				}
				else
				{
					movement.x = 5.0f;
					m_patrolDist += 5.0f;
				}
			}
			break;
		default:

			break;
	}

	m_enemyPos += movement;
}

Strategy_Kill::AI_STATE Strategy_Kill::GetState(void)
{
	return m_aiState;
}

void Strategy_Kill::SetState(AI_STATE state)
{
	m_aiState = state;
}
