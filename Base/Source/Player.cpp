#include "Player.h"

Player* Player::instances = NULL;
float Player::s_playerMoveSpeed = 5.f;

Player::Player(void)
{
}

void Player::Init(void)
{
	m_currentState = E_PLAYER_STATE::PS_IDLE;
	//init the action queue to be all false
	for(int actionList = PA_IDLE; actionList != NUM_PLAYER_ACTIONS; actionList++)
	{
		E_PLAYER_ACTION s_actionList = static_cast<E_PLAYER_ACTION>(actionList);
		m_actions[s_actionList] = false;
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

	SetMapPosition(m_transforms.Translation, _map->GetScrollOffset());
	//TODO: **NOTE: factor in collision**
	if(m_actions[PA_MOVE_UP])
	{
		//update status
		m_currentState = PS_WALK;
		//update direction looking at
		SetLookDir(Vector2(0,1) );

		//centralise the player
		ConstrainPlayer(_map);

		if(!_map->CheckCollision(m_lookDir * s_playerMoveSpeed * dt + m_transforms.Translation + Vector2(0,_map->GetTileSize())) )
		{
			m_transforms.Translation += m_lookDir * s_playerMoveSpeed * dt;
		}

		// reseting back to false
		m_actions[PA_MOVE_UP] = false;
	}
	else if(m_actions[PA_MOVE_DOWN])
	{
		//update status
		m_currentState = PS_WALK;
		SetLookDir(Vector2(0,-1) );

		//centralise the player
		ConstrainPlayer(_map);

		if(!_map->CheckCollision(m_lookDir * s_playerMoveSpeed * dt + m_transforms.Translation) )
		{
			m_transforms.Translation += m_lookDir * s_playerMoveSpeed * dt;
		}

		// reseting back to false
		m_actions[PA_MOVE_DOWN] = false;
	}

	if(m_actions[PA_MOVE_LEFT])
	{
		//update status
		m_currentState = PS_WALK;
		SetLookDir(Vector2(-1,0) );

		//centralise the player
		ConstrainPlayer(_map);

		if(!_map->CheckCollision(m_lookDir * s_playerMoveSpeed * dt + m_transforms.Translation) )
		{
			m_transforms.Translation += m_lookDir * s_playerMoveSpeed * dt;
		}

		// reseting back to false
		m_actions[PA_MOVE_LEFT] = false;
	}
	else if(m_actions[PA_MOVE_RIGHT])
	{
		//update status
		m_currentState = PS_WALK;
		SetLookDir(Vector2(1,0) );

		//centralise the player
		ConstrainPlayer(_map);

		if(!_map->CheckCollision(m_lookDir * s_playerMoveSpeed * dt + m_transforms.Translation + Vector2(_map->GetTileSize(), 0)) )
		{
			m_transforms.Translation += m_lookDir * s_playerMoveSpeed * dt;
		}

		// reseting back to false
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

void Player::SetState(Player::E_PLAYER_STATE currentState)
{
	this->m_currentState = currentState;
}

Player::E_PLAYER_STATE Player::GetState(void) const
{
	return this->m_currentState;
}

void Player::ConstrainPlayer( TileMap* _map)
{
	//keep player within a small box at centre
	//constrain X axis
	if(m_transforms.Translation.x < 0)
	{
		m_transforms.Translation.x = 0;
	}
	else if(m_transforms.Translation.x > _map->GetMapSize().x - _map->GetTileSize() )
	{
		m_transforms.Translation = _map->GetMapSize().x - _map->GetTileSize();
	}

	if(m_transforms.Translation.y < 0)
	{
		m_transforms.Translation.y = 0;
	}
	else if(m_transforms.Translation.y > _map->GetMapSize().y - _map->GetTileSize() )
	{
		m_transforms.Translation =_map->GetMapSize().y - _map->GetTileSize();
	}
}
