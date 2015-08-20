#include "Enemy.h"


Enemy::Enemy(void)
	:m_alertLevel(0)
	, m_enemyState(ES_PATROL)
	, m_bReachPos(false)
{

}

Enemy::~Enemy(void)
{
}

void Enemy::Init(Vector2 pos, E_ENEMY_STATE enemyState)
{
	this->m_transforms.Translation = m_oldPos = pos;
	m_enemyState = enemyState;
}

void Enemy::update(double dt, TileMap* _map)
{
	Character::Update();
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
	//else if() //Spectre hosts on the enemy
	//{
	//  m_enemyState = ES_POSSESED
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
		m_patrolPointA = m_oldPos;
		MoveTo(m_patrolPointA, m_patrolPointB, _map); // remember to set
	}
	else if (m_enemyState == ES_CHASE)
	{
		//Make enemy chase after the hero's current position with path-finding
		m_bAlerted = true;
	}
	else if (m_enemyState == ES_ATTACK)
	{
		//Hit him already
	}
	else if (m_enemyState == ES_POSSESED)
	{
		//Reveal him
	}
	else if (m_enemyState == ES_SCAN)
	{
		//Check the area for 2 rotation
		m_alertLevel -= 1;
	}

	if(m_bAlerted)
	{
		m_alertLevel += 1;
	}
}

void Enemy::SetEndPatrolPoint(Vector2 pos)
{
	m_patrolPointB = pos;
}

bool Enemy::MoveTo(Vector2 StartPos, Vector2 EndPos, TileMap* _map) //TODO: passed in map - used for collision when doing PathFinding
{
	Vector2 moveDirection = (EndPos - StartPos).Normalized();//**note patrol point a is start pos and enemy initial pos
	
	m_transforms.Translation += moveDirection * 10.f; // moving to the patrol point 

	if(m_transforms.Translation == EndPos && !m_bReachPos) // swap position
	{
		StartPos = StartPos + EndPos;//add tgt (ab)
		EndPos/* start pos*/ = StartPos - EndPos;//set b = a(ab) - b :: get a
		StartPos/* end pos*/ = StartPos - EndPos;//set a = a(ab) - b(a) :: get b
	}
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