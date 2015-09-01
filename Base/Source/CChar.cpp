#include "CChar.h"

Character::Character(void)
	: m_health(0)
	, m_maxHealth(0)
	, m_animMesh(NULL)
	, m_lookDir(Vector2::ZERO_VECTOR)
	, m_active(false)
	, m_moveSpeed(0)
	, MapEntity( Vector2(0,0) )
{
}

Character::~Character(void)
{
}

void Character::Init(bool active, int health, Vector2 dir, Mesh* mesh,int moveSpeed)
{
	this->m_active							= active;
	this->m_maxHealth = this->m_health		= health;
	this->m_lookDir							= dir;
	this->m_animMesh						= mesh;
	this->m_moveSpeed						= moveSpeed;
}

void Character::Update()
{
}

void Character::SetHealth(int health)
{
	this->m_health = health;
}
void Character::SetMaxHealth(int health)
{
	this->m_maxHealth = health;
}

void Character::AddToHealth(int health_plus)
{
	this->m_health += health_plus;
}

void Character::SetLookDir(Vector2 dir)
{
	this->m_lookDir = dir;
}
void Character::SetMoveSpeed(int moveSpeed)
{
	this->m_moveSpeed = moveSpeed;
}

void Character::SetAnimMesh(Mesh* mesh)
{
	this->m_animMesh = mesh;
}
void Character::SetActive(bool active)
{
	this->m_active = active;
}

// === Get functions ===
int Character::GetHealth(void)const
{
	return this->m_health;
}
int Character::GetMaxHealth(void)const
{
	return this->m_maxHealth;
}

Vector2 Character::GetLookDir(void)const
{
	return this->m_lookDir;
}
int Character::GetMoveSpeed(void) const
{
	return this->m_moveSpeed;
}

Mesh* Character::GetAnimMesh(void)const
{
	return this->m_animMesh;
}
bool Character::GetActive(void) const
{
	return this->m_active;
}

void Character::updateScreenPos(Vector2 pos)
{
	m_transforms.Translation = pos;
}

void Character::SpottedTarget(Vector2 pos)
{
	
}