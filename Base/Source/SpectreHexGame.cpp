#include "SpectreHexGame.h"

const float SpectreHexGame::MIN_BALL_RADIUS = 20.0f;
const Vector2 SpectreHexGame::MIN_BALL_SCALE(MIN_BALL_RADIUS, MIN_BALL_RADIUS);
const float SpectreHexGame::MIN_BALL_MASS = 0.01f;
const float SpectreHexGame::PLAYER_BALL_MULTIPLIER = 1.0f;
const float SpectreHexGame::PLAYER_MOVE_FORCE = 500.0f;
const float SpectreHexGame::WALL_THICKNESS = 50.0f;

SpectreHexGame::SpectreHexGame()
	: m__player(NULL)
	, m_state(GS_START)
{
}


SpectreHexGame::~SpectreHexGame()
{
}

void SpectreHexGame::Init(Mesh* _shadowBallMesh, Mesh* _circuitWallMesh, int viewWidth, int viewHeight)
{
	// Init Player
	if (m__player == NULL)
	{
		m__player = fetchObject();
		m__player->SetPos(Vector3(50.0f, (viewHeight - MIN_BALL_RADIUS * PLAYER_BALL_MULTIPLIER) * 0.5));
		m__player->SetScale(MIN_BALL_SCALE);
		m__player->SetMass(MIN_BALL_MASS * PLAYER_BALL_MULTIPLIER);
		m__player->SetMesh(_shadowBallMesh);
	}

	// Generate the Walls
	// -- Left
	PhysicalObject* wall = fetchObject();
	wall->SetPos(Vector3(WALL_THICKNESS * 0.5, viewHeight * 0.5f));
	wall->SetNormal(Vector2(1.0f));
	wall->SetScale(Vector2(WALL_THICKNESS, viewHeight));
	wall->SetKinematic(true);
	wall->SetMesh(_circuitWallMesh);

	// Top
	wall = fetchObject();
	wall->SetPos(Vector3(viewWidth * 0.5f, viewHeight - WALL_THICKNESS * 0.5));
	wall->SetNormal(Vector2(0.0f, 1.0f));
	wall->SetScale(Vector2(viewWidth, WALL_THICKNESS));
	wall->SetKinematic(true);
	wall->SetMesh(_circuitWallMesh);

	// Bottom
	wall = fetchObject();
	wall->SetPos(Vector3(viewWidth * 0.5f, WALL_THICKNESS * 0.5));
	wall->SetNormal(Vector2(0.0f, 1.0f));
	wall->SetScale(Vector2(viewWidth, WALL_THICKNESS));
	wall->SetKinematic(true);
	wall->SetMesh(_circuitWallMesh);

	// Generate balls
	for (int ball = 0; ball < MAX_BALLS; ++ball)
	{
		PhysicalObject* sBall = fetchObject();
		sBall->SetPos(Vector3(50.0f, (viewHeight - MIN_BALL_RADIUS) * 0.5));
		sBall->SetScale(MIN_BALL_SCALE);
		sBall->SetMass(MIN_BALL_MASS);
		sBall->SetMesh(_shadowBallMesh);
	}
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
	static bool s_shot = false;		// Ensures balls are only shot once

	// Forces
	static const Vector2 INITIAL_PUSH_PLAYER(25.0f);
	static const float INITIAL_PUSH_X = 1000.0f;

	// Timers
	static double s_timer = 0.0f;				// Timer for the balls to be introduced with the introduction shot
	static const double TIME_LIMIT = 2.5;		// The time to wait before players can start playing

	if (!s_shot)	// Apply the force once only
	{
		/* 
		 * Shoot the shadow balls in (entrance)
		 */
		// For producing a spread of balls
		const float OFFSET_Y_PER_SHOT = INITIAL_PUSH_X / MAX_BALLS;
		float offsetY = -(OFFSET_Y_PER_SHOT * MAX_BALLS * 0.5);

		for (vector<PhysicalObject*>::iterator ball = m_ballList.begin(); ball != m_ballList.end(); ++ball)
		{
			if (*ball == m__player)
			{
				// Shoot the player with less force to not push it so far
				m__player->AddForce(INITIAL_PUSH_PLAYER, dt);
			}
			else if (!(*ball)->GetKinematic())
			{
				(*ball)->AddForce(Vector2(INITIAL_PUSH_X, offsetY), dt);

				// Increment the offsetY for a spread effect
				offsetY += OFFSET_Y_PER_SHOT;
			}
		}

		// Stop the shooting
		s_shot = true;
	}
	else
	{
		s_timer += dt;

		if (s_timer > TIME_LIMIT)
		{
			m_state = GS_PLAYING;
		}

		for (vector<PhysicalObject*>::iterator ball = m_ballList.begin(); ball != m_ballList.end(); ++ball)
		{
			PhysicalObject* sBall = *ball;

			if (!sBall->GetActive())
			{
				continue;
			}
			else
			{
				sBall->UpdatePhysics(dt);
			}
		}
	}
}

void SpectreHexGame::ballsUpdate(double dt)
{
	for (vector<PhysicalObject*>::iterator ball = m_ballList.begin(); ball != m_ballList.end(); ++ball)
	{
		PhysicalObject* sBall = *ball;

		if (!sBall->GetActive())
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
}
