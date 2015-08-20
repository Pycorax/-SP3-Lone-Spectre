#include "Collider2D.h"


Collider2D::Collider2D(void) : m_colliderType(CT_RADIUS)
{
}


Collider2D::~Collider2D(void)
{
}

void Collider2D::SetColliderType(const E_COLLIDER_TYPE type)
{
	m_colliderType = type;
}

bool Collider2D::CollideWith(Collider2D* _other, const double DT)
{
	if (!collider2D_isActive() || !_other->collider2D_isActive())
	{
		return false;		// False if either object is inactive
	}

	// Use more precise but expensive box collision detection algorithms if a normal is provided and a velocity is returned by the other
	if (collider2D_getNormal() != Vector3::ZERO_VECTOR && collider2D_getVelocity() != Vector3::ZERO_VECTOR)
	{
		return preciseCollideWith(_other, DT);
	}

	// Delegate the correct collider function
	switch (m_colliderType)
	{
		case CT_RADIUS:
			return radiusCollideWith(_other);
			break;
		case CT_AABB:
			return aabbCollideWith(_other);
			break;
	}

	return false;
}

void Collider2D::calcAABBBounds()
{
	Transform t = collider2D_getTransforms();

	if (m_oldTransform == t)
	{
		// Don't calculate if the old transforms are the same as the current
		return;
	}
	else
	{
		// Set the min and max bound
		m_minBound = t.Translation - t.Scale * 0.5;
		m_maxBound = t.Translation + t.Scale * 0.5;

		// Update the old transforms
		m_oldTransform = t;
	}
}

bool Collider2D::radiusCollideWith(Collider2D * _other)
{
	Transform t = collider2D_getTransforms();
	Transform ot = _other->collider2D_getTransforms();

	// Discard Z
	t.Translation.z = ot.Translation.z = 0.0f;

	float distSquared = (t.Translation - ot.Translation).LengthSquared();
	float combinedRadius = t.Scale.x * 0.5 + ot.Scale.x * 0.5;

	if (distSquared <= combinedRadius * combinedRadius)
	{
		return true;
	}

	return false;
}

bool Collider2D::aabbCollideWith(Collider2D * _other)
{
	// Calculate Bounds
	this->calcAABBBounds();
	_other->calcAABBBounds();

	// AABB Collision checking
	if (
		m_minBound.x < _other->m_maxBound.x && m_maxBound.x > _other->m_minBound.x
		&&
		m_minBound.y < _other->m_maxBound.y && m_maxBound.y > _other->m_minBound.y
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Collider2D::preciseCollideWith(Collider2D * _other, const double DT)
{
	Transform t = collider2D_getTransforms();
	Transform ot = _other->collider2D_getTransforms();

	// For Thickness
	// |(w0 - b1).N| < r + h / 2
	Vector3 w0 = t.Translation;
	Vector3 b1 = ot.Translation + _other->collider2D_getVelocity() * static_cast<float>(DT);
	Vector3 N = collider2D_getNormal();
	float r = _other->collider2D_getTransforms().Scale.x;
	float h = collider2D_getTransforms().Scale.x;

	// For Length
	// |(w0 - b1).NP| < r + l / 2
	Vector3 NP(-N.y, N.x);
	float l = collider2D_getTransforms().Scale.y;

	return abs((w0 - b1).Dot(N)) < r + h * 0.5 && abs((w0 - b1).Dot(NP)) < r + l * 0.5;
}
