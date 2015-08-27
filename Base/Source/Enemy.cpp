#include "Enemy.h"
#include "Direction.h"
#include "ViewerUpdater.h"

Enemy::Enemy(void)
	:m_alertLevel(0)
	, m_enemyState(ES_PATROL)
	, m_bReachPos(false)
	, m_pathPointCounter(0)
	, m_bPossesion(false)
	, m_moveTime(0)
	
{

}

Enemy::~Enemy(void)
{
	// TODO: Delete animations
}

void Enemy::Init(Vector2 pos, Mesh* _mesh)
{
	m_oldPos = pos;
	SetMesh(_mesh );
	m_lookDir = Direction::DIRECTIONS[Direction::DIR_RIGHT];
	m_enemyAction = EA_IDLE_RIGHT;
	//patrol mode
	m_enemyState = ES_PATROL;

	Animation* _a;
	//Idle South
	_a = new Animation();
	_a->Set(1, 1, 0, 0.f);
	AddAnimation(_a, Enemy::EA_IDLE_DOWN);

	//Idle East
	_a = new Animation();
	_a->Set(4, 4, 0, 0.f);
	AddAnimation(_a, Enemy::EA_IDLE_RIGHT);

	//Idle East
	_a = new Animation();
	_a->Set(7, 7, 0, 0.f);
	AddAnimation(_a, Enemy::EA_IDLE_LEFT);

	//Idle North
	_a = new Animation();
	_a->Set(10, 10, 0, 0.f);
	AddAnimation(_a, Enemy::EA_IDLE_UP);

	//Walk South
	_a = new Animation();
	_a->Set(0, 2, 0, 0.4f);
	AddAnimation(_a, Enemy::EA_WALK_DOWN);

	//Walk East
	_a = new Animation();
	_a->Set(3, 5, 0, 0.4f);
	AddAnimation(_a, Enemy::EA_WALK_RIGHT);

	//Walk East
	_a = new Animation();
	_a->Set(6, 8, 0, 0.4f);
	AddAnimation(_a, Enemy::EA_WALK_LEFT);

	//Walk North
	_a = new Animation();
	_a->Set(9, 11, 0, 0.4f);
	AddAnimation(_a, Enemy::EA_WALK_UP);
}

void Enemy::SetPossesion(bool state)
{
	m_bPossesion = state;
}

void Enemy::Update(double dt, TileMap* _map)
{
	Character::Update();

	//PathFinder::UpdatePath(_map->GetTileSize());

	// Update FOV
	ClearViewBox(this, _map);
	CreateViewBox(this, _map);

	//if ()//If any enemy see Hero, affects other enemies too
	//{
	//	m_enemyState = ES_CHASE;
	//	m_alertLevel = 2;
	//}
	////else if() //Enemy can hit Hero
	////{
	////	m_enemyState = ES_ATTACK;
	////}
	//else if (m_bAlerted) //Enemy lost sight of Hero || hero hides in a shadow
	//{
	//	m_enemyState = ES_SCAN;
	//}
	//else if(m_bPossesion) //Spectre hosts on the enemy
	//{
	//	m_enemyState = ES_POSSESED
	//}
	//else if (GetHealth() <= 0) //Enemy dies
	//{
	//	m_enemyState = ES_KNOCKOUT;
	//	m_bAlerted = false;
	//}
	//else if (m_alertLevel == 0) //Enemy becomes less suspicious after checking
	//{
	//	m_enemyState = ES_PATROL;
	//	m_bAlerted = false;
	//}
	if(m_bPossesion == false && m_enemyState == ES_POSSESED)
	{
		if(MoveTo(m_pathWay[0], _map, dt) )
		{
			m_pathPointCounter = 0;
			m_enemyState = ES_PATROL;
		}
	}

	switch (m_enemyState)
	{
		case ES_PATROL:
		{
			//** NOTE: array store the tile position not map position **
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
			}
			break;
		}
		case ES_CHASE:
		{
			////Make enemy chase after the hero's current position with path-finding
			//m_bAlerted = true;
			//Vector2 nextTarget;
			//UpdatePath(_map->GetTileSize());
			//vector<AINode*> path = GetPath();
			//if (path.size() > 0)
			//{
			//	nextTarget.x = path[m_pathPointCounter]->m_gridPosX;
			//	nextTarget.y = path[m_pathPointCounter]->m_gridPosY;
			//}

			//if (MoveTo(nextTarget, _map, dt));
			//{
			//	if (path.size() == m_pathPointCounter) // if patrol counter reached the last one
			//	{
			//		m_pathPointCounter = 0; // reset back to 0
			//	}
			//	else
			//	{
			//		m_pathPointCounter += 1; // move on to next point
			//	}
			//}
			//if (m_bAlerted)
			//{
			//	if (m_alertLevel < 3)
			//	{
			//		m_alertLevel += dt;
			//	}
			//}
		}
	case ES_ATTACK:
		{
			break;
		}
	case ES_POSSESED:
		{
			break;
		}
	case ES_SCAN:
		{
			if (m_alertLevel > 0)
			{
				
			//Rotate and check the area for depending on alert level : MAX (2)
				m_checkAround = 0;
				static const double S_WAIT_TIME = 2.0;

				if (m_checkAround < S_WAIT_TIME * 1)
				{
					m_lookDir = Direction::DIRECTIONS[Direction::DIR_UP];
				}
				else if (m_checkAround >= S_WAIT_TIME * 1 && m_checkAround < S_WAIT_TIME * 2)
				{
					m_lookDir = Direction::DIRECTIONS[Direction::DIR_DOWN];
				}
				else if (m_checkAround >= S_WAIT_TIME * 2 && m_checkAround < S_WAIT_TIME * 3)
				{
					m_lookDir = Direction::DIRECTIONS[Direction::DIR_LEFT];
				}
				else if (m_checkAround >= S_WAIT_TIME * 3 && m_checkAround < S_WAIT_TIME * 4)
				{
					m_lookDir = Direction::DIRECTIONS[Direction::DIR_RIGHT];
				}
				m_alertLevel -= 1;
				m_checkAround += dt;
			}
			else // alert level (0)
			{
				m_enemyState = ES_PATROL;
			}
			break;
		}
	}
	ChangeAnimation(dt);
	SetMapPosition(GetMapPos(), _map->GetScrollOffset(), _map->GetTileSize());
}

