#ifndef OBJECTIVE_ASSASSINATE_H
#define OBJECTIVE_ASSASSINATE_H

#include "Objective.h"
#include "NPC.h"

class ObjectiveAssassinate : public Objective
{
private: //Variables
	bool assassinated;

	//NPC Pointer
	NPC* m__target;

public: //Functions
	ObjectiveAssassinate();
	virtual ~ObjectiveAssassinate();

	void Init();
	void Update(double dt);
	void Activate();
	bool Active();
	void Reset();
	bool IsCompleted();
};

#endif