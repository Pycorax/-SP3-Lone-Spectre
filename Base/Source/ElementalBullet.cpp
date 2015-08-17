#include "ElementalBullet.h"



ElementalBullet::ElementalBullet()
{
}


ElementalBullet::~ElementalBullet()
{
}

void ElementalBullet::Init(Vector3 position, Vector3 velocity, Vector3 scale, float lifetime, ELEMENT_TYPE element)
{
	BulletObject::Init(position, velocity, scale, lifetime);
	m_element = element;
}
