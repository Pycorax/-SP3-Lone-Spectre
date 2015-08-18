/*
#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter()
	: m_maxActive(10)
	, m_baseParticle(NULL)
{
}


ParticleEmitter::~ParticleEmitter()
{
	// Delete all particles3
	while (m_particles.size() > 0)
	{
		delete m_particles.back();
		m_particles.pop_back();
	}
}

void ParticleEmitter::Init(Vector3 pos, PhysicsObject * baseParticle, Vector3 minVel, Vector3 maxVel, unsigned maxParticles)
{
	m_pos = pos;
	m_baseParticle = baseParticle;
	m_minVel = minVel;
	m_maxVel = maxVel;
	m_maxActive = maxParticles;
}

void ParticleEmitter::Update(double dt)
{
	int activeParticles = 0;

	// Update current particles
	for (vector<PhysicsObject*>::iterator it = m_particles.begin(); it != m_particles.end(); ++it)
	{
		(*it)->Update(dt);

		if ((*it)->IsActive())
		{
			// Apply Diff Transforms
			Transform transforms = (*it)->GetTransform();
			transforms.Rotation += diff.Rotation * dt;
			transforms.Scale += diff.Scale * dt;

			transforms.Scale.x = Math::Clamp(transforms.Scale.x, 0.001f, transforms.Scale.x);
			transforms.Scale.y = Math::Clamp(transforms.Scale.y, 0.001f, transforms.Scale.y);
			transforms.Scale.z = Math::Clamp(transforms.Scale.z, 0.001f, transforms.Scale.z);

			(*it)->SetRot(transforms.Rotation);
			(*it)->SetScale(transforms.Scale);

			++activeParticles;
		}
	}

	// Delay the spawning of particles
	static const double SPAWN_DELAY = m_baseParticle->GetLifetime() * 0.05;
	static double timer = 0;
	timer -= dt;

	if (timer > Math::EPSILON)
	{
		return;
	}
	else
	{
		timer = SPAWN_DELAY;
	}

	// Spawn more particles if there is still space
	if(m_maxActive - activeParticles > 0)
	{
		spawnParticle(dt);
	}
}

void ParticleEmitter::SetDiff(Transform transform)
{
	diff = transform;
}

const vector<PhysicsObject*> ParticleEmitter::GetParticles(void)
{
	return m_particles;
}

void ParticleEmitter::spawnParticle(double dt)
{
	// Look for a particle
	for (vector<PhysicsObject*>::iterator it = m_particles.begin(); it != m_particles.end(); ++it)
	{
		if ((*it)->IsActive() == false)
		{
			initParticle(*it);

			return;
		}
	}

	// If no particles are available, make more to store
	const int MAX_BATCH_BUILD = 10;
	PhysicsObject* ptr;
	for (int i = 0; i < MAX_BATCH_BUILD; ++i)
	{
		ptr = new PhysicsObject();
		initParticle(ptr);
		m_particles.push_back(ptr);
	}

	// Activate one particle
	m_particles.back()->SetActive(true);
}

void ParticleEmitter::initParticle(PhysicsObject * particle)
{
	// Set the properties of this particle to the template
	*particle = *m_baseParticle;

	// Spawn this dude at the emitter
	particle->SetPos(m_pos);

	// Randomize the velocity
	Vector3 vel(Math::RandFloatMinMax(m_minVel.x, m_maxVel.x), Math::RandFloatMinMax(m_minVel.y, m_maxVel.y), Math::RandFloatMinMax(m_minVel.z, m_maxVel.z));
	particle->SetVelocity(vel);
}

*/