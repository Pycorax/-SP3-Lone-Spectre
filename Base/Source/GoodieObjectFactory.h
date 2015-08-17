#ifndef GOODIES_FACTORY_H
#define GOODIES_FACTORY_H

#include "GoodieObject.h"

class GoodieObjectFactory
{
	public:
		enum GOODIE_TYPE
		{
			TREASURE_GOODIE,
			HEALTH_GOODIE,
			NUM_GOODIES
		};

		GoodieObjectFactory();
		~GoodieObjectFactory();

		GoodieObject* Create(GOODIE_TYPE type);
};

#endif