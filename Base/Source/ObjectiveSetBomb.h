#ifndef OBJECTIVE_SETBOMB_H
#define OBJECTIVE_SETBOMB_H

#include "Objective.h"

class ObjectiveSetBomb : public Objective
{
private: //Variables
	bool m_setBomb;
	bool m_setting;
	//Time set to plant bomb
	float m_setTime;
	// 
	float m_setTimeOriginal;
public: //Functions
	ObjectiveSetBomb();
	virtual ~ObjectiveSetBomb();

	void Init(float TimeToSetBomb);
	void Update(double dt);
	void Activate();
	bool Active();
	bool IsCompleted();
	void Reset();

	float GetTimer();
	void ResetActive();
	void ResetTimer();
	float GetSetMaxTime()const;
};

#endif