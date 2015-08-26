#include "Enemy.h"

#include "ViewerUpdater.h"

Enemy::Enemy(void)
:m_alertLevel(0)
, m_enemyState(ES_PATROL)
, m_bReachPos(false)
, m_pathPointCounter(0)
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

void Enemy::Update(double dt, TileMap* _map)
{
	Character::Update();

	//PathFinder::UpdatePath(_map->GetTileSize());

	// Update FOV
	UpdateViewBox(this, _map);

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

	switch (m_enemyState)
	{
		case ES_PATROL:
		{
			//MoveTo(m_pathWay.GetPoint(m_pathPointCounter).ToVector3(), _map, dt) ;// updates to next postition

			if (MoveTo(m_pathWay[m_pathPointCounter], _map, dt))
			{

				if (m_pathPointCounter >= m_pathWay.size() - 1)
				{
					m_pathPointCounter = 0;
				}
				else
				{
					m_pathPointCounter++;
				}

				break;
			}
		}
		case ES_CHASE:
		{
			//Make enemy chase after the hero's current position with path-finding
			m_bAlerted = true;
			Vector2 nextTarget;
			UpdatePath(_map->GetTileSize());
			vector<AINode*> path = GetPath();
			if (path.size() > 0)
			{
				nextTarget.x = path[m_pathPointCounter]->m_gridPosX;
				nextTarget.y = path[m_pathPointCounter]->m_gridPosY;
			}

			if (MoveTo(nextTarget, _map, dt));
			{
				if (path.size() == m_pathPointCounter) // if patrol counter reached the last one
				{
					m_pathPointCounter = 0; // reset back to 0
				}
				else
				{
					m_pathPointCounter += 1; // move on to next point
				}
			}
			if (m_bAlerted)
			{
				if (m_alertLevel < 3)
				{
					m_alertLevel += dt;
				}
			}
		}
		case ES_ATTACK:
		{
			break;
		}
		case ES_POSSESED:
		{
			if (m_oldPos != m_spectralPositon)
			{
				SetMapPosition(m_oldPos, _map->GetScrollOffset(), _map->GetTileSize());
			}
			break;
		}
		case ES_SCAN:
		{
			if (m_alertLevel > 0)
			{
				m_checkAround = 0;
				static const double S_WAIT_TIME = 2.0;

				if (m_checkAround >= S_WAIT_TIME * 1)
				{
					m_lookDir = S_DIRECTION[Character::DIR_UP];
				}
				else if (m_checkAround >= 2 && m_checkAround < S_WAIT_TIME * 2)
				{
					m_lookDir = S_DIRECTION[Character::DIR_DOWN];
				}
				else if (m_checkAround >= 4 && m_checkAround < S_WAIT_TIME * 3)
				{
					m_lookDir = S_DIRECTION[Character::DIR_LEFT];
				}
				else if (m_checkAround >= 6 && m_checkAround < S_WAIT_TIME * 4)
				{
					m_lookDir = S_DIRECTION[Character::DIR_RIGHT];
				}
				m_alertLevel -= 1;
				m_checkAround += dt;
			}
			else
			{
				m_enemyState = ES_PATROL;
			}
			//Check the area for 2 rotation
			break;
		}
	}
}

void Enemy::AddAnimation(Animation* _anim, E_ENEMY_ACTION enemyState)
{
	m__animationList[enemyState] = _anim;
}

void Enemy::ChangeAnimation()
{
	if (m_bPossesion == true || m_bChasing == true || m_bSearching == true)
	{
		if (m_lookDir == S_DIRECTION[Character::DIR_DOWN])
		{
			m_enemyAction = EA_WALK_DOWN;
		}
		else if (m_lookDir == S_DIRECTION[Character::DIR_UP])
		{
			m_enemyAction = EA_WALK_UP;
		}
		else if (m_lookDir == S_DIRECTION[Character::DIR_LEFT])
		{
			m_enemyAction = EA_WALK_LEFT;
		}
		else if (m_lookDir == S_DIRECTION[Character::DIR_RIGHT])
		{
			m_enemyAction = EA_WALK_RIGHT;
		}
	}
	else if (m_bScanning == true || m_bAttacking == true)
	{
		if (m_lookDir == S_DIRECTION[Character::DIR_DOWN])
		{
			m_enemyAction = EA_IDLE_DOWN;
		}
		else if (m_lookDir == S_DIRECTION[Character::DIR_UP])
		{
			m_enemyAction = EA_IDLE_UP;
		}
		else if (m_lookDir == S_DIRECTION[Character::DIR_LEFT])
		{
			m_enemyAction = EA_IDLE_LEFT;
		}
		else if (m_lookDir == S_DIRECTION[Character::DIR_RIGHT])
		{
			m_enemyAction = EA_IDLE_RIGHT;
		}
	}
}

void Enemy::AddPatrolPoint(Vector2 pos)
{
	m_pathWay.push_back(pos);
}

//return true if reached 
bool Enemy::MoveTo(Vector2 EndPos, TileMap* _map, double dt) //TODO: PathFinding
{
	//set look direction towards next target location base off current location on map
	m_lookDir = (EndPos - GetMapPos()).Normalized();
	//next location
	Vector2 newMapPos = GetMapPos() + GetLookDir();

	if (_map->CheckCollision(newMapPos)) // check collision at next pos
	{
		// swap pos - patrolPointB - target location
		return true; // reached a dead end
	}
	else
	{
		if (m_lookDir.x > 0 && newMapPos.x >= EndPos.x) //traveling along x axis -> moving right
		{
			return true; // reached target
		}
		else if (m_lookDir.x < 0 && newMapPos.x <= EndPos.x) // -> moving left
		{
			return true; // reached target
		}
		if (m_lookDir.y > 0 && newMapPos.y >= EndPos.y) //traveling along y axis -> moving up
		{
			return true; // reached target
		}
		else if (m_lookDir.y < 0 && newMapPos.y <= EndPos.y) // -> moving down
		{
			return true; // reached target
		}
		SetMapPosition(newMapPos, _map->GetScrollOffset(), _map->GetTileSize());
	}
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

Vector2 Enemy::pathFinder_getTilePosition(void)
{
	return GetMapPos();
}

Vector2 Enemy::viewer_GetTilePos(void)
{
	return GetMapTilePos();		// TODO: This will start working when proper tile map positions are passed in like intended
}

Vector2 Enemy::viewer_GetDirection(void)
{
	return m_lookDir;
}

void Enemy::SetSpectrePosition(Vector2 spectralPosition)
{
	this->m_spectralPositon = spectralPosition;
}
