#include "NPC.h"
#include "Direction.h"
#include "ViewerUpdater.h"

NPC::NPC(void)
	: m_npcType(NT_ENEMY)
	, m_alertLevel(0)
	, m_enemyState(ES_PATROL)
	, m_bReachPos(false)
	, m_pathPointCounter(0)
	, m_bPossesion(false)
	, m_moveTime(0)
	, m_checkAround(0)
	, m_AttackCountdown(0)
	, m_ViewingTimer(0)
	, m_wasPossessed(false)
	, _player(NULL)
	
{
	for (size_t anim = 0; anim < NUM_ENEMY_ACTION; ++anim)
	{
		m__animationList[anim] = NULL;
	}
}

NPC::~NPC(void)
{
}

NPC * NPC::CreateCopy(NPC npcToCopy)
{
	NPC* newNPC = new NPC(npcToCopy);

	for (size_t anim = 0; anim < NUM_ENEMY_ACTION; ++anim)
	{
		if (npcToCopy.m__animationList[anim] != NULL)
		{
			newNPC->m__animationList[anim] = new Animation(*npcToCopy.m__animationList[anim]);
		}
	}

	return newNPC;
}

void NPC::SetMoveToDist(Vector2 pos, float TileSize)
{
	m_lookDir = (pos - GetMapTilePos()).Normalized();
	m_moveToDist = ((pos * TileSize) - GetMapPos()).Length()/* * TileSize*/;
	m_moveDist = 0;
}

void NPC::Init(Vector2 pos, Mesh* _mesh)
{
	m_oldPos = pos;
	m_moveToDist = 0.f;
	m_moveDist = 0.f;

	SetMesh(_mesh );
	m_lookDir = Direction::DIRECTIONS[Direction::DIR_RIGHT];
	m_enemyAction = EA_IDLE_RIGHT;
	//patrol mode
	m_enemyState = ES_PATROL;

	Animation* _a;
	//Idle South
	_a = new Animation();
	_a->Set(1, 1, 0, 0.f);
	AddAnimation(_a, NPC::EA_IDLE_DOWN);

	//Idle East
	_a = new Animation();
	_a->Set(4, 4, 0, 0.f);
	AddAnimation(_a, NPC::EA_IDLE_RIGHT);

	//Idle East
	_a = new Animation();
	_a->Set(7, 7, 0, 0.f);
	AddAnimation(_a, NPC::EA_IDLE_LEFT);

	//Idle North
	_a = new Animation();
	_a->Set(10, 10, 0, 0.f);
	AddAnimation(_a, NPC::EA_IDLE_UP);

	//Walk South
	_a = new Animation();
	_a->Set(0, 2, 0, 0.4f);
	AddAnimation(_a, NPC::EA_WALK_DOWN);

	//Walk East
	_a = new Animation();
	_a->Set(3, 5, 0, 0.4f);
	AddAnimation(_a, NPC::EA_WALK_RIGHT);

	//Walk East
	_a = new Animation();
	_a->Set(6, 8, 0, 0.4f);
	AddAnimation(_a, NPC::EA_WALK_LEFT);

	//Walk North
	_a = new Animation();
	_a->Set(9, 11, 0, 0.4f);
	AddAnimation(_a, NPC::EA_WALK_UP);
}

void NPC::SetDefaultView(Vector2 view)
{
	InitViewer(view.x , view.y);
	m_viewY = view.y;
}

void NPC::SetPossesion(bool state)
{
	m_bPossesion = state;
}

