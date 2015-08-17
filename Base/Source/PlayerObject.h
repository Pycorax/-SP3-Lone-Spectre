#ifndef PLAYER_OBJECT_H
#define PLAYER_OBJECT_H

#include "CharObject.h"
#include "Elemental.h"

class PlayerObject : public CharObject
{
	private:
		// Singleton
		static const int MAX_INSTANCES = 1;
		static PlayerObject* instances;

		PlayerObject();

	public:
		static PlayerObject* GetInstance(int instance = 0);
		virtual ~PlayerObject();

		void Update(MapLayer * map, double dt);

		Elemental::ELEMENT_TYPE GetWeaponElement(void);

	private:
		// Other
		void constrainHero(MapLayer* map, double dt);
};

#endif