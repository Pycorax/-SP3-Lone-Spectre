#include "GameObject2D.h"


GameObject2D::GameObject2D(void) 
	: m__mesh(NULL)
	, m_meshSpriteID(-1)
{

}

GameObject2D::~GameObject2D(void)
{
}

void GameObject2D::SetPos(const Vector3 pos)
{
	m_transforms.Translation = pos;
}

void GameObject2D::SetRot(const Vector3 rot)
{
	m_transforms.Rotation = rot;
}

void GameObject2D::SetScale(const Vector3 scale)
{
	m_transforms.Scale = scale;
}

void GameObject2D::SetMesh(Mesh * _mesh)
{
	m__mesh = _mesh;
}

void GameObject2D::SetActive(bool active)
{
	m_active = active;
}

void GameObject2D::SetMeshSpriteID(int id)
{
	m_meshSpriteID = id;
}

Transform GameObject2D::GetTransform(void) const
{
	return m_transforms;
}

Mesh* GameObject2D::GetMesh(void) const
{
	return m__mesh;
}

bool GameObject2D::GetActive(void) const
{
	return m_active;
}

int GameObject2D::GetMeshSpriteID(void) const
{
	return m_meshSpriteID;
}
