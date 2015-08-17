#include "RenderTree.h"

// Constructors & Destructors
RenderTree::RenderTree(void)
{

}

RenderTree::RenderTree(PhysicsObject _geo)
{
	geo = _geo;
}

RenderTree::~RenderTree(void)
{

}

// Add
void RenderTree::AddChild(RenderTree childTree)
{
	childTrees.push_back(childTree);
}

// Getters
PhysicsObject RenderTree::GetGeometry(void)
{
	return geo;
}

vector<RenderTree> RenderTree::GetChildren(void)
{
	return childTrees;
}