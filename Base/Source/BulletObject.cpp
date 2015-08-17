#include "BulletObject.h"



BulletObject::BulletObject() : PhysicsObject()
{
	m_damage = 0;
	SetType(PhysicsObject::PHY_BALL);
}


BulletObject::~BulletObject()
{
}

void BulletObject::Init(Vector3 position, Vector3 velocity, Vector3 scale, float lifetime)
{
	PhysicsObject::Init(position, velocity, scale);
	SetLifetime(lifetime);
}

void BulletObject::SetDamage(int dmg)
{
	m_damage = dmg;
}

int BulletObject::GetDamage(void) const
{
	return m_damage;
}
