#ifndef STRATEGY_H
#define STRATEGY_H

#include "Vector2.h"

class Strategy
{
	protected:
		Vector2 m_dest;
		Vector2 m_enemyPos;

	public:
		Strategy();
		virtual ~Strategy();

		virtual void Update(double dt) = 0;

		// Setters
		virtual void SetDestination(Vector2 dest);
		virtual void SetEnemyPosition(Vector2 enemyPos);
		
		// Getters
		virtual Vector2 GetEnemyPosition(void) const;

		int CalculateDistanceSquared(void);
};

#endif