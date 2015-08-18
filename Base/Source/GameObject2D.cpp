#include "GameObject2D.h"


GameObject2D::GameObject2D(void)
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

Transform GameObject2D::GetTransform(void) const
{
	return m_transforms;
}

Mesh* GameObject2D::GetMesh(void) const
{
	return m__mesh;
}