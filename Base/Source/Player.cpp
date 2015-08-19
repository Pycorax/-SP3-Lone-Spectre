#include "Player.h"

Player* Player::instances = NULL;
float Player::s_playerMoveSpeed = 300.f;

Player::Player(void)
{
}

void Player::Init(Mesh* _mesh)
{
	SetMesh(_mesh);
	SetMapPosition(Vector2(512,400), Vector2(0,0));
	m_currentState = E_PLAYER_STATE::PS_IDLE;
	//init the action queue to be all false
	/*for(int actionList = PA_IDLE; actionList != NUM_PLAYER_ACTIONS; actionList++)
	{
		E_PLAYER_ACTION s_actionList = static_cast<E_PLAYER_ACTION>(actionList);
		m_actions[s_actionList] = false;
	}*/
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
	if (instances == NULL)
	{
		instances = new Player[S_MAX_INSTANCES];
	}

	if (instance >= 0 && instance < S_MAX_INSTANCES)
	{
		return instances + instance;
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
		moveUpDown(dt, false, _map);
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
		moveUpDown(dt, true, _map);
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
		moveLeftRight(dt, false, _map);
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
		moveLeftRight(dt, true, _map);
		m_actions[PA_MOVE_RIGHT] = false;
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
	
}

void Player::Clear()
{
	// Not working
	/*for (int player = 0; player < S_MAX_INSTANCES; ++player)
	{
		delete instances + player;
		instances + player = NULL;
	}*/
}

void Player::SetState(Player::E_PLAYER_STATE currentState)
{
	this->m_currentState = currentState;
}

Player::E_PLAYER_STATE Player::GetState(void) const
{
	return this->m_currentState;
}

bool Player::moveLeftRight(double dt, bool mode, TileMap* _map)
{
	if (mode) // Move right
	{
		Vector2 newPos = GetMapPos() + Vector2(_map->GetTileSize() + (s_playerMoveSpeed * dt)); // Add a tile size to shift origin point from left to right | New position if move
		if (_map->CheckCollision(newPos)) // Collided
		{
			Vector2 rightOrigin = GetMapPos() + Vector2(_map->GetTileSize()); // Shift player origin from left to right
			Vector2 tilePos = Vector2(floor(rightOrigin.x / _map->GetTileSize()) * _map->GetTileSize(), GetMapPos().y);
			if (tilePos.x >= _map->GetMapSize().x)
			{
				tilePos.x -= _map->GetTileSize();
			}
			_map->AddToScrollOffset(tilePos - GetMapPos()); // Change scroll offset
			SetMapPosition(tilePos, _map->GetScrollOffset()); // Snap player to wall
		}
		else // No collision
		{
			newPos -= Vector2(_map->GetTileSize());
			_map->AddToScrollOffset(Vector2(s_playerMoveSpeed * dt));
			SetMapPosition(newPos, _map->GetScrollOffset()); // Remove tile size that was added previously
		}
	}
	else // Move left
	{
		Vector2 newPos = GetMapPos() + Vector2(-s_playerMoveSpeed * dt); // New position if move
		if (_map->CheckCollision(newPos)) // Collided
		{
			Vector2 leftOrigin = GetMapPos(); // Shift player origin from left to right
			Vector2 tilePos = Vector2(floor(leftOrigin.x / _map->GetTileSize()) * _map->GetTileSize(), GetMapPos().y); // Round down decimal number to get tile position
			if (tilePos.x < 0)
			{
				tilePos.x = 0;
			}
			_map->AddToScrollOffset(tilePos - GetMapPos()); // Change scroll offset
			SetMapPosition(tilePos, _map->GetScrollOffset()); // Snap player to wall
		}
		else
		{
			_map->AddToScrollOffset(Vector2(-s_playerMoveSpeed * dt));
			SetMapPosition(newPos, _map->GetScrollOffset());
		}
	}
	return true;
}

bool Player::moveUpDown(double dt, bool mode, TileMap* _map)
{
	if (mode) // Move down
	{
		Vector2 newPos = GetMapPos() + Vector2(0, -s_playerMoveSpeed * dt); // New position if move
		if (_map->CheckCollision(newPos)) // Collided
		{
			Vector2 bottomOrigin = GetMapPos(); // Shift player origin from left to right
			Vector2 tilePos = Vector2(GetMapPos().x, floor(bottomOrigin.y / _map->GetTileSize()) * _map->GetTileSize()); // Round down decimal number to get tile position
			if (tilePos.y < 0)
			{
				tilePos.y = 0;
			}
			_map->AddToScrollOffset(tilePos - GetMapPos()); // Change scroll offset
			SetMapPosition(tilePos, _map->GetScrollOffset()); // Snap player to wall
		}
		else // No collision
		{
			_map->AddToScrollOffset(Vector2(0, -s_playerMoveSpeed * dt));
			SetMapPosition(newPos, _map->GetScrollOffset());
		}
	}
	else // Move up
	{
		Vector2 newPos = GetMapPos() + Vector2(0, _map->GetTileSize() + (s_playerMoveSpeed * dt)); // Add a tile size to shift origin point from bottom to top | New position if move 
		if (_map->CheckCollision(newPos)) // Collided
		{
			Vector2 topOrigin = GetMapPos() + Vector2(0, _map->GetTileSize()); // Shift player origin from left to right
			Vector2 tilePos = Vector2(GetMapPos().x, floor(topOrigin.y / _map->GetTileSize()) * _map->GetTileSize()); // Round down decimal number to get tile position
			if (tilePos.y >= _map->GetMapSize().y)
			{
				tilePos.y -= _map->GetTileSize();
			}
			_map->AddToScrollOffset(tilePos - GetMapPos()); // Change scroll offset
			SetMapPosition(tilePos, _map->GetScrollOffset()); // Snap player to wall
		}
		else
		{
			newPos -= Vector2(0, _map->GetTileSize());
			_map->AddToScrollOffset(Vector2(0, s_playerMoveSpeed * dt));
			SetMapPosition(newPos, _map->GetScrollOffset());
		}
	}
	return true;
}