#include "Player.h"

Player* Player::instances = NULL;

Player::Player(void)
	:m_currentState(E_PLAYER_STATE::PS_IDLE)
{
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

void Player::Update(double dt)
{
	Character::Update();
}

void Player::SetState(Player::E_PLAYER_STATE currentState)
{
	this->m_currentState = currentState;
}

Player::E_PLAYER_STATE Player::GetState(void) const
{
	return this->m_currentState;
}