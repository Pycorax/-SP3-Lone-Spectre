#ifndef OBJECTIVE_DEFUSE_H
#define OBJECTIVE_DEFUSE_H

#include "Objective.h"

class ObjectiveDefuse : public Objective
{
private: //Variables
	//if complete
	bool m_defused;
	//if defusing
	bool m_defusing;
	//defuse timer
	float m_defuseTime;
public: //Variable
	static const float S_M_MAX_DEFUSE_TIME;
public: //Functions
	ObjectiveDefuse();
	virtual ~ObjectiveDefuse();

	void Init();
	void Update(double dt);
	void Activate();
	bool Active();
	bool IsCompleted();
	void Reset();

	float GetTimer();
};

#endif