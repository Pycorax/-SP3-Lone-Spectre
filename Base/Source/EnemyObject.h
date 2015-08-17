#ifndef ENEMY_OBJECT_H
#define ENEMY_OBJECT_H

// Inheritance
#include "CharObject.h"

// Others
#include "Strategy.h"

class EnemyObject : public CharObject
{
	protected:
		// Target
		Vector2 m_target;
		// Strategy
		Strategy* m_strat;

	public:
		EnemyObject();
		virtual ~EnemyObject();

		virtual void Update(double dt);

		void SetTarget(Vector2 target);
		Vector2 GetTarget(void);

		void ChangeStrategy(Strategy* newStrat, bool deleteOld = true);
};

#endif