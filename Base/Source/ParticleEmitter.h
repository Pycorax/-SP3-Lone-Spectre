// TODO: Particle Class : GameObject3D, Physics2D
/*
#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include <vector>

using std::vector;


class ParticleEmitter
{
	protected:
		// Positioning
		Vector3 m_pos;

		// Behavior Modifiers
		unsigned m_maxActive;						// The maximum number of particles that can be active
		Transform diff;

		// Assets
		PhysicsObject* m_baseParticle;				// A template of the particle that this emitter will emit
		vector<PhysicsObject*> m_particles;			// All the particles that will be emitted	
		
		// Action
		Vector3 m_minVel;							// The minimum velocity of a particle
		Vector3 m_maxVel;							// The maximum velocity of a particle

	public:
		ParticleEmitter();
		virtual ~ParticleEmitter();

		virtual void Init(Vector3 pos, PhysicsObject* baseParticle, Vector3 minVel, Vector3 maxVel, unsigned maxParticles = 10);
		virtual void Update(double dt);

		void SetDiff(Transform transform);

		const vector<PhysicsObject*> GetParticles(void);

	protected:
		void spawnParticle(double dt);
		void initParticle(PhysicsObject* particle);
};

#endif
*/