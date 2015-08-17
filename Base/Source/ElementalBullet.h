#pragma once
#include "BulletObject.h"
#include "Elemental.h"

class ElementalBullet :	public BulletObject, public Elemental
{
	public:
		ElementalBullet();
		virtual ~ElementalBullet();

		void Init(Vector3 position, Vector3 velocity, Vector3 scale, float lifetime, ELEMENT_TYPE element);
};

