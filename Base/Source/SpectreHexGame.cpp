#include "SpectreHexGame.h"

const float SpectreHexGame::MIN_BALL_RADIUS = 20.0f;
const Vector2 SpectreHexGame::MIN_BALL_SCALE(MIN_BALL_RADIUS, MIN_BALL_RADIUS);
const float SpectreHexGame::MIN_BALL_MASS = 1.0f;
const float SpectreHexGame::PLAYER_BALL_MULTIPLIER = 1.0f;
const float SpectreHexGame::PLAYER_MOVE_FORCE = 300.0f;
const float SpectreHexGame::WALL_THICKNESS = 50.0f;

SpectreHexGame::SpectreHexGame()
	: m__player(NULL)
	, m_state(GS_PLAYING)
{
}


SpectreHexGame::~SpectreHexGame()
{
}

void SpectreHexGame::Init(Mesh* _shadowBallMesh, Mesh* _circuitWallMesh, Mesh* _bgMesh, int viewWidth, int viewHeight)
{
	// Init Player
	if (m__player == NULL)
	{
		m__player = fetchObject();
		m__player->SetPos(Vector3(100.0f, (viewHeight - MIN_BALL_RADIUS * PLAYER_BALL_MULTIPLIER) * 0.5));
		m__player->SetScale(MIN_BALL_SCALE);
		m__player->InitPhysics2D(MIN_BALL_MASS* PLAYER_BALL_MULTIPLIER, false);
		m__player->SetMass(MIN_BALL_MASS * PLAYER_BALL_MULTIPLIER);
		m__player->SetMesh(_shadowBallMesh);
	}

	// Generate the Walls
	// -- Left
	PhysicalObject* wall = fetchObject();
	wall->SetPos(Vector3(WALL_THICKNESS * 0.5, viewHeight * 0.5f));
	wall->SetScale(Vector2(WALL_THICKNESS, viewHeight));
	wall->InitPhysics2D(1.0f, true, Vector2::ZERO_VECTOR, Vector2(1.0f, 0.0f));
	wall->SetMesh(_circuitWallMesh);

	//// Top
	//wall = fetchObject();
	//wall->SetPos(Vector3(viewWidth * 0.5f, viewHeight - WALL_THICKNESS * 0.5));
	//wall->SetScale(Vector2(WALL_THICKNESS, viewWidth));
	//wall->InitPhysics2D(1.0f, true, Vector2::ZERO_VECTOR, Vector2(0.0f, 1.0f));
	//wall->SetMesh(_circuitWallMesh);

	//// Bottom
	//wall = fetchObject();
	//wall->SetPos(Vector3(viewWidth * 0.5f, WALL_THICKNESS * 0.5));
	//wall->SetScale(Vector2(WALL_THICKNESS, viewWidth));
	//wall->InitPhysics2D(1.0f, true, Vector2::ZERO_VECTOR, Vector2(0.0f, 1.0f));
	//wall->SetMesh(_circuitWallMesh);

	// Generate balls
	for (int ball = 0; ball < 1; ++ball)
	{
		PhysicalObject* sBall = fetchObject();
		sBall->SetPos(Vector3(200.0f, (viewHeight - MIN_BALL_RADIUS) * 0.5));
		sBall->SetScale(MIN_BALL_SCALE);
		sBall->InitPhysics2D(MIN_BALL_MASS, false);
		sBall->SetMesh(_shadowBallMesh);
	}

	// Generate BG
	m__background = new GameObject2D;
	m__background->SetMesh(_bgMesh);
	m__background->SetPos(Vector2::ZERO_VECTOR);
	m__background->SetScale(Vector2(viewWidth, viewHeight));
}

void SpectreHexGame::Update(double dt)
{
	switch (m_state)
	{
		case GS_START:
			startUpdate(dt);
			break;
		case GS_PLAYING:
			ballsUpdate(dt);
			break;
	}
}

void SpectreHexGame::Exit(void)
{
	// Delete balls
	while (m_ballList.size() > 0)
	{
		if (m_ballList.back() != NULL)
		{
			delete m_ballList.back();
			m_ballList.pop_back();
		}
	}
}

void SpectreHexGame::Move(bool left, bool right, bool up, bool down, double dt)
{
	// Don't attempt to move 
	if (m__player == NULL || m_state != GS_PLAYING)
	{
		return;
	}

	Vector2 resultantForce;

	// Left
	if (left)
	{
		resultantForce.x -= PLAYER_MOVE_FORCE * dt;
	}
	// Right
	else if (right)
	{
		resultantForce.x += PLAYER_MOVE_FORCE * dt;
	}

	// Down
	if (down)
	{
		resultantForce.y -= PLAYER_MOVE_FORCE * dt;
	}
	// Up
	else if (up)
	{
		resultantForce.y += PLAYER_MOVE_FORCE * dt;
	}

	m__player->AddForce(resultantForce, dt);
}

vector<GameObject2D*> SpectreHexGame::GetRenderObjects(void)
{
	vector<GameObject2D*> renderList;

	renderList.push_back(m__background);

	for (vector<PhysicalObject*>::iterator ball = m_ballList.begin(); ball != m_ballList.end(); ++ball)
	{
		if ((*ball)->GetActive())
		{
			renderList.push_back(*ball);
		}
	}

	return renderList;
}

