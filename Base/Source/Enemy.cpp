#include "Enemy.h"


Enemy::Enemy(void)
	:m_alertLevel(0)
	, m_enemyState(ES_PATROL)
	, m_bPatrolling(false)
	, m_bReachPos(false)
{

}

Enemy::~Enemy(void)
{
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
		//Make the enemy walk from a place to another place
		m_bPatrolling = true;
		if(m_bPatrolling)
		{
			MoveTo(m_oldPos + 100, _map);
		}
	}
	else if (m_enemyState == ES_CHASE)
	{
		//Make enemy chase after the hero's current position with path-finding
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


	if(!m_bPatrolling)//updates oldpos , can be used for other - when not patrolling
	{
		m_oldPos = m_patrolPointA = m_transforms.Translation;//set start patrol position using initial enemy pos
	}
}

bool Enemy::MoveTo(Vector2 pos, TileMap* _map) //TODO: passed in map - used for collision when doing PathFinding
{
	m_patrolPointB = pos; //set the end patrol position

	Vector2 moveDirection = (m_patrolPointB - m_patrolPointA).Normalized();//**note patrol point a is start pos and enemy initial pos
	
	m_transforms.Translation += moveDirection * 10.f; // moving to the patrol point 

	if(m_transforms.Translation == m_patrolPointB && !m_bReachPos) // swap position
	{
		m_patrolPointA = m_patrolPointA + m_patrolPointB;//add tgt (ab)
		m_patrolPointB = m_patrolPointA - m_patrolPointB;//set b = a(ab) - b :: get a
		m_patrolPointA = m_patrolPointA - m_patrolPointB;//set a = a(ab) - b(a) :: get b
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