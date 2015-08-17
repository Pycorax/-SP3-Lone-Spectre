#include "GObject.h"


GObject::GObject(void)
{
}


GObject::~GObject(void)
{
}

void GObject::SetPos(Vector3 pos)
{
	m_transforms.Translation = pos;
}

void GObject::SetRot(Vector3 rot)
{
	m_transforms.Rotation = rot;
}

void GObject::SetScale(Vector3 scale)
{
	m_transforms.Scale = scale;
}

Transform GObject::GetTransform(void) const
{
	return m_transforms;
}
