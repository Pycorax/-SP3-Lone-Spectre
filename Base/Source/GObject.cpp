#include "GObject.h"


GObject::GObject(void)
{

}

GObject::~GObject(void)
{
}

void GObject::SetPos(const Vector3 pos)
{
	m_transforms.Translation = pos;
}

void GObject::SetRot(const Vector3 rot)
{
	m_transforms.Rotation = rot;
}

void GObject::SetScale(const Vector3 scale)
{
	m_transforms.Scale = scale;
}

Transform GObject::GetTransform(void) const
{
	return m_transforms;
}

Mesh* GObject::GetMesh(void) const
{
	return m__mesh;
}