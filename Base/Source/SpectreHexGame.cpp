#include "SpectreHexGame.h"

const float SpectreHexGame::MIN_BALL_RADIUS = 20.0f;
const Vector2 SpectreHexGame::MIN_BALL_SCALE(MIN_BALL_RADIUS, MIN_BALL_RADIUS);
const float SpectreHexGame::MIN_BALL_MASS = 0.01f;
const float SpectreHexGame::PLAYER_BALL_MULTIPLIER = 1.0f;
const float SpectreHexGame::PLAYER_MOVE_FORCE = 500.0f;

SpectreHexGame::SpectreHexGame()
	: m__player(NULL)
{
}


SpectreHexGame::~SpectreHexGame()
{
}

void SpectreHexGame::Init(Mesh* _shadowBallMesh)
{
	// Init Player
	if (m__player == NULL)
	{
		m__player = fetchBall();
		m__player->SetPos(Vector3(50.0f, 50.0f));
		m__player->SetScale(MIN_BALL_SCALE);
		m__player->SetMass(MIN_BALL_MASS * PLAYER_BALL_MULTIPLIER);
		m__player->SetMesh(_shadowBallMesh);
	}

	// Generate balls
	for (int ball = 0; ball < MAX_BALLS; ++ball)
	{
		ShadowBall* sBall = fetchBall();
		sBall->SetPos(Vector3(50.0f, 50.0f));
		sBall->SetScale(MIN_BALL_SCALE);
		sBall->SetMass(MIN_BALL_MASS);
		sBall->SetMesh(_shadowBallMesh);
	}
}

void SpectreHexGame::Update(double dt)
{
	ballsUpdate(dt);
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
	if (m__player == NULL)
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

	for (vector<ShadowBall*>::iterator ball = m_ballList.begin(); ball != m_ballList.end(); ++ball)
	{
		if ((*ball)->GetActive())
		{
			renderList.push_back(*ball);
		}
	}

	return renderList;
}

ShadowBall* SpectreHexGame::fetchBall(void)
{
	// Retrieve a ball
	for (vector<ShadowBall*>::iterator ball = m_ballList.begin(); ball != m_ballList.end(); ++ball)
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
		ShadowBall* sBall = new ShadowBall();
		sBall->SetActive(false);
		m_ballList.push_back(sBall);
	}

	m_ballList.back()->SetActive(true);

	return m_ballList.back();
}

void SpectreHexGame::ballsUpdate(double dt)
{
	for (vector<ShadowBall*>::iterator ball = m_ballList.begin(); ball != m_ballList.end(); ++ball)
	{
		ShadowBall* sBall = *ball;

		if (!sBall->GetActive())
		{
			continue;
		}
		else
		{
			sBall->UpdatePhysics(dt);
		}

		for (vector<ShadowBall*>::iterator ball2 = ball + 1; ball2 != m_ballList.end(); ++ball2)
		{
			ShadowBall* sBall2 = *ball2;

			if (!sBall2->GetActive())
			{
				continue;
			}
			else
			{
				sBall2->UpdatePhysics(dt);
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
