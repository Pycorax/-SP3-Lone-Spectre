#include "ObjectiveHostage.h"

ObjectiveHostage::ObjectiveHostage(void) :
saved(false),
m__target(NULL)
{

}

ObjectiveHostage::~ObjectiveHostage(void)
{

}

void ObjectiveHostage::Init(NPC* _target)
{
	m__target = _target;
}

void ObjectiveHostage::Update(double dt)
{
}

bool ObjectiveHostage::IsCompleted()
{
	return saved;
}

 //If the target has been saved
void ObjectiveHostage::Activate()
{
	saved = true;
}

void ObjectiveHostage::Reset()
{
	saved = false;
}

bool ObjectiveHostage::Active()
{
	return saved;
}