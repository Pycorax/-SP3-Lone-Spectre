#include "ObjectiveAssassinate.h"

ObjectiveAssassinate::ObjectiveAssassinate(void) :
m_assassinated(false),
m_tryingToAssassinate(false)
{

}

ObjectiveAssassinate::~ObjectiveAssassinate(void)
{

}

void ObjectiveAssassinate::Init(NPC* _target)
{
	m__target = _target;
}

void ObjectiveAssassinate::Update(double dt)
{
	if (m_tryingToAssassinate)
	{
		m_assassinated = true;
	}
}

bool ObjectiveAssassinate::IsCompleted()
{
	return m_assassinated;
}

void ObjectiveAssassinate::Activate() //If the target has been killed
{
	m_tryingToAssassinate = true;
}

void ObjectiveAssassinate::Reset()
{
	m_assassinated = false;
}

bool ObjectiveAssassinate::Active()
{
	return m_tryingToAssassinate;
}

NPC* ObjectiveAssassinate::GetTarget(void)const
{
	return m__target;
}