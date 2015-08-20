#include "PhysicalObject.h"

PhysicalObject::PhysicalObject() : GameObject2D(), Collider2D(), Physics2D()
{
}


PhysicalObject::~PhysicalObject()
{
}

Transform PhysicalObject::collider2D_getTransforms(void) const
{
	return m_transforms;
}

bool PhysicalObject::collider2D_isActive(void) const
{
	return m_active;
}

Vector3 PhysicalObject::collider2D_getNormal(void) const
{
	return m_normal;
}

Vector3 PhysicalObject::collider2D_getVelocity(void) const
{
	return m_velocity;
}

Transform PhysicalObject::physics2D_getTransforms() const
{
	return m_transforms;
}

void PhysicalObject::physics2D_setTransforms(Transform t)
{
	m_transforms = t;
}
