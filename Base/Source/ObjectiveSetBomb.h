#ifndef OBJECTIVE_SETBOMB_H
#define OBJECTIVE_SETBOMB_H

#include "Objective.h"

class ObjectiveSetBomb : public Objective
{
private: //Variables
	bool setBomb;
public: //Functions
	ObjectiveSetBomb();
	virtual ~ObjectiveSetBomb();

	void Init();
	void Update(double dt);
	void Activate();
	bool IsCompleted();
};

#endif