void Enemy::AddAnimation(Animation* _anim, E_ENEMY_ACTION enemyState)
{
	m__animationList[enemyState] = _anim;
}

void Enemy::ChangeAnimation(double dt)
{
	if (m_enemyState == ES_POSSESED || m_enemyState == ES_SCAN ||m_enemyState == ES_ATTACK )
	{
		if (m_lookDir == Direction::DIRECTIONS[Direction::DIR_DOWN])
		{
			m_enemyAction = EA_IDLE_DOWN;
		}
		else if (m_lookDir == Direction::DIRECTIONS[Direction::DIR_UP])
		{
			m_enemyAction = EA_IDLE_UP;
		}
		if (m_lookDir == Direction::DIRECTIONS[Direction::DIR_LEFT])
		{
			m_enemyAction = EA_IDLE_LEFT;
		}
		else if (m_lookDir == Direction::DIRECTIONS[Direction::DIR_RIGHT])
		{
			m_enemyAction = EA_IDLE_RIGHT;
		}
	}
	else if (m_enemyState == ES_POSSESED  || m_enemyState == ES_CHASE || m_enemyState == ES_PATROL )
	{
		if (m_lookDir.x < 0)
		{
			m_enemyAction = EA_WALK_LEFT;
		}
		else if (m_lookDir.x > 0)
		{
			m_enemyAction = EA_WALK_RIGHT;
		}
		if (m_lookDir.y < 0)
		{
			m_enemyAction = EA_WALK_DOWN;
		}
		else if (m_lookDir.y > 0)
		{
			m_enemyAction = EA_WALK_UP;
		}
	}
	//updates the sprite animtion with the correct set of animation
	SpriteAnimation* _sa = dynamic_cast<SpriteAnimation* >(m__mesh);
	if(_sa)
	{
		_sa->m_anim = m__animationList[m_enemyAction];
		_sa->Update(dt);
	}
}
void Enemy::ForceSetEnemyState(E_ENEMY_STATE enemyState)
{
	m_enemyState = enemyState;
}

void Enemy::AddPatrolPoint(Vector2 pos)
{
	m_pathWay.push_back(pos);
}

//return true if reached 
bool Enemy::MoveTo(Vector2 EndPos, TileMap* _map, double dt)
{
	if(EndPos == GetMapTilePos() )
	{
		return true; // reached target
	}
	//set look direction towards next target location base off current tile location on map
	m_lookDir = (EndPos - GetMapTilePos()).Normalized();
	Vector2 TargetmapPos = Vector2(EndPos.x * _map->GetTileSize(), EndPos.y * _map->GetTileSize());
	//next location adding using tile 
	Vector2 newMapPos = GetMapPos() + m_lookDir;

	if (_map->CheckCollision(newMapPos)) // check collision at next pos
	{
		// swap pos - patrolPointB - target location
		return true; // reached a dead end
	}
	else
	{
		if (m_lookDir.x > 0 && newMapPos.x >= TargetmapPos.x) //traveling along x axis -> moving right
		{
			return true; // reached target
		}
		else if (m_lookDir.x < 0 && newMapPos.x <= TargetmapPos.x) // -> moving left
		{
			return true; // reached target
		}
		if (m_lookDir.y > 0 && newMapPos.y >= TargetmapPos.y) //traveling along y axis -> moving up
		{
			return true; // reached target
		}
		else if (m_lookDir.y < 0 && newMapPos.y <= TargetmapPos.y) // -> moving down
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

void Enemy::SpottedTarget(Vector2 pos, float &alertLevel, double dt)
{
	m_bAlerted = true;
	alertLevel += dt;
}

Vector2 Enemy::pathFinder_getTilePosition(void)
{
	return GetMapTilePos();
}

Vector2 Enemy::viewer_GetTilePos(void)
{
	return GetMapTilePos();		// TODO: This will start working when proper tile map positions are passed in like intended
}

Vector2 Enemy::viewer_GetDirection(void)
{
	return m_lookDir;
}
