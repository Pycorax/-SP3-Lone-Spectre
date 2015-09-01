#include "ObjectiveDefuse.h"


ObjectiveDefuse::ObjectiveDefuse(void) :
	m_defused(false),
	m_defusing(false),
	m_defuseTime(0),
	m_TimeNeeded(0),
	m_TimeLeftOnBomb(0)
{

}

ObjectiveDefuse::~ObjectiveDefuse(void)
{

}

void ObjectiveDefuse::Init(float defuseTime, float TimeTillBOOM)
{
	m_defuseTime = m_TimeNeeded = defuseTime;
	m_TimeLeftOnBomb = m_TimeLeft = TimeTillBOOM;
}

void ObjectiveDefuse::Update(double dt)
{
	if(m_defusing)
	{
		m_defuseTime -= dt;
		//if defuse timer finish
		if(m_defuseTime <= 0.f)
		{
			m_defused = true;
		}
	}
	m_TimeLeftOnBomb -= dt;
}

//if complete return true;
bool ObjectiveDefuse::IsCompleted()
{
	return m_defused;
}

void ObjectiveDefuse::Activate() //If the bomb has been defused
{
	m_defusing = true;
	//reset defuse time needed for the bomb
	m_defuseTime = m_TimeNeeded;
}

bool ObjectiveDefuse::Active()
{
	return m_defusing;
}

float ObjectiveDefuse::GetTimer()
{
	return m_defuseTime;
}

void ObjectiveDefuse::Reset()
{
	m_defusing = false;
	m_defused = false;
	m_defuseTime = m_TimeNeeded;
}
float ObjectiveDefuse::GetMexTime(void)const
{
	return m_TimeNeeded;
}

float ObjectiveDefuse::GetTimeTillBOOM(void)const
{
	return m_TimeLeftOnBomb;
}

void ObjectiveDefuse::ResetBombTimer(void)
{
	m_TimeLeftOnBomb = m_TimeLeft;
}