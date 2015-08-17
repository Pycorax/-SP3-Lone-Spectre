#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(Mesh* mesh)
	: GameObject(mesh)
	, m_type(PHY_BALL)
	, m_bMortal(false)
{
	SetActive(false);
}

PhysicsObject::~PhysicsObject(void)
{

}

/*
*
	\brief
	For initializing normal balls.
*/
void PhysicsObject::Init(Vector3 position, Vector3 velocity, Vector3 scale) 
{
	init(position, velocity, scale);
	SetType(PHY_BALL);
}

/*
 * 
	\brief
	For initializing walls.
 */
void PhysicsObject::Init(Vector3 position, Vector3 velocity, Vector3 scale, Vector3 normal)
{
	init(position, velocity, scale);
	SetNormal(normal);
	SetType(PHY_WALL_2D);
}

void PhysicsObject::Update(double dt)
{
	// Check if object is active
	if (IsActive())
	{
		// Update position
		m_transforms.Translation += m_vel * static_cast<float>(dt);

		// Decrease lifetime
		m_lifetime -= dt;

		// Kill if it is to be killed
		if (m_bMortal && m_lifetime < Math::EPSILON)
		{
			m_bActive = false;
		}
	}
}

bool PhysicsObject::IsActive(void) const
{
	return m_bActive;
}

void PhysicsObject::SetActive(const bool active)
{
	m_bActive = active;
}

Vector3 PhysicsObject::GetVelocity(void) const
{
	return m_vel;
}

PhysicsObject::PHYSICS_TYPE PhysicsObject::GetType() const
{
	return m_type;
}

Vector3 PhysicsObject::GetNormal() const
{
	return m_normal;
}

float PhysicsObject::GetMass(void) const
{
	return m_mass;
}

float PhysicsObject::GetLifetime(void) const
{
	return m_lifetime;
}

void PhysicsObject::SetVelocity(const Vector3 speed)
{
	m_vel = speed;
}

void PhysicsObject::SetType(const PHYSICS_TYPE type)
{
	m_type = type;
}

void PhysicsObject::SetNormal(const Vector3 normal)
{
	m_normal = normal;

	if (m_normal != Vector3())
	{
		m_normal.Normalize();

		// Set the rotation according to the normal
		m_transforms.Rotation.z = Math::RadianToDegree(atan2(m_normal.y, m_normal.x));
	}
}

void PhysicsObject::SetMass(float mass)
{
	if (mass > Math::EPSILON)
	{
		m_mass = mass;
	}
}

void PhysicsObject::SetLifetime(const float lifetime)
{
	m_lifetime = lifetime;

	if (lifetime < Math::EPSILON)
	{
		m_bMortal = false;
	}
	else
	{
		m_bMortal = true;
	}
}

bool PhysicsObject::CheckCollision(PhysicsObject * po, double dt)
{
	if (m_bActive && po->m_bActive)
	{
		Vector3 deltaVel = m_vel - po->m_vel;
		Vector3 deltaPos = po->m_transforms.Translation - m_transforms.Translation;

		switch (po->m_type)
		{
			case PHY_BALL:
			{
				if (deltaVel.Dot(deltaPos) > 0)
				{
					//return false;
				}

				//Exercise 1: move collision code to CheckCollision()
				float distSquared = ((m_transforms.Translation + m_vel * dt) - (po->m_transforms.Translation + po->m_vel * dt)).LengthSquared();
				float combinedRadius = m_transforms.Scale.x + po->m_transforms.Scale.x;

				//Practical 4, Exercise 13: improve collision detection algorithm
				if (distSquared <= combinedRadius * combinedRadius)
				{
					return true;
				}
			}
			break;

			case PHY_WALL_2D:
			{
				// For Thickness
				// |(w0 - b1).N| < r + h / 2
				Vector3 w0 = po->m_transforms.Translation;
				Vector3 b1 = m_transforms.Translation + m_vel * dt;
				Vector3 N = po->m_normal;
				float r = m_transforms.Scale.x;
				float h = po->m_transforms.Scale.x;

				// For Length
				// |(w0 - b1).NP| < r + l / 2
				Vector3 NP(-N.y, N.x);
				float l = po->m_transforms.Scale.y;

				return abs((w0 - b1).Dot(N)) < r + h * 0.5 && abs((w0 - b1).Dot(NP)) < r + l * 0.5;
			}
			break;
		}
	}

	return false;
}

