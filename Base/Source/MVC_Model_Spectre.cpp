#include "MVC_Model_Spectre.h"


MVC_Model_Spectre::MVC_Model_Spectre(string configSONFile) : MVC_Model(configSONFile), _test(NULL)
{
	_player = Player::GetInstance();
	_player->Init();
}


MVC_Model_Spectre::~MVC_Model_Spectre(void)
{
}

static char s_g_actionKey = NULL;
void MVC_Model_Spectre::processKeyAction(double dt)
{
	if(m_bKeyPressed[MOVE_FORWARD_KEY])
	{
		_player->Update(dt, _test, _player->PA_MOVE_UP);
	}
	else if(m_bKeyPressed[MOVE_BACKWARD_KEY])
	{
		_player->Update(dt, _test, _player->PA_MOVE_DOWN);
	}
	else if(m_bKeyPressed[MOVE_LEFT_KEY] )
	{
		_player->Update(dt, _test, _player->PA_MOVE_LEFT);
	}
	else if(m_bKeyPressed[MOVE_RIGHT_KEY] )
	{
		_player->Update(dt, _test, _player->PA_MOVE_RIGHT);
	}
}

void MVC_Model_Spectre::Init()
{
	MVC_Model::Init();
}

void MVC_Model_Spectre::Update(double dt)
{
	MVC_Model::Update(dt);
	processKeyAction(dt);
}

void MVC_Model_Spectre::Exit()
{
	MVC_Model::Exit();
}