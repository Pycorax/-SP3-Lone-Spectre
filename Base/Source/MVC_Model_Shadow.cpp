#include "MVC_Model_Shadow.h"

MVC_Model_Shadow::MVC_Model_Shadow(string configSONFile) : MVC_Model_3D(configSONFile)
{

}

MVC_Model_Shadow::~MVC_Model_Shadow()
{
}

void MVC_Model_Shadow::Init(void)
{
	MVC_Model_3D::Init();

	Mesh* lightDepthQuadMesh = GetMeshResource("LightDepthQuad");

	GameObject* go = new GameObject(lightDepthQuadMesh, false);
	go->SetPos(Vector3(700.0f, 500.0f, 0.0f));
	go->SetScale(Vector3(1.0f, 1.0f, 1.0f));
	go->SetBillboard(false);
	go->SetLighted(false);

	m_lightDepthQuad = go;
}

void MVC_Model_Shadow::Update(double dt)
{
	MVC_Model::Update(dt);

	// Push GameObjects into the Render List
	m_renderList3D = goList;

	// Render the FPS Count
	m_renderList2D.push_back(m_fpsCount);
	m_renderList2D.push_back(m_lightDepthQuad);
}

void MVC_Model_Shadow::Exit(void)
{
	MVC_Model::Exit();

	delete m_lightDepthQuad;
}

void MVC_Model_Shadow::processKeyAction(double dt)
{
	// Camera Movement
	if (m_bKeyPressed[MOVE_FORWARD_KEY])
	{
		m_camera->UpdateStatus('w');
	}
	if (m_bKeyPressed[MOVE_BACKWARD_KEY])
	{
		m_camera->UpdateStatus('s');
	}
	if (m_bKeyPressed[MOVE_LEFT_KEY])
	{
		m_camera->UpdateStatus('a');
	}
	if (m_bKeyPressed[MOVE_RIGHT_KEY])
	{
		m_camera->UpdateStatus('d');
	}

	// Camera Look
	if (m_bKeyPressed[LOOK_UP_KEY])
	{
		m_camera->UpdateStatus('i');
	}
	if (m_bKeyPressed[LOOK_DOWN_KEY])
	{
		m_camera->UpdateStatus('k');
	}
	if (m_bKeyPressed[LOOK_LEFT_KEY])
	{
		m_camera->UpdateStatus('j');
	}
	if (m_bKeyPressed[LOOK_RIGHT_KEY])
	{
		m_camera->UpdateStatus('l');
	}

	// Special Camera Movements
	if (m_bKeyPressed[MOVE_JUMP_KEY])
	{
		m_camera->UpdateStatus(' ');
	}
	else if (m_bKeyPressed[MOVE_CROUCH_KEY])
	{
		m_camera->UpdateStatus('c');
	}
	else if (m_bKeyPressed[MOVE_PRONE_KEY])
	{
		m_camera->UpdateStatus('z');
	}
	else if (m_bKeyPressed[MOVE_SPRINT_KEY])
	{
		m_camera->UpdateStatus('x');
	}

	// Camera Switching
	if (m_bKeyPressed[CAMERA_FP_KEY])
	{
		m_camera->UpdateStatus(0);
	}
	else if (m_bKeyPressed[CAMERA_FP_NOCLIP_KEY])
	{
		m_camera->UpdateStatus(1);
	}

	// Debug Lighting
	static const float speed = 20;

	if (m_bKeyPressed[MISC_1_KEY])
	{
		m_lights[0].position.x -= speed * static_cast<float>(dt);
	}
	if (m_bKeyPressed[MISC_2_KEY])
	{
		m_lights[0].position.z += speed * static_cast<float>(dt);
	}
	if (m_bKeyPressed[MISC_3_KEY])
	{
		m_lights[0].position.z -= speed * static_cast<float>(dt);
	}
	if (m_bKeyPressed[MISC_4_KEY])
	{
		m_lights[0].position.x += speed * static_cast<float>(dt);
	}
	if (m_bKeyPressed[MISC_5_KEY])
	{
		m_lights[0].position.y+= speed * static_cast<float>(dt);
	}
	if (m_bKeyPressed[MISC_6_KEY])
	{
		m_lights[0].position.y -= speed * static_cast<float>(dt);
	}
}
