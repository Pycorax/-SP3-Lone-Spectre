#ifndef COLLIDER_2D_H
#define COLLIDER_2D_H

#include "GameObject2D.h"

class Collider2D
{
public:		// Enumeration
	enum E_COLLIDER_TYPE							// An enum that lists the types of collision detection methods that Collider2D can use
	{
		CT_RADIUS,
		CT_AABB,
		CT_TOTAL
	};

private:	// Variables
	E_COLLIDER_TYPE m_colliderType;					// Stores the type of collision checking used
	Transform m_oldTransform;						// Holds the transforms at the previous calcAABBBounds() to prevent calculating for no reason when transforms has not changed
	Vector3 m_maxBound, m_minBound;					// Stores the max and min bounds of the hit box

public:		// Functions
	Collider2D(void);
	virtual ~Collider2D(void);
	
	/*
	 * Setters
	 */
	void SetColliderType(const E_COLLIDER_TYPE type);
	
	/*
	 * Other Functions
	 */
	bool CollideWith(Collider2D* _other, const double DT);

private:	// Functions
	// Calculates m_minBound and m_maxBound
	void calcAABBBounds();								
	
	/*
	 *	Helper CollideWith Functions
	 */
	// Radius CollideWith Function
	bool radiusCollideWith(Collider2D* _other);
	// Axis-Aligned Bounding Box CollideWith Function
	bool aabbCollideWith(Collider2D* _other);
	// Forward Checking Collision Detection
	bool preciseCollideWith(Collider2D* _other, double dt);

	/*
	 *	Abstract Functions
	 *	These functions need to be defined in child classes and serves as the interface between child classes and this class.
	 */
	// Function to send Collider2D the actual transforms of the child.
	virtual Transform collider2D_getTransforms(void) const = 0;	
	// Function to send Collider2D the state of the child.
	virtual bool collider2D_isActive(void) const = 0;
	// Function to send Collider2D the normal of the child. This is for more precise box collisions. Forces CollideWith() to use forward checking collision detection if Vector3::ZERO_VECTOR was not returned by this function and getVelocity() of the other.
	virtual Vector3 collider2D_getNormal(void) const = 0;
	// Function to send Collider2D the velocity of the child. This is for more precise box collisions. Forces CollideWith() to use forward checking collision detection if Vector3::ZERO_VECTOR was not returned by this function and getNormal() of the other.
	virtual Vector3 collider2D_getVelocity(void) const = 0;
};

#endif