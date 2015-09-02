#include "ObjectiveSetBomb.h"

ObjectiveSetBomb::ObjectiveSetBomb(void) :
m_setBomb(false)
, m_setting(false)
, m_setTime(0)
,m_setTimeOriginal(0)
{

}

ObjectiveSetBomb::~ObjectiveSetBomb(void)
{

}

void ObjectiveSetBomb::Init(float TimeToSetBomb)
{
	m_setTime = m_setTimeOriginal = TimeToSetBomb;
}


void ObjectiveSetBomb::Update(double dt)
{
	if (m_setting)
	{
		m_setTime -= dt;
		//If defuse timer finish
		if (m_setTime <= 0.f)
		{
			m_setBomb = true;
		}
	}
}

bool ObjectiveSetBomb::IsCompleted()
{
	return m_setBomb;
}

void ObjectiveSetBomb::Activate() //If the bomb has been set
{
	m_setting = true;
	m_setTime = m_setTimeOriginal;
}

bool ObjectiveSetBomb::Active()
{
	return m_setting;
}

float ObjectiveSetBomb::GetTimer()
{
	return m_setTime;
}

void ObjectiveSetBomb::Reset()
{
	m_setting = false;
	m_setBomb = false;
}

void ObjectiveSetBomb::ResetActive()
{
	m_setting = false;
}

void ObjectiveSetBomb::ResetTimer()
{
	m_setTime = m_setTimeOriginal;
}

float ObjectiveSetBomb::GetSetMaxTime()const
{
	return m_setTimeOriginal;
}