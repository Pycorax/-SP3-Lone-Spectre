#include "ObjectiveCollect.h"

ObjectiveCollect::ObjectiveCollect(void):
totalObjects(0)
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

bool ObjectiveCollect::Completed()
{
	return totalObjects == 0;
}

void ObjectiveCollect::Activate() //If the objective has been taken
{
	totalObjects -= 1;
}