static const double S_ALERT_HOSTILE = 4.0f;
bool NPC::Update(double dt, TileMap* _map)
{
	static const int S_MIN_VIEW_DISTANCE = 1;
	
	ChangeAnimation(dt);

	if (GetActive() == false)
	{
		return false;
	}

	Character::Update();	
	if (m_transforms.Translation.x < 0 || m_transforms.Translation.x > _map->GetScreenSize().x || m_transforms.Translation.y < 0 || m_transforms.Translation.y > _map->GetScreenSize().y)
	{
		// Do not update viewbox
	}
	else
	{
		ClearViewBox(this, _map);

		//update view distance according to alert level
		InitViewer(S_MIN_VIEW_DISTANCE, m_alertLevel + m_viewY);

		// Update FOV
		CreateViewBox(this, _map);
	}

	// Possession
	if(m_bPossesion == false && m_enemyState == ES_POSSESED)
	{
		m_enemyState = ES_KNOCKOUT;
	}

	switch (m_enemyState)
	{
		case ES_PATROL: //TL;DR : Walking to specified points
		{
			if (MoveTo(m_pathWay[m_pathPointCounter], _map, dt))
			{
				if (m_pathPointCounter < m_pathWay.size() - 1)
				{
					m_pathPointCounter++;
					//set look direction towards next target location base off current tile location on map
					SetMoveToDist(m_pathWay[m_pathPointCounter],_map->GetTileSize());
					ChangeAnimation(dt);
				}
				else
				{
					m_pathPointCounter = 0;
					//set look direction towards next target location base off current tile location on map
					SetMoveToDist(m_pathWay[m_pathPointCounter],_map->GetTileSize() );
					ChangeAnimation(dt);
				}
			}
			break;
		}
	case ES_SPOTTED: //TL;DR : If player walked into current enemy view
		{
			//code is mainly in spottedtarget : when player is in enemy view
			break;
		}
	case ES_ATTACK:
		{
			if(!AttackingInView(_player) )
			{
				m_enemyState = ES_SCAN;
			}
			else // Killed player
			{
				m_AttackCountdown = 0;
				return true;
			}
			break;
		}
	case ES_GOSTAN:
		{
			// If stack is empty, means we finally reached back
			if (m_possessedTourStops.size() == 0)
			{
				// Reached back the hijacked position
				m_enemyState = ES_PATROL;

				// Recovered from the previous possession by gostaning
				m_wasPossessed = false;
			}
			else
			{
				// Check if finished moving to the top position
				if (MoveTo(m_possessedTourStops.top(), _map, dt))
				{
					SetMoveToDist(m_possessedTourStops.top(), _map->GetTileSize() );
					std::cout << m_possessedTourStops.top() << std::endl;
					// If reached, then remove the top so that the next time it checks the top to move to, it is the next position
					m_possessedTourStops.pop();

				}
			}
			break;
		}
	case ES_POSSESED:
		{
			// Remember that I was possesed so that I can know to gostan later 
			m_wasPossessed = true;

			//Check who is being possessed and what they do
			if (m_npcType == NT_ENEMY)
			{
				// Store the tour route
				// -- Calculate the current tile position
				Vector2 tilePos = GetMapTilePos();

				// Only store this tile position if the previous was not the same or if there was no previous
				if ((m_possessedTourStops.size() > 0 && m_possessedTourStops.top() != tilePos) || m_possessedTourStops.size() == 0)
				{
					m_possessedTourStops.push(tilePos);
				}
			}
			else if (m_npcType == NT_TARGET)
			{
				SetActive(false);
				ClearViewBox(this, _map);
				return false;
			}
			
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
				m_lookDir = Direction::DIRECTIONS[Direction::DIR_RIGHT];
			}
			else if (m_checkAround >= S_WAIT_TIME * 2 && m_checkAround < S_WAIT_TIME * 3)
			{
				m_lookDir = Direction::DIRECTIONS[Direction::DIR_DOWN];
			}
			else if (m_checkAround >= S_WAIT_TIME * 3 && m_checkAround < S_WAIT_TIME * 4)
			{
				m_lookDir = Direction::DIRECTIONS[Direction::DIR_LEFT];
			}
			else if (m_checkAround >= S_WAIT_TIME * 4) //timing is over
			{
				if(m_wasPossessed) // if no need to trace back
				{
					//retracing steps back
					m_checkAround = 0;
					m_enemyState = ES_GOSTAN;
					if(m_possessedTourStops.size() > 0)
					{
						m_possessedTourStops.pop();
						SetMoveToDist(m_possessedTourStops.top() , _map->GetTileSize() );
					}
				}
				else
				{
					// contnue back to patrol
					m_checkAround = 0;
					m_enemyState = ES_PATROL;
					SetMoveToDist(m_pathWay[m_pathPointCounter], _map->GetTileSize());
				}
			}
			m_checkAround += dt;

			break;
		}
	case ES_KNOCKOUT: //TL;DR: enemy not alert, no reaction if player walk infront
		{
			//just reusing variable - original use is for SCANING
			m_checkAround += dt;
			if( m_checkAround > 4)
			{
				m_checkAround = 0;
				m_enemyState = ES_SCAN;
			}
		}
	}

	//if have seen player 
	if(m_AttackCountdown > 0)
	{
		//reset[last seen] timer starts
		m_ViewingTimer += dt;
		//if not in view and reset timer hits
		if(m_ViewingTimer >= 2)
		{
			m_AttackCountdown = 0.0;
			m_ViewingTimer = 0.0;
			m_enemyState = ES_SCAN;
			m_ViewingTimer = 0;
			m_AttackCountdown = 0;
		}
	}
	SetMapPosition(GetMapPos(), _map->GetScrollOffset(), _map->GetTileSize());
	ChangeAnimation(dt);
	return false;
}

bool NPC::AttackingInView(Character* _go)
{
	//attacked player - target character
	if(_go->GetHealth() > 0 )
	{
		_go->AddToHealth(-1);
		return true;
	}
	return false;
}

void NPC::SetNPCType(E_NPC_TYPE type)
{
	m_npcType = type;
}

NPC::E_NPC_TYPE NPC::GetNPCType(void)
{
	return m_npcType;
}

void NPC::SetPlayerPtr(Character* _ptr)
{
	_player = _ptr;
}

void NPC::AddAnimation(Animation* _anim, E_ENEMY_ACTION enemyState)
{
	m__animationList[enemyState] = _anim;
}