PhysicalObject* SpectreHexGame::fetchObject(void)
{
	// Retrieve a ball
	for (vector<PhysicalObject*>::iterator ball = m_ballList.begin(); ball != m_ballList.end(); ++ball)
	{
		if (!(*ball)->GetActive())
		{
			(*ball)->SetActive(true);
			return *ball;
		}
	}

	// Generate some if unavailable
	const int BATCH_MAKE = 10;
	for (size_t ball = 0; ball < BATCH_MAKE; ++ball)
	{
		PhysicalObject* sBall = new PhysicalObject();
		sBall->SetActive(false);
		m_ballList.push_back(sBall);
	}

	m_ballList.back()->SetActive(true);

	return m_ballList.back();
}

void SpectreHexGame::startUpdate(double dt)
{
	// Forces
	static const Vector2 INITIAL_PUSH_PLAYER(2.0f);
	static const float INITIAL_PUSH_X = 5.0f;

	// Timers
	static double s_timer = 0.0f;				// Timer for the balls to be introduced with the introduction shot
	static const double TIME_LIMIT = 1.0;		// The time to wait before players can start playing

	s_timer += dt;

	if (s_timer > TIME_LIMIT)
	{
		// Stop the shooting
		m_state = GS_PLAYING;
	}

	/* 
	 * Shoot the shadow balls in (entrance)
	 */
	// For producing a spread of balls
	const float OFFSET_Y_PER_SHOT = INITIAL_PUSH_X / MAX_BALLS;
	float offsetY = -(OFFSET_Y_PER_SHOT * MAX_BALLS * 0.5);

	for (vector<PhysicalObject*>::iterator phyObj = m_ballList.begin(); phyObj != m_ballList.end(); ++phyObj)
	{
		PhysicalObject* po = static_cast<PhysicalObject *>(*phyObj);

		if (!po->GetActive())
		{
			continue;
		}
		
		// Update this Physical
		po->UpdatePhysics(dt);

		for (std::vector<PhysicalObject *>::iterator phyObj2 = phyObj + 1; phyObj2 != m_ballList.end(); ++phyObj2)
		{
			PhysicalObject *po2 = static_cast<PhysicalObject *>(*phyObj2);

			PhysicalObject* poA = po;
			PhysicalObject* poB = po2;

			if (po->GetNormal() != Vector2::ZERO_VECTOR)
			{
				if (po2->GetNormal() != Vector2::ZERO_VECTOR)
				{
					continue;
				}

				poA = po2;
				poB = po;
			}

			if (poA->CollideWith(poB, dt))
			{
				poA->CollideRespondTo(poB);
			}
		}
	}
}

void SpectreHexGame::ballsUpdate(double dt)
{
	for (vector<PhysicalObject*>::iterator phyObj = m_ballList.begin(); phyObj != m_ballList.end(); ++phyObj)
	{
		PhysicalObject* po = static_cast<PhysicalObject *>(*phyObj);

		if (!po->GetActive())
		{
			continue;
		}

		// Update this Physical
		po->UpdatePhysics(dt);

		for (std::vector<PhysicalObject *>::iterator phyObj2 = phyObj + 1; phyObj2 != m_ballList.end(); ++phyObj2)
		{
			PhysicalObject *po2 = static_cast<PhysicalObject *>(*phyObj2);

			PhysicalObject* poA = po;
			PhysicalObject* poB = po2;

			if (po->GetNormal() != Vector2::ZERO_VECTOR)
			{
				if (po2->GetNormal() != Vector2::ZERO_VECTOR)
				{
					continue;
				}

				poA = po2;
				poB = po;
			}

			if (poA->CollideWith(poB, dt))
			{
				poA->CollideRespondTo(poB);
			}
		}
	}

	/*
	for (vector<PhysicalObject*>::iterator ball = m_ballList.begin(); ball != m_ballList.end(); ++ball)
	{
		PhysicalObject* sBall = *ball;

		if (!sBall->GetActive() || sBall->GetNormal() != Vector2::ZERO_VECTOR)
		{
			continue;
		}
		else
		{
			sBall->UpdatePhysics(dt);
		}

		for (vector<PhysicalObject*>::iterator ball2 = ball + 1; ball2 != m_ballList.end(); ++ball2)
		{
			PhysicalObject* sBall2 = *ball2;

			if (!sBall2->GetActive())
			{
				continue;
			}

			// Collision Response
			if (sBall == m__player)
			{
				// Calculate Merged Size
				Vector3 playerScale = m__player->GetTransform().Scale + sBall2->GetTransform().Scale;

				// Set Merged Mass
				m__player->SetMass(m__player->GetMass() + sBall2->GetMass());
				// Set Merged Size
				//m__player->SetScale(playerScale);

				// Deactivate the other ball
				sBall2->SetActive(false);
			}
			else if (sBall->CollideWith(sBall2, dt))
			{
				sBall->CollideRespondTo(sBall2);
			}
		}
	}
	*/
}
