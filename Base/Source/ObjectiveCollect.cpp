#include "ObjectiveCollect.h"

ObjectiveCollect::ObjectiveCollect(void)
	: m_totalObjects(0)
{
	
}

ObjectiveCollect::~ObjectiveCollect(void)
{

}

void ObjectiveCollect::Init(int numDocuments)
{
	// Error Checking
	if (numDocuments < 0)
	{
		return;
	}

	m_totalObjects = numDocuments;
}

void ObjectiveCollect::Update(double dt)
{

}

bool ObjectiveCollect::IsCompleted()
{
	return m_totalObjects == 0;
}

void ObjectiveCollect::Reset()
{
	//use the init
	m_totalObjects = 1;
}

void ObjectiveCollect::Activate() //If the objective has been taken
{
	m_totalObjects -= 1;
}

bool ObjectiveCollect::Active()
{
	return m_totalObjects == 0;
}