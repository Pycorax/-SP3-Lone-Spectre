#include "GameObject.h"

GameObject::GameObject(Mesh* mesh, bool collidable /* = true*/) : BasicObject(mesh)
{
	SetCollidable(collidable);
}

GameObject::~GameObject()
{
	// Mesh is not deleted here as a collection of meshes will be stored in a Mesh Library
}

// Setters
void GameObject::SetBoundExt(Vector3 ext)
{
	m_boundExt = ext;
}

void GameObject::SetBoundOffset(Vector3 offset)
{
	m_boundOffset = offset;
}

void GameObject::SetCollidable(bool collide)
{
	m_collidable = collide;
}

// Getters
Vector3 GameObject::GetBoundExt(void)
{
	return m_boundExt;
}

Vector3 GameObject::GetBoundOffset(void)
{
	return m_boundOffset;
}

bool GameObject::IsCollidable(void)
{
	return m_collidable;
}

bool GameObject::CollisionTest(GameObject * go)
{
	if (this->IsCollidable() == false || go->IsCollidable() == false)
	{
		return false;
	}

	// Get Half lengths
	Vector3 v3_thisHalfLength = GetTransform().Scale + GetBoundExt();
	v3_thisHalfLength.x /= 2;
	v3_thisHalfLength.y /= 2;
	v3_thisHalfLength.z /= 2;

	Vector3 v3_otherHalfLength = go->GetTransform().Scale + go->GetBoundExt();
	v3_otherHalfLength.x /= 2;
	v3_otherHalfLength.y /= 2;
	v3_otherHalfLength.z /= 2;

	// Calculate the bounds
	Vector3 v3_thisBoundMin = GetTransform().Translation + GetBoundOffset() - v3_thisHalfLength;
	Vector3 v3_otherBoundMin = go->GetTransform().Translation + go->GetBoundOffset() - v3_otherHalfLength;
	Vector3 v3_thisBoundMax = GetTransform().Translation + GetBoundOffset() + v3_thisHalfLength;
	Vector3 v3_otherBoundMax = go->GetTransform().Translation + go->GetBoundOffset() + v3_otherHalfLength;

	// Case 1: This Object MAX is INSIDE other Object
	if (
		// X-axis
		v3_thisBoundMax.x > v3_otherBoundMin.x && v3_thisBoundMax.x < v3_otherBoundMax.x &&
		v3_thisBoundMax.y > v3_otherBoundMin.y && v3_thisBoundMax.y < v3_otherBoundMax.y &&
		v3_thisBoundMax.z > v3_otherBoundMin.z && v3_thisBoundMax.z < v3_otherBoundMax.z

		)
	{
		return true;
	}

	// Case 2: This Object MIN IS INSIDE other Object
	if (
		// X-axis
		v3_thisBoundMin.x > v3_otherBoundMin.x && v3_thisBoundMin.x < v3_otherBoundMax.x &&
		v3_thisBoundMin.y > v3_otherBoundMin.y && v3_thisBoundMin.y < v3_otherBoundMax.y &&
		v3_thisBoundMin.z > v3_otherBoundMin.z && v3_thisBoundMin.z < v3_otherBoundMax.z

		)
	{
		return true;
	}

	// Case 3: Other Object MAX IS INSIDE this
	if (
		// X-axis
		v3_otherBoundMax.x > v3_thisBoundMin.x && v3_otherBoundMax.x < v3_thisBoundMax.x &&
		v3_otherBoundMax.y > v3_thisBoundMin.y && v3_otherBoundMax.y < v3_thisBoundMax.y &&
		v3_otherBoundMax.z > v3_thisBoundMin.z && v3_otherBoundMax.z < v3_thisBoundMax.z

		)
	{
		return true;
	}

	// Case 4: Other Object MIN IS INSIDE this
	if (
		// X-axis
		v3_otherBoundMax.x > v3_thisBoundMin.x && v3_otherBoundMax.x < v3_thisBoundMax.x &&
		v3_otherBoundMax.y > v3_thisBoundMin.y && v3_otherBoundMax.y < v3_thisBoundMax.y &&
		v3_otherBoundMax.z > v3_thisBoundMin.z && v3_otherBoundMax.z < v3_thisBoundMax.z

		)
	{
		return true;
	}

	return false;
}
