#ifndef OBJECTIVE_COLLECT_H
#define OBJECTIVE_COLLECT_H

#include "Objective.h"

class ObjectiveCollect : public Objective
{
private: //Variables
	int m_totalObjects;
public: //Functions
	ObjectiveCollect();
	virtual ~ObjectiveCollect();

	void Init(int numDocuments);
	void Update(double dt);
	void Activate();

	bool Active();
	void Reset();

	bool IsCompleted();
};

#endif