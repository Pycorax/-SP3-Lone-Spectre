#include "Player.h"

Player* Player::s_instances = NULL;
float Player::s_playerMoveSpeed = 300.f;

Player::Player(void)
{
}

void Player::Init(Mesh* _mesh)
{
	SetMesh(_mesh);
	m_moving = false;
	m_moveDist = 0.f;
	m_currentState = E_PLAYER_STATE::PS_IDLE;
	//Init the action queue to be all false
	for (int i = 0; i < NUM_PLAYER_ACTIONS; ++i)
	{
		m_actions[i] = false;
	}
}

Player::~Player(void)
{

}

Player* Player::GetInstance(int instance)
{
	if (s_instances == NULL)
	{
		s_instances = new Player[S_MAX_INSTANCES];
	}

	if (instance >= 0 && instance < S_MAX_INSTANCES)
	{
		return s_instances + instance;
	}
	else
	{
		return NULL;
	}
}
//set Player action to desired state
void Player::SetActions(E_PLAYER_ACTION type,bool status)
{
	m_actions[type] = status;
}

void Player::Update(double dt, TileMap* _map)
{
	Character::Update();
	//TODO: **NOTE: factor in collision**
	if (!m_moving)
	{
		if(m_actions[PA_MOVE_UP])
		{
			/*//update status
			m_currentState = PS_WALK;
			//update direction looking at
			SetLookDir(Vector2(0,1) );

			//centralise the player
			constrainPlayer(_map);

			if(!_map->CheckCollision(m_lookDir * s_playerMoveSpeed * dt + m_transforms.Translation + Vector2(0,_map->GetTileSize())) )
			{
			m_transforms.Translation += m_lookDir * s_playerMoveSpeed * dt;
			}

			// reseting back to false
			m_actions[PA_MOVE_UP] = false;*/
			SetLookDir(Vector2(0, 1));
			m_moving = true;
			//moveUpDown(dt, false, _map);
			m_actions[PA_MOVE_UP] = false;
		}
		else if(m_actions[PA_MOVE_DOWN])
		{
			/*//update status
			m_currentState = PS_WALK;
			SetLookDir(Vector2(0,-1) );

			//centralise the player
			constrainPlayer(_map);

			if(!_map->CheckCollision(m_lookDir * s_playerMoveSpeed * dt + m_transforms.Translation) )
			{
			m_transforms.Translation += m_lookDir * s_playerMoveSpeed * dt;
			}

			// reseting back to false
			m_actions[PA_MOVE_DOWN] = false;*/
			SetLookDir(Vector2(0, -1));
			m_moving = true;
			//moveUpDown(dt, true, _map);
			m_actions[PA_MOVE_DOWN] = false;
		}

		if(m_actions[PA_MOVE_LEFT])
		{
			/*//update status
			m_currentState = PS_WALK;
			SetLookDir(Vector2(-1,0) );

			//centralise the player
			constrainPlayer(_map);

			if(!_map->CheckCollision(m_lookDir * s_playerMoveSpeed * dt + m_transforms.Translation) )
			{
			m_transforms.Translation += m_lookDir * s_playerMoveSpeed * dt;
			}

			// reseting back to false
			m_actions[PA_MOVE_LEFT] = false;*/
			SetLookDir(Vector2(-1));
			m_moving = true;
			//moveLeftRight(dt, false, _map);
			m_actions[PA_MOVE_LEFT] = false;
		}
		else if(m_actions[PA_MOVE_RIGHT])
		{
			/*//update status
			m_currentState = PS_WALK;
			SetLookDir(Vector2(1,0) );

			//centralise the player
			constrainPlayer(_map);

			if(!_map->CheckCollision(m_lookDir * s_playerMoveSpeed * dt + m_transforms.Translation + Vector2(_map->GetTileSize(), 0)) )
			{
			m_transforms.Translation += m_lookDir * s_playerMoveSpeed * dt;
			}

			// reseting back to false
			m_actions[PA_MOVE_RIGHT] = false;*/
			SetLookDir(Vector2(1));
			m_moving = true;
			//moveLeftRight(dt, true, _map);
			m_actions[PA_MOVE_RIGHT] = false;
		}
	}
	if(m_actions[PA_INTERACT])
	{
		Vector2 interactionDistance(m_lookDir * _map->GetTileSize()); 
		//TODO : Add in algorithm for determerning the type of action
		//		Host, Dive , Jump or Hex
		// if( lookDir == enemy back) -> host
		// if( lookDir == Item on map) ->dive
		// if( m_currentState == Dive || host && lookDir == enemyBack || item on map) -> jump then become dive ||host
		// if (lookDir == Camera ) -> Hex goto minigame
	
		// reseting back to false
		m_actions[PA_INTERACT] = false;
	}

	if (m_moving)
	{
		move(dt, _map);
	}
}

void Player::UpdateHost(double dt)
{
	static_cast <Enemy* >(m__host);
	//update host here
}

void Player::Clear()
{
	if (s_instances)
	{
		delete[] s_instances;
		s_instances = NULL;
	}
}

void Player::SetState(Player::E_PLAYER_STATE currentState)
{
	this->m_currentState = currentState;
}

Player::E_PLAYER_STATE Player::GetState(void) const
{
	return this->m_currentState;
}

void Player::move(double dt, TileMap* _map)
{
	Vector2 newOrigin;
	if (GetLookDir().x < 0 || GetLookDir().y < 0) // Moving left or down
	{
		newOrigin = GetMapPos(); // No change in origin (Bottom or Left)
	}
	else // Moving right or up
	{
		newOrigin = GetMapPos() + (GetLookDir() * _map->GetTileSize()); // Change in origin (Top or Right)
	}
	Vector2 newPos = newOrigin + (GetLookDir() * s_playerMoveSpeed * dt); // New position if move
	if (_map->CheckCollision(newPos) || (m_moveDist + (s_playerMoveSpeed * dt)) >= _map->GetTileSize()) // Collided or moving more than 1 tile
	{
		Vector2 tilePos;
		if (GetLookDir().x == 0) // Moving along y axis, snap y axis and ignore x axis
		{
			tilePos = Vector2(newOrigin.x, floor(newOrigin.y / _map->GetTileSize()) * _map->GetTileSize()); // Position to snap to by axis
			if (tilePos.y >= _map->GetMapSize().y)
			{
				tilePos.y -= _map->GetTileSize();
			}
			else if (tilePos.y < 0)
			{
				tilePos.y = 0;
			}
		}
		else // Moving along x axis, snap x axis and ignore y axis
		{
			tilePos = Vector2(floor(newOrigin.x / _map->GetTileSize()) * _map->GetTileSize(), newOrigin.y); // Position to snap to by axis
			if (tilePos.x >= _map->GetMapSize().x)
			{
				tilePos.x -= _map->GetTileSize();
			}
			else if (tilePos.x < 0)
			{
				tilePos.x = 0;
			}
		}
		_map->AddToScrollOffset(tilePos - GetMapPos()); // Change scroll offset
		SetMapPosition(tilePos, _map->GetScrollOffset()); // Snap player to wall
		resetMove();
	}
	else
	{
		if (GetLookDir().x >= 1 || GetLookDir().y >= 1) // Moving right or up
		{
			newPos -= GetLookDir() * _map->GetTileSize();
		}
		_map->AddToScrollOffset(GetLookDir() * s_playerMoveSpeed * dt);
		SetMapPosition(newPos, _map->GetScrollOffset()); // Remove tile size that was added previously
		m_moveDist += s_playerMoveSpeed * dt;
	}
}

void Player::resetMove()
{
	m_moving = false;
	m_moveDist = 0.f;
}