#ifndef COLLIDER_2D_H
#define COLLIDER_2D_H

#include "GObject.h"

class Collider2D
{
private:
	Transform m_oldTransform;
	Vector3 m_maxBound, m_minBound;

public:
	Collider2D(void);
	virtual ~Collider2D(void);
	
	bool CollideWith(Collider2D* _other);

private:	// Functions
	void calcBounds();

	// To be Defined in Child
	Transform getTransforms(void) = 0;						// Function to send Collider2D the actual position of the child
	bool isActive(void) = 0;								// Function to send Collider2D the state of the child
};

#endif