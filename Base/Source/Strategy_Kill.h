#ifndef STRATEGY_KILL_H
#define STRATEGY_KILL_H

#include "Strategy.h"

class Strategy_Kill : public Strategy
{
	public:
		enum AI_STATE
		{
			IDLE_STATE,
			REPEL_STATE,
			ATTACK_STATE,
			PATROL_STATE,
			NUM_ENEMY_STATE
		};

	private:
		AI_STATE m_aiState;
		double m_idleTimer;
		float m_patrolDist;
		bool m_patrolLeft;

	public:
		Strategy_Kill();
		virtual ~Strategy_Kill();

		void Update(double dt);

		AI_STATE GetState(void);

		void SetState(AI_STATE state);
};

#endif