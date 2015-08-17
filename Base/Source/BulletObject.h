#ifndef BULLET_OBJECT_H
#define BULLET_OBJECT_H

#include "PhysicsObject.h"

class BulletObject : public PhysicsObject
{
	int m_damage;

	public:
		BulletObject();
		~BulletObject();

		void Init(Vector3 position, Vector3 velocity, Vector3 scale, float lifetime);

		void SetDamage(int dmg);
		
		int GetDamage(void) const;
};

#endif