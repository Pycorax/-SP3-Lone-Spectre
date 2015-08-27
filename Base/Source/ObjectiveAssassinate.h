#ifndef OBJECTIVE_ASSASSINATE_H
#define OBJECTIVE_ASSASSINATE_H

#include "Objective.h"

class ObjectiveAssassinate : public Objective
{
private: //Variables
	bool assassinated;
public: //Functions
	ObjectiveAssassinate();
	virtual ~ObjectiveAssassinate();

	void Init();
	void Update(double dt);
	void Activate();
	bool IsCompleted();
};

#endif