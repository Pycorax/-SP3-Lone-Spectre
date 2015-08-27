#ifndef OBJECTIVE_DEFUSE_H
#define OBJECTIVE_DEFUSE_H

#include "Objective.h"

class ObjectiveDefuse : public Objective
{
private: //Variables
	bool defused;
public: //Functions
	ObjectiveDefuse();
	virtual ~ObjectiveDefuse();

	void Init();
	void Update(double dt);
	void Activate();
	bool Completed();
};

#endif