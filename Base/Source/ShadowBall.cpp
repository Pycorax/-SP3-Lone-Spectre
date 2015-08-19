#include "ShadowBall.h"

ShadowBall::ShadowBall()
{
}


ShadowBall::~ShadowBall()
{
}

Transform ShadowBall::collider2D_getTransforms(void) const
{
	return m_transforms;
}

bool ShadowBall::collider2D_isActive(void) const
{
	return m_active;
}

Vector3 ShadowBall::collider2D_getNormal(void) const
{
	return NULL;
}

Vector3 ShadowBall::collider2D_getVelocity(void) const
{
	return m_velocity;
}

Transform ShadowBall::physics2D_getTransforms() const
{
	return m_transforms;
}

void ShadowBall::physics2D_setTransforms(Transform t)
{
	m_transforms = t;
}
