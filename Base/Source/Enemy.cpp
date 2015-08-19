#include "Enemy.h"


Enemy::Enemy(void)
	:m_alertLevel(0)
	,m_enemyState(ES_PATROL)
{

}

Enemy::~Enemy(void)
{
}

void Enemy::update(double dt)
{
	//if ()//If any enemy see Hero, affects other enemies too
	//{
	//	m_enemyState = ES_CHASE;
	//	alerted = true;
	//  m_alertLevel = 2;
	//}
	//else if() //Enemy can hit Hero
	//{
	//	m_enemyState = ES_ATTACK;
	//}
	//else if (alerted == true) //Enemy lost sight of Hero || hero hides in a shadow
	//{
	//	m_enemyState = ES_SCAN;
	//}
	//else if() //Spectre shadowDive in front of enemy
	//{
	//  m_enemyState = ES_REVEAL
	//}
	//else if () //Enemy dies
	//{
	//	m_enemyState = ES_KNOCKOUT;
	//  alerted = false;
	//}
	//else if (m_alertLevel == 0) //Enemy becomes less suspicious after checking
	//{
	//	m_enemyState = ES_PATROL;
	//  alerted = false;
	//}

	if (m_enemyState == ES_PATROL)
	{
		//Make the enemy walk from a place to another place
	}
	else if (m_enemyState == ES_CHASE)
	{
		//Make enemy chase after the hero's current position with path-finding
	}
	else if (m_enemyState == ES_ATTACK)
	{
		//Hit him already
	}
	else if (m_enemyState == ES_REVEAL)
	{
		//Reveal him
	}
	else if (m_enemyState == ES_SCAN)
	{
		//Check the area for 2 rotation
		m_alertLevel -= 1;
	}
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