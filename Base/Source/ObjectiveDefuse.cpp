#include "ObjectiveDefuse.h"

ObjectiveDefuse::ObjectiveDefuse(void) :
	m_defused(false),
	m_defusing(false),
	m_defuseTime(5.f)
{

}

ObjectiveDefuse::~ObjectiveDefuse(void)
{

}

void ObjectiveDefuse::Init()
{

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
	m_defuseTime = 5.f;
}

bool ObjectiveDefuse::Active()
{
	return m_defusing;
}
