#include "MVC_Model_Zombie.h"

MVC_Model_Zombie::MVC_Model_Zombie(string configSONFile) : MVC_Model_3D(configSONFile)
{

}

MVC_Model_Zombie::~MVC_Model_Zombie()
{
}

void MVC_Model_Zombie::Init(void)
{
	MVC_Model_3D::Init();

	// Initialize ParticleEmitter
	PhysicsObject* particle = new PhysicsObject(GetMeshResource("Ember"));
	particle->SetBillboard(true);
	particle->SetFogged(false);
	particle->SetLighted(true);
	particle->SetLifetime(7.5f);
	particle->SetScale(Vector3(0.5f, 0.1f, 0.5f));
	particle->SetRot(Vector3(90.0f, 0.0f, 0.0f));

	Transform diff;
	diff.Scale.Set(-0.05f, -0.05f, -0.05f);

	m_smoker.Init(Vector3(400.0f, 180.0f, -750.0f), particle, Vector3(-2.0f, 5.0f, -2.0f), Vector3(2.0f, 12.0f, 2.0f), 70);
	m_smoker.SetDiff(diff);

	// Initialize Car
	m_car = new GameObject(GetMeshResource("Car"));
	m_car->SetPos(Vector3(600.0f, 180.0f, -825.0f));
	m_car->SetRot(Vector3(0.0f, 180.0f));
	m_car->SetScale(Vector3(1.5f, 1.5f, 1.5f));

	// Set position of the fps count
	m_fpsCount->SetPos(Vector3());
}

void MVC_Model_Zombie::Update(double dt)
{
	MVC_Model::Update(dt);

	m_smoker.Update(dt);

	// Car Update
	Vector3 carStartPos(600.0f, 180.0f, -3000.0f);
	Vector3 carEndPos(600.0f, 180.0f, 3000.0f);
	const Vector3 carVel(0.0f, 0.0f, 320.0f);

	Vector3 carPos = m_car->GetTransform().Translation;

	if (carPos.z > carEndPos.z)
	{
		carPos.z = carStartPos.z;
	}

	m_car->SetPos(carPos + carVel * dt);


	// Push GameObjects into the Render List
	m_renderList3D = goList;
	m_renderList3D.push_back(m_car);

	// Render the FPS Count
	m_renderList2D.push_back(m_fpsCount);

	// -- Push particles into the renderList
	const vector<PhysicsObject*> particles = m_smoker.GetParticles();
	for (vector<PhysicsObject*>::const_iterator it = particles.begin(); it != particles.end(); ++it)
	{
		if ((*it)->IsActive())
		{
			m_renderList3D.push_back(*it);
		}
	}
}

void MVC_Model_Zombie::Exit(void)
{
	MVC_Model::Exit();

	if (m_car != NULL)
	{
		delete m_car;
		m_car = NULL;
	}
}

void MVC_Model_Zombie::processKeyAction(double dt)
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
}
