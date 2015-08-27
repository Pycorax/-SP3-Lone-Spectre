#ifndef OBJECTIVE_COLLECT_H
#define OBJECTIVE_COLLECT_H

#include "Objective.h"

class ObjectiveCollect : public Objective
{
private: //Variables
	int totalObjects;
public: //Functions
	ObjectiveCollect();
	virtual ~ObjectiveCollect();

	void Init();
	void Update(double dt);
	void Activate();	
	bool Active();
	void Reset();
	bool IsCompleted();
};

#endif