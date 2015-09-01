#ifndef OBJECTIVE_ESCAPE_H
#define OBJECTIVE_ESCAPE_H

// Inheritance Includes
#include "Objective.h"

class ObjectiveEscape : public Objective
{
public:
	ObjectiveEscape();
	virtual ~ObjectiveEscape();

	virtual bool IsCompleted();
	virtual bool Active();
	virtual void Activate();
	virtual void Reset();
};

#endif