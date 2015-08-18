#include "Player.h"

Player* Player::instances = NULL;

Player::Player(void)
{
}

void Player::Init(void)
{
	m_currentState = E_PLAYER_STATE::PS_IDLE;
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

void Player::Update(double dt, TileMap* _map, E_PLAYER_ACTION movementKey)
{
	Character::Update();
		switch (movementKey)
		{
		//case E_PLAYER_ACTION::PA_MOVE_UP:
		//	{
		//		//move northward
		//		m_currentState = PS_WALK;
		//		//update direction looking at
		//		SetLookDir(Vector2(0,1) );
		//		//check if position at next frame is free
		//		if(_map->CheckCollision(m_transforms.Translation + )
		//		{
		//			//moving
		//			SetPos(GetPos() + GetLookDir() * dt);
		//		}
		//		break;
		//	}
		//case  E_PLAYER_ACTION::PA_MOVE_DOWN:
		//	{
		//		//move south
		//		m_currentState = PS_WALK;
		//		SetLookDir(Vector2(0,-1) );
		//		if(_map->CheckCollision(GetPos() + GetLookDir() * dt) )
		//		{
		//			SetPos(GetPos() + GetLookDir() * dt);
		//		}
		//		break;
		//	}
		//case  E_PLAYER_ACTION::PA_MOVE_LEFT:
		//	{
		//		//move left
		//		m_currentState = PS_WALK;
		//		SetLookDir(Vector2(-1,0) );
		//		if(_map->CheckCollision(GetPos() + GetLookDir() * dt) )
		//		{
		//			SetPos(GetPos() + GetLookDir() * dt);
		//		}
		//		break;
		//	}
		//case  E_PLAYER_ACTION::PA_MOVE_RIGHT:
		//	{
		//		//move right
		//		m_currentState = PS_WALK;
		//		SetLookDir(Vector2(1,0) );
		//		if(_map->CheckCollision(GetPos() + GetLookDir() * dt) )
		//		{
		//			SetPos(GetPos() + GetLookDir() * dt);
		//		}
		//		break;
		//	}
		default:
			{
				//if not moving
				m_currentState = PS_IDLE;
				break;
			}
		};
}

void Player::SetState(Player::E_PLAYER_STATE currentState)
{
	this->m_currentState = currentState;
}

Player::E_PLAYER_STATE Player::GetState(void) const
{
	return this->m_currentState;
}

void Player::ConstrainPlayer(TileMap* _map)
{

}