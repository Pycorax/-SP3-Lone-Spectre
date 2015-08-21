#include "Physics2D.h"


Physics2D::Physics2D(void)
{
}


Physics2D::~Physics2D(void)
{
}

void Physics2D::InitPhysics2D(float mass, bool kinematic, Vector2 vel, Vector2 normal)
{
	if (mass < Math::EPSILON)
	{
		throw std::invalid_argument("Mass cannot be negative or zero!");
	}

	m_mass = mass;
	m_kinematic = kinematic;
	m_velocity = vel;
	SetNormal(normal);
}

/*
 * Setter Functions
 */
void Physics2D::SetNormal(Vector2 normal)
{
	m_normal = normal;

	if (m_normal != Vector3::ZERO_VECTOR)
	{
		m_normal.Normalize();

		// Set the rotation according to the normal
		Transform t = physics2D_getTransforms();
		t.Rotation.z = Math::RadianToDegree(atan2(m_normal.y, m_normal.x));
		physics2D_setTransforms(t);
	}
}

void Physics2D::SetVelocity(Vector2 velocity)
{
	m_velocity = velocity;
}

void Physics2D::SetMass(float mass)
{
	m_mass = mass;
}

void Physics2D::SetKinematic(bool kinematic)
{
	m_kinematic = kinematic;
}

/*
 * Getter Functions
 */
Vector2 Physics2D::GetNormal(void)
{
	return m_normal;
}

Vector2 Physics2D::GetVelocity(void)
{
	return m_velocity;
}

float Physics2D::GetMass(void)
{
	return m_mass;
}

bool Physics2D::GetKinematic(void)
{
	return m_kinematic;
}

/*
 * Physics Functions
 */
void Physics2D::AddForce(const Vector2 FORCE, const double DT)
{
	// F = ma; a = F/m
	Vector2 acceleration = FORCE * (1.0f / m_mass);

	// v = u + a
	m_velocity += acceleration * static_cast<float>(DT);
}

void Physics2D::UpdatePhysics(const double DT)
{
	// Get the transforms
	Transform t = physics2D_getTransforms();

	// Update position according to the velocity
	t.Translation += m_velocity * static_cast<float>(DT);

	// Set the transforms to the new transforms
	physics2D_setTransforms(t);
}

void Physics2D::CollideRespondTo(Physics2D* _other)
{	
	// Collision: This Ball with Other Wall
	if (_other->m_normal != Vector2::ZERO_VECTOR && _other->m_kinematic)
	{
		/*
		Transform t = physics2D_getTransforms();
		Transform ot = _other->physics2D_getTransforms();

		// For Thickness
		// |(w0 - b1).N| < r + h / 2
		Vector3 w0 = ot.Translation;
		Vector3 b1 = t.Translation;
		Vector3 N = _other->m_normal;
		float r = t.Scale.x;
		float h = ot.Scale.x;

		// For Length
		// |(w0 - b1).NP| < r + l / 2
		Vector3 NP(-N.y, N.x);
		float l = ot.Scale.y;

		if (abs((w0 - b1).Dot(N)) < r + h * 0.5)
		{
			// v = u - (2u.N)N
			Vector3 u = m_velocity;
			Vector3 v = u - 2 * u.Dot(N) * N;
			m_velocity = v;
		}
		else if (abs((w0 - b1).Dot(NP)) < r + l * 0.5)
		{
			// v = u - (2u.NP)NP
			Vector3 u = m_velocity;
			Vector3 v = u - 2 * u.Dot(NP) * NP;
			m_velocity = v;
		}
		*/

		Physics2D* ball = NULL;
		Physics2D* wall = NULL;

		// Assign the ball and wall correctly
		if (m_normal != Vector2::ZERO_VECTOR && _other->m_normal == Vector2::ZERO_VECTOR)
		{
			ball = _other;
			wall = this;
		}
		else if (m_normal == Vector2::ZERO_VECTOR && _other->m_normal != Vector2::ZERO_VECTOR)
		{
			ball = this;
			wall = _other;
		}

		Transform wallT = wall->physics2D_getTransforms();
		Transform ballT = ball->physics2D_getTransforms();

		Vector3 w0 = wallT.Translation;
		Vector3 b1 = ballT.Translation;
		Vector3 N = wall->m_normal;
		float r = ballT.Scale.x * 0.5f; // Half of ball diameter (Radius)
		float h = wallT.Scale.x; // Depth of wall
		float l = wallT.Scale.y; // Length of wall
		Vector3 NP(-N.y, N.x, 0);
		if (abs((w0 - b1).Dot(N)) < r + (h * 0.5f))
		{
			// v = u - (2u.N)N
			Vector3 u = ball->m_velocity;
			Vector3 N = wall->m_normal;
			Vector3 v = u - 2 * u.Dot(N) * N;
			ball->m_velocity = v;
		}
		if (abs((w0 - b1).Dot(NP)) < r + (l * 0.5f))
		{
			// v = u - (2u.N)N
			Vector3 u = ball->m_velocity;
			Vector3 N = wall->m_normal;
			Vector3 v = u - 2 * u.Dot(NP) * NP;
			ball->m_velocity = v;
		}

		return;
	}

	if (_other->m_kinematic)	// Collision: This Ball with Other Pillar
	{
		Vector3 tangent = (_other->physics2D_getTransforms().Translation - physics2D_getTransforms().Translation);
		if (tangent != Vector3::ZERO_VECTOR)
		{
			tangent.Normalize();
		}
		m_velocity = m_velocity - ((2 * m_velocity).Dot(tangent)) * tangent;
	}
	else						// Collision: This Ball with Other Ball
	{
		Vector2 u1 = m_velocity;
		Vector2 u2 = _other->m_velocity;
		Vector2 deltaVel = u2 - u1;

		if (deltaVel != Vector2::ZERO_VECTOR)
		{
			deltaVel.Normalize();
		}

		// Prevent internal collision
		if (deltaVel.LengthSquared() <= -Math::EPSILON)
		{
			return;
		}

		Vector2 tangent = (physics2D_getTransforms().Translation - _other->physics2D_getTransforms().Translation);

		if (tangent != Vector2::ZERO_VECTOR)
		{
			tangent.Normalize();
		}

		Vector2 u1N = u1.Dot(tangent) * tangent;
		Vector2 u2N = u2.Dot(tangent) * tangent;

		m_velocity = u1 + (2 * _other->m_mass) / (m_mass + _other->m_mass) * (u2N - u1N);
		_other->m_velocity = u2 + (2 * m_mass) / (m_mass + _other->m_mass) * (u1N - u2N);
	}
}