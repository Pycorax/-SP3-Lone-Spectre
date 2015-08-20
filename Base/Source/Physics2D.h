#ifndef PHYSICS_2D_H
#define PHYSICS_2D_H

#include "Vector2.h"
#include "GameObject2D.h"

class Physics2D
{
protected:	// Variables
	Vector2 m_normal;			// The normal of this object. If a normal is provided, this object is considered a box
	Vector2 m_velocity;			// The velocity of this object in world unit per second
	float m_mass;				// The mass of this object. Affects collision response.
	bool m_kinematic;			// If true, this object will not be affected by collision response (e.g. wall, floor, pillar)

public:		// Functions
	Physics2D(void);
	virtual ~Physics2D(void);

	void InitPhysics2D(float mass, bool kinematic, Vector2 vel = Vector2::ZERO_VECTOR, Vector2 normal = Vector2::ZERO_VECTOR);

	/*
	 * Setter Functions
	 */
	void SetNormal(Vector2 normal);
	void SetVelocity(Vector2 velocity);
	void SetMass(float mass);
	void SetKinematic(bool kinematic);

	/*
	 * Getter Functions
	 */
	Vector2 GetNormal(void);
	Vector2 GetVelocity(void);
	float GetMass(void);
	bool GetKinematic(void);

	/*
	 * Physics Functions
	 */
	virtual void AddForce(const Vector2 FORCE, const double DT);		// Moves the object by adding a force to the object
	virtual void UpdatePhysics(const double DT);						// Updates the object with it's velocity
	virtual void CollideRespondTo(Physics2D* _other);					// Responds to collision

private:	// Functions
	/*
	 *	Abstract Functions
	 *	These functions need to be defined in child classes and serves as the interface between child classes and this class.
	 */
	// Function to send Physics2D the actual transforms of the child.
	virtual Transform physics2D_getTransforms() const = 0;
	// Function to set the actual transforms of the child from Physics2D.
	virtual void physics2D_setTransforms(Transform t) = 0;
};

#endif