#include "ObjectiveAssassinate.h"

ObjectiveAssassinate::ObjectiveAssassinate(void) :
assassinated(false)
{

}

ObjectiveAssassinate::~ObjectiveAssassinate(void)
{

}

void ObjectiveAssassinate::Init()
{

}

void ObjectiveAssassinate::Update(double dt)
{

}

bool ObjectiveAssassinate::IsCompleted()
{
	return assassinated == true;
}

void ObjectiveAssassinate::Activate() //If the target has been killed
{
	assassinated = true;
}
