#include "ObjectiveSetBomb.h"

ObjectiveSetBomb::ObjectiveSetBomb(void) :
setBomb(false)
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

}

bool ObjectiveSetBomb::IsCompleted()
{
	return setBomb == true;
}

void ObjectiveSetBomb::Activate() //If the bomb has been set
{
	setBomb = true;
}
