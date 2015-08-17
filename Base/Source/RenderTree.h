#ifndef RENDER_TREE_H
#define RENDER_TREE_H

#include <vector>
#include "PhysicsObject.h"

using std::vector;

class RenderTree
{
	protected:
		PhysicsObject geo;
		vector<RenderTree> childTrees;

	public:
		// Constructors & Destructors
		RenderTree(void);
		RenderTree(PhysicsObject _geo);
		~RenderTree(void);

		// Add
		void AddChild(RenderTree childTree);

		// Getters
		PhysicsObject GetGeometry(void);
		vector<RenderTree> GetChildren(void);
};

#endif