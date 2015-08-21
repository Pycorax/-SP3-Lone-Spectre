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
	//{y
	//	m_enemyState = ES_PATROL;
	//  alerted = false;
	//}

	if (m_enemyState == ES_PATROL)
	{
		m_lookDir = (m_patrolPointB - m_patrolPointA).Normalized();//**note patrol point a is start pos and enemy initial pos
		Vector2 newMapPos;

		newMapPos = GetMapPos() + (GetLookDir() * _map->GetTileSize() * dt); // moving to the patrol point 

		if(m_lookDir.x == 1 && newMapPos.x >= m_patrolPointB.x) //traveling along x axis -> moving right
		{
			// swap pos - patrolPointB - target location
			Vector2 tempStore;
			tempStore = m_patrolPointB;
			m_patrolPointB = m_patrolPointA;
			m_patrolPointA = tempStore;
		}
		else if(m_lookDir.x == -1 && newMapPos.x <= m_patrolPointB.x) // -> moving left
		{
			// swap pos - patrolPointB - target location
			Vector2 tempStore;
			tempStore = m_patrolPointB;
			m_patrolPointB = m_patrolPointA;
			m_patrolPointA = tempStore;
		}
		if(m_lookDir.y == 1 && newMapPos.y >= m_patrolPointB.y) //traveling along y axis -> moving up
		{
			// swap pos - patrolPointB - target location
			Vector2 tempStore;
			tempStore = m_patrolPointB;
			m_patrolPointB = m_patrolPointA;
			m_patrolPointA = tempStore;
		}
		else if(m_lookDir.y == -1 && newMapPos.y <= m_patrolPointB.y ) // -> moving down
		{
			// swap pos - patrolPointB - target location
			Vector2 tempStore;
			tempStore = m_patrolPointB;
			m_patrolPointB = m_patrolPointA;
			m_patrolPointA = tempStore;
		}
		if(m_bReachPos) // reset to not reached target after bouncing
		{
			m_bReachPos = false;
		}
		SetMapPosition(newMapPos, _map->GetScrollOffset() );
	}
	else if (m_enemyState == ES_CHASE)
	{
		//Make enemy chase after the hero's current position with path-finding
		m_bAlerted = true;

		if (m_bAlerted)
		{
			if (m_alertLevel < 3)
			{
				m_alertLevel += dt;
			}
		}
	}
	else if (m_enemyState == ES_ATTACK)
	{
		//Hit him already
		
	}
	else if (m_enemyState == ES_POSSESED)
	{
		if (m_oldPos.x != m_spectralPositon.x)
		{
			m_oldPos.x = m_spectralPositon.x;
		}
		if (m_oldPos.y != m_spectralPositon.y)
		{
			m_oldPos.y = m_spectralPositon.y;
		}
	}
	else if (m_enemyState == ES_SCAN)
	{
		m_bScanning = true;
		if (m_bScanning == true)
		{
			if (m_alertLevel > 0)
			{
				m_checkAround = 0;
				if (m_checkAround >= 2)
				{
					m_lookDir = Vector2(1, 0);
				}
				else if (m_checkAround >= 2 && m_checkAround < 4)
				{
					m_lookDir = Vector2(0, 1);
				}
				else if (m_checkAround >= 4 && m_checkAround < 6)
				{
					m_lookDir = Vector2(-1, 0);
				}
				else if (m_checkAround >= 6 && m_checkAround < 8)
				{
					m_lookDir = Vector2(0, -1);
				}
				m_alertLevel -= 1;
			}
			else
			{
				m_bScanning = false;
			}
		}
		//Check the area for 2 rotation
	}
	m_checkAround += dt;
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
	m_lookDir = (m_patrolPointB - m_patrolPointA).Normalized();//**note patrol point a is start pos and enemy initial pos
	Vector2 newMapPos;
	
	if(!_map->CheckCollision(GetMapPos() + (GetLookDir() * _map->GetTileSize() )) ) // checks next position if have anything
	{
		newMapPos = GetMapPos() + (GetLookDir() * _map->GetTileSize() );//* dt); // moving to the patrol point 
	}
	else 
	{
		//swap target position to other side
		Vector2 tempStore;
		tempStore = m_patrolPointB;
		m_patrolPointB = m_patrolPointA;
		m_patrolPointA = tempStore;
	}

	if(m_lookDir.x == 1 && newMapPos.x >= m_patrolPointB.x) //traveling along x axis -> moving right
	{
		// swap pos - patrolPointB -> target location
		Vector2 tempStore;

		tempStore = m_patrolPointB;
		m_patrolPointB = m_patrolPointA;
		m_patrolPointA = tempStore;
	}
	else if(m_lookDir.x == -1 && newMapPos.x <= m_patrolPointB.x) // -> moving left
	{
		// swap pos - patrolPointB -> target location
		Vector2 tempStore;
		tempStore = EndPos;
		EndPos = StartPos;
		StartPos = tempStore;
		m_bReachPos = true;
			
	}
	if(m_lookDir.y == 1 && newMapPos.y >= m_patrolPointB.y) //traveling along y axis -> moving up
	{
		// swap pos - patrolPointB -> target location
		Vector2 tempStore;
		tempStore = m_patrolPointB;
		m_patrolPointB = m_patrolPointA;
		m_patrolPointA = tempStore;
	}
	else if(m_lookDir.y == -1 && newMapPos.y <= m_patrolPointB.y ) // -> moving down
	{
		// swap pos - patrolPointB -> target location
		Vector2 tempStore;
		tempStore = m_patrolPointB;
		m_patrolPointB = m_patrolPointA;
		m_patrolPointA = tempStore;
	}
	SetMapPosition(newMapPos, _map->GetScrollOffset() );
	return false;
}

void Enemy::SetAlertLevel(int alertlevel)
{
	this->m_alertLevel = alertlevel;
}

int Enemy::GetAlertLevel(void)
{
	return this->m_alertLevel;
}

void Enemy::SpottedTarget(Vector2 pos)
{
}

void Enemy::SetSpectrePosition(Vector2 spectralPosition)
{
	this->m_spectralPositon = spectralPosition;
}
