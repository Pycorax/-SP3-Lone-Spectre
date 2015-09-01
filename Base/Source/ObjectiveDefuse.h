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
	//maximum time
	float m_TimeNeeded;
	//Time left
	float m_TimeLeftOnBomb;
	//Original Time Left
	float m_TimeLeft;
public: //Functions
	ObjectiveDefuse();
	virtual ~ObjectiveDefuse();

	void Init(float defuseTime, float TimeTillBOOM);
	void Update(double dt);
	void Activate();
	bool Active();
	bool IsCompleted();
	void Reset();

	float GetTimer();
	float GetMexTime(void)const;
	float GetTimeTillBOOM(void) const;
	void ResetBombTimer(void);
};

#endif