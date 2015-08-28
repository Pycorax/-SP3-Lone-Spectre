#include "ObjectiveCollect.h"

ObjectiveCollect::ObjectiveCollect(void):
totalObjects(1)
{
	
}

ObjectiveCollect::~ObjectiveCollect(void)
{

}

void ObjectiveCollect::Init()
{

}

void ObjectiveCollect::Update(double dt)
{

}

bool ObjectiveCollect::IsCompleted()
{
	return totalObjects == 0;
}

void ObjectiveCollect::Reset()
{
	//use the init
	totalObjects = 1;
}

void ObjectiveCollect::Activate() //If the objective has been taken
{
	totalObjects -= 1;
}

bool ObjectiveCollect::Active()
{
	return totalObjects == 0;
}