#include "MVC_Model_Spectre.h"


MVC_Model_Spectre::MVC_Model_Spectre(string configSONFile) : MVC_Model(configSONFile), _test(NULL)
{
	m_player = Player::GetInstance();
	m_player->Init();
}


MVC_Model_Spectre::~MVC_Model_Spectre(void)
{
}

static char s_g_actionKey = NULL;
void MVC_Model_Spectre::processKeyAction(double dt)
{
	if(m_bKeyPressed[MOVE_FORWARD_KEY]) //'W'
	{
		//queues player action 
		m_player->SetActions(m_player->PA_MOVE_UP, true);
	}
	 if(m_bKeyPressed[MOVE_BACKWARD_KEY])//'S'
	{
		m_player->SetActions(m_player->PA_MOVE_DOWN, true);
	}
	if(m_bKeyPressed[MOVE_LEFT_KEY] ) //'A'
	{
		m_player->SetActions(m_player->PA_MOVE_LEFT, true);
	}
	if(m_bKeyPressed[MOVE_RIGHT_KEY] ) //'D'
	{
		m_player->SetActions(m_player->PA_MOVE_RIGHT, true);
	}
	if(m_bKeyPressed[INTERACT_NEXT_KEY]) // 'E'
	{
		m_player->SetActions(m_player->PA_INTERACT, true);
	}
	//updates player depending on actions queued.
	m_player->Update(dt, _test,m_viewWidth, m_viewHeight );

	// Quitting the game
	if (m_bKeyPressed[GAME_EXIT_KEY])
	{
		// TODO: Open a pause menu and then quit by that instead. Do actual pausing or return to menus
		m_running = false;
	}
}

void MVC_Model_Spectre::Init(void)
{
	MVC_Model::Init();

	m__testGO = new GameObject2D;
	m__testGO->SetMesh(GetMeshResource("Quad"));
	m__testGO->SetPos(Vector2(100.0f, m_viewHeight * 0.5));
	m__testGO->SetScale(Vector2(50.0f, 50.0f));
}

void MVC_Model_Spectre::Update(double dt)
{
	MVC_Model::Update(dt);

	Vector3 pos = m__testGO->GetTransform().Translation;

	pos += Vector3(50.0f * dt);

	m__testGO->SetPos(pos);

	m_renderList2D.push(m__testGO);
}

void MVC_Model_Spectre::Exit(void)
{
	if (m__testGO)
	{
		delete m__testGO;
		m__testGO = NULL;
	}

	MVC_Model::Exit();
}