#ifndef GHOST_STRATEGY_H
#define GHOST_STRATEGY_H

#include "Strategy.h"

class GhostStrategy : public Strategy
{
	public:
		GhostStrategy();
		virtual ~GhostStrategy();

		virtual void Update(double dt);
};

#endif