#ifndef STRATEGY_SAFE_H
#define STRATEGY_SAFE_H

#include "Strategy.h"

class Strategy_Safe	: public Strategy
{
	public:
		Strategy_Safe();
		~Strategy_Safe();

		void Update(double dt);
};

#endif