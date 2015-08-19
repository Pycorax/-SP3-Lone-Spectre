#ifndef PHYSICAL_OBJECT_H
#define PHYSICAL_OBJECT_H

#include "GameObject2D.h"
#include "Collider2D.h"
#include "Physics2D.h"

class PhysicalObject : public GameObject2D, public Collider2D, public Physics2D 
{
public:
	PhysicalObject();
	~PhysicalObject();

private:
	/*
	 * Abstract Functions for Collider2D
	 */
	virtual Transform collider2D_getTransforms(void) const;
	virtual bool collider2D_isActive(void) const;
	virtual Vector3 collider2D_getNormal(void) const;
	virtual Vector3 collider2D_getVelocity(void) const;

	/*
	* Abstract Functions for Physics2D
	*/
	// Function to send Physics2D the actual transforms of the child.
	virtual Transform physics2D_getTransforms() const;
	// Function to set the actual transforms of the child from Physics2D.
	virtual void physics2D_setTransforms(Transform t);
};

#endif