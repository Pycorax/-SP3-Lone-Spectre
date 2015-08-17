#include "GoodieObjectFactory.h"

#include "TreasureGoodie.h"
#include "HealthGoodie.h"

#include <cstdlib>

GoodieObjectFactory::GoodieObjectFactory()
{
}


GoodieObjectFactory::~GoodieObjectFactory()
{
}

GoodieObject * GoodieObjectFactory::Create(GOODIE_TYPE type)
{
	GoodieObject* goody = NULL;

	switch (type)
	{
		case TREASURE_GOODIE:
			goody = new TreasureGoodie;
			break;
		case HEALTH_GOODIE:
			goody = new HealthGoodie;
			break;
		default:
			break;
	}

	return goody;
}

