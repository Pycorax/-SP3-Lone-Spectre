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
	, m_checkAround(0)
	
{
	for (size_t anim = 0; anim < NUM_ENEMY_ACTION; ++anim)
	{
		m__animationList[anim] = NULL;
	}
}

Enemy::~Enemy(void)
{
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
	
	//update view distance according to alert level
	InitViewer(1, m_alertLevel + 2);
	
	// Update FOV
	ClearViewBox(this, _map);
	CreateViewBox(this, _map);

	// Possession
	if(m_bPossesion == false && m_enemyState == ES_POSSESED)
	{
		if (MoveTo(m_pathWay[m_pathPointCounter], _map, dt))
		{
			if (m_pathPointCounter < m_pathWay.size() - 1)
			{
				m_pathPointCounter++;
				m_enemyState = ES_KNOCKOUT;
			}
			else
			{
				m_pathPointCounter = 0;
				m_enemyState = ES_KNOCKOUT;
			}
		}

	}
	switch (m_enemyState)
	{
		case ES_PATROL:
		{
			//** NOTE: array stores the tile position not map position **
			if (MoveTo(m_pathWay[m_pathPointCounter], _map, dt))
			{
				if (m_pathPointCounter < m_pathWay.size() - 1)
				{
					m_pathPointCounter++;
				}
				else
				{
					m_pathPointCounter = 0;
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
			UpdatePath();
			vector<AINode*> chasePath = GetPath();

			if (chasePath.size() > 0)
			{
				Vector2 targetPos(chasePath.front()->m_gridPosX, chasePath.front()->m_gridPosY);
				if(MoveTo(targetPos, _map, dt) )
				{
					//TODO: update the next point along the path here?

				}
			}
		}
	case ES_ATTACK:
		{
			//if alert level go below 2 , go back to patroling
			if(m_alertLevel < 2)
			{
				m_enemyState = ES_SCAN;
			}
			break;
		}
	case ES_POSSESED:
		{
			break;
		}
	case ES_SCAN:
		{
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
			else //timing is over
			{
				m_checkAround = 0;
				m_enemyState = ES_PATROL;
			}
			m_checkAround += dt;
			break;
		}
	case ES_KNOCKOUT:
		{
			m_checkAround += dt;
			if( m_checkAround > 4)
			{
				m_checkAround = 0;
				m_enemyState = ES_SCAN;
			}
		}
	}
	if (m_alertLevel >= 3 )
	{
		m_enemyState = ES_ATTACK;
	}
	ChangeAnimation(dt);
	SetMapPosition(GetMapPos(), _map->GetScrollOffset(), _map->GetTileSize());
}

bool Enemy::AttackingInView(Character* _go)
{
	//attacked player - target character
	if(_go->GetHealth() > 0 )
	{
		_go->AddToHealth(-1);
		return true;
	}
	return false;
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
		if (m_lookDir == Direction::DIRECTIONS[Direction::DIR_LEFT])
		{
			m_enemyAction = EA_WALK_LEFT;
		}
		else if (m_lookDir == Direction::DIRECTIONS[Direction::DIR_RIGHT])
		{
			m_enemyAction = EA_WALK_RIGHT;
		}
		if (m_lookDir == Direction::DIRECTIONS[Direction::DIR_DOWN])
		{
			m_enemyAction = EA_WALK_DOWN;
		}
		else if (m_lookDir == Direction::DIRECTIONS[Direction::DIR_UP])
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

static bool s_LeftRightMove = false; // if going diagonal direction, move left/right then move up/down
//return true if reached 
bool Enemy::MoveTo(Vector2 EndPos, TileMap* _map, double dt)
{
	static const float S_MOVE_SPEED = 60.0f;

	//if standing on the tile
	if(EndPos == GetMapTilePos() )
	{
		return true;
	}

	//set look direction towards next target location base off current tile location on map
	m_lookDir = (EndPos - GetMapTilePos()).Normalized();

	//diagonal movement move left right first 
	if(m_lookDir.x != 0 )
	{
		// set direction to either facing left or right
		if(m_lookDir.x < 0)
		{
			m_lookDir = Direction::DIRECTIONS[Direction::DIR_LEFT];
		}
		else
		{
			m_lookDir = Direction::DIRECTIONS[Direction::DIR_RIGHT];
		}
	}
	//then move up down after moving left right
	else if (m_lookDir.y != 0 )
	{
		// set direction to either facing up or down
		if(m_lookDir.y < 0)
		{
			m_lookDir = Direction::DIRECTIONS[Direction::DIR_DOWN];
		}
		else
		{
			m_lookDir = Direction::DIRECTIONS[Direction::DIR_UP];
		}
	}

	//checking if tile reached 
	//checking left
	if(m_lookDir.x < 0 && EndPos == GetMapTilePos() + Vector2(1,0) )
	{
		return true;
	}
	//checking right
	else if(m_lookDir.x > 0 && EndPos == GetMapTilePos() - Vector2(1,0))
	{
		return true;
	}
	//check down
	else if(m_lookDir.y < 0 && EndPos == GetMapTilePos() + Vector2(0,1))
	{
		s_LeftRightMove = true;
		return true;
	}
	//checking up
	else if(m_lookDir.y > 0 && EndPos == GetMapTilePos() - Vector2(0,1))
	{
		s_LeftRightMove = true;
		return true;
	}
	//converting the end pos from tile pos to map pos
	Vector2 TargetmapPos = Vector2(EndPos.x * _map->GetTileSize(), EndPos.y * _map->GetTileSize());
	//next location by map pos
	Vector2 newMapPos = GetMapPos() + m_lookDir * S_MOVE_SPEED * dt;

	//go to next position
	SetMapPosition(newMapPos, _map->GetScrollOffset(), _map->GetTileSize());
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

void Enemy::pathFinder_getTilePosition(unsigned& tileXPos, unsigned& tileYPos) const
{
	Vector2 mapTilePos = GetMapTilePos();

	tileXPos = mapTilePos.x;
	tileYPos = mapTilePos.y;
}

Vector2 Enemy::viewer_GetTilePos(void)
{
	return GetMapTilePos();		
}

Vector2 Enemy::viewer_GetDirection(void)
{
	return m_lookDir;
}
