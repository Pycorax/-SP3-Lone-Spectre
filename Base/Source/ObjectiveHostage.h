#ifndef OBJECTIVE_HOSTAGE_H
#define OBJECTIVE_HOSTAGE_H

#include "Objective.h"
#include "NPC.h"
#include "TileMap.h"

class ObjectiveHostage : public Objective
{
private: //Variables
	bool saved;

public: //Functions
	ObjectiveHostage();
	virtual ~ObjectiveHostage();

	void Init(NPC* _target);
	void Update(double dt);
	void Activate();
	bool Active();
	void Reset();
	bool IsCompleted();

	//points to target hostage
	NPC* m__target;
};

#endif