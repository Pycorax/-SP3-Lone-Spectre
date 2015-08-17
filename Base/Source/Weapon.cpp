#include "Weapon.h"

Weapon::Weapon(string name/* = "Unnamed Weapon" */, float damage/* = 0.0f */, float range/* = 0.0f */) : m_name(name), m_fDamage(damage), m_fRange(range), m_type(NUM_WEAPON_TYPE), m_mesh(NULL)
{
	m_shotSound = "";
}

Weapon::~Weapon(void)
{
}

void Weapon::SetName(string name)
{
	m_name = name;
}

void Weapon::SetMesh(Mesh * mesh)
{
	m_mesh = mesh;
}

// Setters
void Weapon::SetDamage(float damage)
{
	m_fDamage = damage;
}

void Weapon::SetRange(float range)
{
	m_fRange = range;
}

void Weapon::SetShotSound(string filePath)
{
	m_shotSound = filePath;
}

string Weapon::GetName(void)
{
	return m_name;
}

Mesh * Weapon::GetMesh(void)
{
	return m_mesh;
}

// Getters
float Weapon::GetDamage(void)
{
	return m_fDamage;
}

float Weapon::GetRange(void)
{
	return m_fRange;
}

Weapon::WEAPON_TYPE Weapon::GetType(void)
{
	return m_type;
}

string Weapon::GetAttackSound(void)
{
	return m_shotSound;
}
