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
	this->SetPos(pos);
	m_oldPos = pos;
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
		Vector2 moveDirection = (m_patrolPointB - m_patrolPointA).Normalized();//**note patrol point a is start pos and enemy initial pos

		m_transforms.Translation += moveDirection; // moving to the patrol point 

		if(m_transforms.Translation == m_patrolPointB && !m_bReachPos)// swap position
		{
			Vector2 tempStore;
			tempStore = m_patrolPointB;
			m_patrolPointB = m_patrolPointA;
			m_patrolPointA = tempStore;
			m_bReachPos = true;

		}	
		else if(m_transforms.Translation == m_oldPos && m_bReachPos)
		{
			Vector2 tempStore;
			tempStore = m_patrolPointB;
			m_patrolPointB = m_patrolPointA;
			m_patrolPointA = tempStore;
			m_bReachPos = false;
		}
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

void Enemy::SetStartPatrolPoint(Vector2 pos)
{
	m_patrolPointA = pos;
}

void Enemy::SetEndPatrolPoint(Vector2 pos)
{
	m_patrolPointB = pos;
}

bool Enemy::MoveTo(Vector2 StartPos, Vector2 EndPos, TileMap* _map) //TODO: PathFinding
{
	Vector2 moveDirection = (EndPos - StartPos).Normalized();//**note patrol point a is start pos and enemy initial pos
	
	m_transforms.Translation += moveDirection; // moving to the patrol point 

	if(m_transforms.Translation == EndPos && !m_bReachPos)// swap position
	{
		Vector2 tempStore;
		tempStore = EndPos;
		EndPos = StartPos;
		StartPos = tempStore;
		m_bReachPos = true;
			
	}	
	else if(m_transforms.Translation == m_oldPos && m_bReachPos)
	{
		Vector2 tempStore;
		tempStore = EndPos;
		EndPos = StartPos;
		StartPos = tempStore;
		m_bReachPos = false;
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

void Enemy::SpottedTarget(Vector2 pos)
{
}