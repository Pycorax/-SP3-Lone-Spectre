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
	if (_other->collider2D_getNormal() != Vector3::ZERO_VECTOR && collider2D_getVelocity() != Vector3::ZERO_VECTOR)
	{
		//return aabbCollideWith(_other);
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

	// Prevent internal collision
	Vector3 relativeVel = collider2D_getVelocity() - _other->collider2D_getVelocity();
	Vector3 relativeDir = t.Translation - ot.Translation;
	float totalRadius = t.Scale.x * 0.5 + ot.Scale.x * 0.5;

	if (relativeVel.Dot(-relativeDir) < 0) // Not moving towards each other
	{
		return false;
	}

	// Discard Z
	t.Translation.z = ot.Translation.z = 0.0f;

	float distSquared = (t.Translation - ot.Translation).LengthSquared();
	if (distSquared <= totalRadius * totalRadius)
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
	/*
	// For Thickness
	// |(w0 - b1).N| < r + h / 2
	Vector3 w0 = ot.Translation;
	Vector3 b1 = t.Translation + collider2D_getVelocity() * DT;
	Vector3 N = _other->collider2D_getNormal();
	float r = t.Scale.x;
	float h = ot.Scale.y;

	// For Length
	// |(w0 - b1).NP| < r + l / 2
	Vector3 NP(-N.y, N.x);
	float l = ot.Scale.x;

	Vector3 vel = collider2D_getVelocity();

	if (abs(vel.y) > abs(vel.x))
	{
		return abs((w0 - b1).Dot(N)) < (h + r) * 0.5 && abs((w0 - b1).Dot(NP)) < r + l * 0.5;
	}
	else
	{
		return abs((w0 - b1).Dot(N)) * 2 < (h + r) * 0.5 && abs((w0 - b1).Dot(NP)) < r + l * 0.5;
	}
	*/

	Collider2D* ball = NULL;
	Collider2D* wall = NULL;

	// Assign the ball and wall correctly
	if (collider2D_getNormal() != Vector2::ZERO_VECTOR && _other->collider2D_getNormal() == Vector2::ZERO_VECTOR)
	{
		ball = _other;
		wall = this;
	}
	else if (collider2D_getNormal() == Vector2::ZERO_VECTOR && _other->collider2D_getNormal() != Vector2::ZERO_VECTOR)
	{
		ball = this;
		wall = _other;
	}
	else
	{
		// If both are walls or both are balls, this can never be true so don't bother calculating
		return false;
	}

	// Direction check to prevent internal collision
	float dotValue = ball->collider2D_getVelocity().Dot(wall->collider2D_getNormal());
	if (dotValue > 0)
	{
		return false;
	}

	Transform wallT = wall->collider2D_getTransforms();
	Transform ballT = ball->collider2D_getTransforms();;

	Vector2 w0 = wallT.Translation; // No need to check 1 frame ahead for wall
	Vector2 b1 = ballT.Translation + ball->collider2D_getVelocity() * DT; // Check pos 1 frame ahead for ball
	Vector2 N = wall->collider2D_getNormal();
	float r = ball->collider2D_getTransforms().Scale.x * 0.5f; // Half of ball diameter (Radius)
	float h = wall->collider2D_getTransforms().Scale.x; // Depth of wall
	float l = wall->collider2D_getTransforms().Scale.y; // Length of wall
	Vector2 NP(-N.y, N.x);
	if (abs((w0 - b1).Dot(N)) < r + (h * 0.5f) && abs((w0 - b1).Dot(NP)) < r + (l * 0.5f))
	{
		return true;
	}
	return false;
}