void PhysicsObject::RespondCollision(PhysicsObject* po)
{
	switch (po->m_type)
	{
		case PHY_BALL:
		{
			if (m_type != PHY_BALL)
			{
				return;
			}

			float m1 = m_mass;
			float m2 = po->m_mass;
			Vector3 u1 = m_vel;
			Vector3 u2 = po->m_vel;

			/*
			initialMomentum = m1 * u1 + m2 * u2;

			Vector3 momentum1 = m_mass * m_vel;
			Vector3 momentum2 = po->m_mass * po->m_vel;

			m_vel = momentum2 * (1.f / m_mass);
			po->m_vel = momentum1 * (1.f / po->m_mass);
			*/

			// Conservation of Energy Based Collision
			//v1 = (2 * m1 * u1 + u2 * (m2 - m1)) * (1 / (m1 + m2));
			//v2 = (2 * m2 * u2 + u1 * (m1 - m2)) * (1 / (m1 + m2));

			bool skate3 = false;

			/*
			* Skate 3 Physics Code
			*/
			Vector3 tangent = (m_transforms.Translation - po->m_transforms.Translation).Normalized();
			Vector3 u1N = u1.Dot(tangent) * tangent;
			Vector3 u2N = u2.Dot(tangent) * tangent;

			if (!skate3)
			{
				/*
				* Proper Physics Code
				*/
				tangent = (po->m_transforms.Translation - m_transforms.Translation).Normalized();
				u1N = u1.Dot(tangent) / u1.LengthSquared() * tangent;
				u2N = u2.Dot(-tangent) / u2.LengthSquared() * -tangent;
			}


			Vector3 v1 = u1 + ((2 * m2) / (m1 + m2)) * (u2N - u1N);
			Vector3 v2 = u2 + ((2 * m1) / (m1 + m2)) * (u1N - u2N);

			m_vel = v2;
			po->m_vel = v1;
		}
		break;

		case PHY_WALL_2D:
		{
			if (m_type != PHY_BALL)
			{
				return;
			}

			// For Thickness
			// |(w0 - b1).N| < r + h / 2
			Vector3 w0 = po->m_transforms.Translation;
			Vector3 b1 = m_transforms.Translation;
			Vector3 N = po->m_normal;
			float r = m_transforms.Scale.x;
			float h = po->m_transforms.Scale.x;

			// For Length
			// |(w0 - b1).NP| < r + l / 2
			Vector3 NP(-N.y, N.x);
			float l = po->m_transforms.Scale.y;

			if (abs((w0 - b1).Dot(N)) > r + h * 0.5)
			{
				// v = u - (2u.N)N
				Vector3 u = m_vel;
				Vector3 v = u - 2 * u.Dot(N) * N;
				m_vel = v;
			}
			else if (abs((w0 - b1).Dot(NP)) > r + l * 0.5)
			{
				// v = u - (2u.NP)NP
				Vector3 u = m_vel;
				Vector3 v = u - 2 * u.Dot(NP) * NP;
				m_vel = v;
			}
		}
		break;
	}
}

void PhysicsObject::init(Vector3 position, Vector3 velocity, Vector3 scale)
{
	SetActive(true);
	SetPos(position);
	SetVelocity(velocity);
	SetScale(scale);
}

Vector3 PhysicsObject::calcMomentum()
{
	return m_mass * m_vel;
}