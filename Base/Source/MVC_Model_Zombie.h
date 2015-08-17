#ifndef MVC_MODEL_ZOMBIE_H
#define MVC_MODEL_ZOMBIE_H

#include "MVC_Model_3D.h"

#include "ParticleEmitter.h"

class MVC_Model_Zombie : public MVC_Model_3D
{
	ParticleEmitter m_smoker;
	GameObject* m_car;

	public:
		MVC_Model_Zombie(string configSONFile);
		~MVC_Model_Zombie();

		void Init(void);
		void Update(double dt);
		void Exit(void);

	private:
		// Input
		void processKeyAction(double dt);
};

#endif