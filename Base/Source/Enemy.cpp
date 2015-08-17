#include "Enemy.h"


Enemy::Enemy(void)
	:m_alertLevel(0)
{
}

Enemy::~Enemy(void)
{
}

void Enemy::update(double dt)
{
}

bool Enemy::MoveTo(Vector2 pos)
{
	return false;
}

void Enemy::SetAlertLevel(int alertlevel)
{
	this->m_alertLevel += alertlevel;
}

int Enemy::GetAlertLevel(void)
{
	return this->m_alertLevel;
}