#include "BasicObject.h"

BasicObject::BasicObject(Mesh * mesh) 
	: m_mesh(mesh)
	, m_bLighted(false)
	, m_bFogged(false)
	, m_bBillboard(false)
{
}

BasicObject::~BasicObject()
{
}

void BasicObject::SetPos(Vector3 pos)
{
	m_transforms.Translation = pos;
}

void BasicObject::SetRot(Vector3 rot)
{
	m_transforms.Rotation = rot;
}

void BasicObject::SetScale(Vector3 scale)
{
	m_transforms.Scale = scale;
}

void BasicObject::SetMesh(Mesh* mesh)
{
	m_mesh = mesh;
}

void BasicObject::SetLighted(bool light)
{
	m_bLighted = light;
}

void BasicObject::SetFogged(bool fog)
{
	m_bFogged = fog;
}

void BasicObject::SetBillboard(bool billboard)
{
	m_bBillboard = billboard;
}

Transform BasicObject::GetTransform(void) const
{
	return m_transforms;
}

Mesh* BasicObject::GetMesh()
{
	return m_mesh;
}

bool BasicObject::IsLighted(void)
{
	return m_bLighted;
}

bool BasicObject::IsFogged(void)
{
	return m_bFogged;
}

bool BasicObject::IsBillboard(void)
{
	return m_bBillboard;
}
