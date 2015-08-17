/******************************************************************************/
/*!
\file	PhysicsObject.h
\author Tng Kah Wei
\brief
Class to work with Objects and their Physical properties
*/
/******************************************************************************/
#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include "GameObject.h"

#include "Vector3.h"

/******************************************************************************/
/*!
		Class PhysicsObject:
\brief	Class to work with Objects and their Physical properties
*/
/******************************************************************************/
class PhysicsObject : public GameObject
{
	public:
		enum PHYSICS_TYPE
		{
			PHY_QUAD_PARTICLE,
			PHY_BALL,
			PHY_WALL_2D,
			PHY_HOLE,
			NUM_PHYSICS_TYPE
		};

		PhysicsObject(Mesh* mesh = NULL);
		virtual ~PhysicsObject(void);

		virtual void Init(Vector3 position, Vector3 velocity, Vector3 scale);				// For balls
		void Init(Vector3 position, Vector3 velocity, Vector3 scale, Vector3 normal);		// For walls
		virtual void Update(double dt);

		virtual bool IsActive(void) const;
		Vector3 GetVelocity(void) const;
		PHYSICS_TYPE GetType(void) const;
		Vector3 GetNormal(void) const;
		float GetMass(void) const;
		float GetLifetime(void) const;

		void SetActive(const bool active);		
		void SetVelocity(const Vector3 speed);
		void SetType(const PHYSICS_TYPE type);
		void SetNormal(const Vector3 normal);
		void SetMass(float mass);
		void SetLifetime(const float lifetime);		// Set lifetime to a negative value to disable death on end of lifetime

		bool CheckCollision(PhysicsObject* po, double dt);
		void RespondCollision(PhysicsObject* po);

	private:
		PHYSICS_TYPE m_type;
		Vector3 m_vel;
		Vector3 m_normal;
		bool m_bActive;
		float m_mass;
		float m_lifetime;
		bool m_bMortal;			// Controls if it is killed at the end of it's lifetime

		void init(Vector3 position, Vector3 velocity, Vector3 scale);
		Vector3 calcMomentum(void);
		
};

#endif