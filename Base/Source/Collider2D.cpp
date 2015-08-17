#include "Collider2D.h"


Collider2D::Collider2D(void)
{
}


Collider2D::~Collider2D(void)
{
}

bool Collider2D::CollideWith(Collider2D* _other)
{
	if (!isActive() || !_other->isActive()) 
	{
		return false;		// False if either object is inactive
	}

	// Calculate Bounds
	this->calcBounds();
	_other->calcBounds();

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

void Collider2D::calcBounds()
{
	Transform t = getTransforms();

	if (m_oldTransform == t)
	{
		// Don't calculate if the old transforms are the same as the current
		return;
	}
	else
	{
		// Set the min and max bound
		m_minBound = t.Translation - t.Scale;
		m_maxBound = t.Translation + t.Scale;

		// Update the old transforms
		m_oldTransform = t;
	}
}