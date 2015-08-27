#include "ObjectiveSetBomb.h"

ObjectiveSetBomb::ObjectiveSetBomb(void) :
m_setBomb(false)
, m_setting(false)
, m_setTime(5.f)
{

}

ObjectiveSetBomb::~ObjectiveSetBomb(void)
{

}

void ObjectiveSetBomb::Init()
{

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
	m_setBomb = true;
	//Reset duration time needed to set the bomb
	m_setTime = 5.f;
}

bool ObjectiveSetBomb::Active()
{
	return m_setting;
}