void NPC::ChangeAnimation(double dt)
{
	//NOT WALKING
	if (m_enemyState == ES_POSSESED || m_enemyState == ES_SCAN || m_enemyState == ES_SPOTTED 
		|| m_enemyState == ES_ATTACK ||m_enemyState == ES_KNOCKOUT)
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
	//WALKING
	else if (m_enemyState == ES_POSSESED  ||  m_enemyState == ES_PATROL ||m_enemyState == ES_GOSTAN)
	{
		if (m_lookDir == Direction::DIRECTIONS[Direction::DIR_LEFT])
		{
			m_enemyAction = EA_WALK_LEFT;
			//cout << "LEFT" << endl;
		}
		else if (m_lookDir == Direction::DIRECTIONS[Direction::DIR_RIGHT])
		{
			m_enemyAction = EA_WALK_RIGHT;
			//cout << "RIGHT" << endl;
		}
		if (m_lookDir == Direction::DIRECTIONS[Direction::DIR_DOWN])
		{
			m_enemyAction = EA_WALK_DOWN;
			//cout << "DOWN" << endl;
		}
		else if (m_lookDir == Direction::DIRECTIONS[Direction::DIR_UP])
		{
			m_enemyAction = EA_WALK_UP;
			//cout << "UP" << endl;
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
void NPC::ForceSetEnemyState(E_ENEMY_STATE enemyState)
{
	m_enemyState = enemyState;
}

void NPC::AddPatrolPoint(Vector2 pos)
{
	m_pathWay.push_back(pos);
}

vector<Vector2> NPC::GetPathWay(void)const
{
	return m_pathWay;
}

//return true if reached 
bool NPC::MoveTo(Vector2 EndPos, TileMap* _map, double dt)
{
	static const float S_MOVE_SPEED = 100.f;
	Vector2 s_newOrigin;
	bool shiftOrigin = false;
	float tileSize = _map->GetTileSize();
	if (m_lookDir.x < 0 || m_lookDir.y < 0) // Moving left or down
	{
		s_newOrigin = GetMapPos(); // No change in origin (Bottom or Left)
	}
	else // Moving right or up
	{
		s_newOrigin = GetMapPos() + (m_lookDir * tileSize); // Change in origin (Top or Right)
		shiftOrigin = true;
	}
	Vector2 newPos = s_newOrigin + (m_lookDir * S_MOVE_SPEED * dt); // New position if move
	if (_map->CheckCollision(newPos) || (m_moveDist + (S_MOVE_SPEED * dt)) >= m_moveToDist )
	{
		Vector2 tilePos;
		if (shiftOrigin)
		{
			s_newOrigin -= m_lookDir/* * 0.5*/; // Shift origin to half the distance between new and original origin
		}
		if (m_lookDir.x == 0) // Moving along y axis, snap y axis and ignore x axis
		{
			tilePos = Vector2(s_newOrigin.x, floor(s_newOrigin.y / tileSize) * tileSize); // Position to snap to by axis
			if (tilePos.y >= _map->GetMapSize().y)
			{
				tilePos.y -= _map->GetTileSize();
			}
			else if (tilePos.y < 0)
			{
				tilePos.y = 0;
			}
		}
		else if (GetLookDir().y == 0) // Moving along x axis, snap x axis and ignore y axis
		{
			tilePos = Vector2(floor(s_newOrigin.x / tileSize) * tileSize, s_newOrigin.y); // Position to snap to by axis
			if (tilePos.x >= _map->GetMapSize().x)
			{
				tilePos.x -= _map->GetTileSize();
			}
			else if (tilePos.x < 0)
			{
				tilePos.x = 0;
			}
		}
		SetMapPosition(tilePos, _map->GetScrollOffset(), _map->GetTileSize()); // Snap player to wall

		m_moveDist = 0.f;
		return true;
	}
	else
	{
		if (shiftOrigin) // Moving right or up
		{
			newPos -= GetLookDir() * _map->GetTileSize();
		}
		SetMapPosition(newPos, _map->GetScrollOffset(), _map->GetTileSize()); // Remove tile size that was added previously
		m_moveDist += S_MOVE_SPEED * dt;
	}

	return false;

}


void NPC::SetAlertLevel(int alertlevel)
{
	this->m_alertLevel = alertlevel;
}

int NPC::GetAlertLevel(void)
{
	return this->m_alertLevel;
}

//if player is in view
void NPC::SpottedTarget(Vector2 pos, float &alertLevel, double dt)
{
	//if current enemy is has not been possessed recently
	if(m_enemyState != ES_KNOCKOUT)
	{
		//increase alert level
		alertLevel += dt;
		//if havent attack yet
		if(m_AttackCountdown <= 2)
		{
			//go to spotting mode
			m_enemyState = ES_SPOTTED;
		}
		//if looking at enemy
		m_AttackCountdown += dt;
		//if reached time limit	  and			alert level reached
		if(m_AttackCountdown >= 2 && m_alertLevel >= S_ALERT_HOSTILE)
		{
			m_enemyState = ES_ATTACK;
		}
	}
}

Vector2 NPC::viewer_GetTilePos(void)
{
	return GetMapTilePos();		
}

Vector2 NPC::viewer_GetDirection(void)
{
	return m_lookDir;
}
