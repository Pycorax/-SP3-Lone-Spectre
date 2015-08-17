#ifndef MVC_MODEL_SHADOW_H
#define MVC_MODEL_SHADOW_H

#include "MVC_Model_3D.h"

#include "DepthFBO.h"

class MVC_Model_Shadow : public MVC_Model_3D
{
	public:
		GameObject* m_lightDepthQuad;

		MVC_Model_Shadow(string configSONFile);
		~MVC_Model_Shadow();

		void Init(void);
		void Update(double dt);
		void Exit(void);

	private:
		// Input
		void processKeyAction(double dt);
};

#endif