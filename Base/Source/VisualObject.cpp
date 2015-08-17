#include "VisualObject.h"

VisualObject::VisualObject(Mesh * mesh/* = NULL*/) : BasicObject(mesh), m_repeat(0)
{
}

VisualObject::~VisualObject()
{
}

void VisualObject::SetDeltaTransform(Transform deltaTrans)
{
	m_deltaTransform = deltaTrans;
}

void VisualObject::SetDeltaPos(Vector3 deltaPos)
{
	m_deltaTransform.Translation = deltaPos;
}

void VisualObject::SetDeltaRot(Vector3 deltaRot)
{
	m_deltaTransform.Rotation = deltaRot;
}

void VisualObject::SetDeltaScale(Vector3 deltaScale)
{
	m_deltaTransform.Scale = deltaScale;
}

void VisualObject::SetRepeat(int repeat)
{
	m_repeat = repeat;
}

Transform VisualObject::GetDeltaTransform(void)
{
	return m_deltaTransform;
}

int VisualObject::GetRepeat(void)
{
	return m_repeat;
}
