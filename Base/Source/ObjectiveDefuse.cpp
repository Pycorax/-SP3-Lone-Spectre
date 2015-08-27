#include "ObjectiveDefuse.h"

ObjectiveDefuse::ObjectiveDefuse(void) :
defused(false)
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

}

bool ObjectiveDefuse::Completed()
{
	return defused == true;
}

void ObjectiveDefuse::Activate() //If the bomb has been defused
{
	defused = true;
}
