#include "SpectreHexGame.h"

// -- Balls
const float SpectreHexGame::MIN_BALL_RADIUS = 10.0f;
const float SpectreHexGame::MAX_BALL_RADIUS = 100.0f;
const float SpectreHexGame::PLAYER_BALL_SPAWN_RADIUS = 30.0f;

// -- Ball Physics
const float SpectreHexGame::MIN_BALL_MASS = 0.001f;
const float SpectreHexGame::PLAYER_BALL_MULTIPLIER = 1.5f;
const float SpectreHexGame::PLAYER_MOVE_FORCE = 30.0f;
const float SpectreHexGame::MAX_PLAYER_VEL = 150.0f;

// -- Walls
const float SpectreHexGame::WALL_THICKNESS = 50.0f;
const float SpectreHexGame::EXIT_WALL_THICKNESS = 400.0f;
const float SpectreHexGame::MIN_PLAYER_EXIT_RADIUS = 300.0f;

// -- Ball Spawning
const Vector2 SpectreHexGame::BALL_SPAWN_MIN_VEL(-120.0f, -120.0f);
const Vector2 SpectreHexGame::BALL_SPAWN_MAX_VEL(120.0f, 120.0f);
const Vector2 SpectreHexGame::BALL_SPAWN_MIN_POS(0.0f + WALL_THICKNESS + MAX_BALL_RADIUS, 0.0f + WALL_THICKNESS + MAX_BALL_RADIUS);
const Vector2 SpectreHexGame::BALL_SPAWN_MAX_POS_OFFSET(-EXIT_WALL_THICKNESS - MAX_BALL_RADIUS, -WALL_THICKNESS - MAX_BALL_RADIUS);
const float SpectreHexGame::MIN_LARGE_BALL_RADIUS = 30.0f;

SpectreHexGame::SpectreHexGame()
	: m__player(NULL)
	, m_state(GS_START)
	, m__background(NULL)
	, m__circuitWallMesh(NULL)
	, m__destroyedWallMesh(NULL)
	, m__restrictedWallMesh(NULL)
	, m__shadowBallMesh(NULL)
	, m__exitWall(NULL)
	, m__loseScreen(NULL)
{
}


SpectreHexGame::~SpectreHexGame()
{
}

void SpectreHexGame::Init(Mesh* _shadowBallMesh, Mesh* _playerBallMesh, Mesh* _circuitWallMesh, Mesh* _destroyedCircuitMesh, Mesh* _restrictedCircuitMesh, Mesh* _loseScreen, Mesh* _bgMesh, int viewWidth, int viewHeight)
{
	// Init all the meshes
	m__shadowBallMesh = _shadowBallMesh;
	m__playerBallMesh = _playerBallMesh;
	m__circuitWallMesh = _circuitWallMesh;
	m__destroyedWallMesh = _destroyedCircuitMesh;
	m__restrictedWallMesh = _restrictedCircuitMesh;

	Reset(viewWidth, viewHeight);

	// Generate BG
	m__background = new GameObject2D;
	m__background->SetMesh(_bgMesh);
	m__background->SetPos(Vector2::ZERO_VECTOR);
	m__background->SetScale(Vector2(viewWidth, viewHeight));

	// Generate Lose Screen
	m__loseScreen = new GameObject2D;
	m__loseScreen->SetMesh(_loseScreen);
	m__loseScreen->SetPos(Vector2(0.0f, viewHeight));
	m__loseScreen->SetScale(Vector2(viewWidth, viewHeight));
}

void SpectreHexGame::Update(double dt)
{
	switch (m_state)
	{
		case GS_START:
			startUpdate(dt);
			break;
		case GS_PLAYING:
			playingUpdate(dt);
			break;
		case GS_WIN_CEREMONY:
			winCeremonyUpdate(dt);
			break;
		case GS_LOSE_CEREMONY:
			loseCeremonyUpdate(dt);
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

void SpectreHexGame::Reset(int viewWidth, int viewHeight)
{
	m_state = GS_START;

	// Clear the list
	for (vector<PhysicalObject*>::iterator po = m_ballList.begin(); po != m_ballList.end(); ++po)
	{
		(*po)->SetActive(false);
	}

	// Reset the loseScreen
	if (m__loseScreen != NULL)
	{
		m__loseScreen->SetPos(Vector2(0.0f, viewHeight));
	}

	// Init Player
	if (m__player == NULL)
	{
		m__player = fetchObject();
	}

	if (m__player != NULL)
	{
		m__player->SetPos(Vector3(100.0f, (viewHeight - MIN_BALL_RADIUS * PLAYER_BALL_MULTIPLIER) * 0.5));
		m__player->SetScale(Vector2(PLAYER_BALL_SPAWN_RADIUS, PLAYER_BALL_SPAWN_RADIUS));
		m__player->InitPhysics2D(MIN_BALL_MASS* PLAYER_BALL_MULTIPLIER, false);
		m__player->SetMass(MIN_BALL_MASS * PLAYER_BALL_MULTIPLIER);
		m__player->SetMesh(m__playerBallMesh);
		m__player->SetActive(true);
	}

	// Generate the Walls
	// -- Left
	PhysicalObject* wall = fetchObject();
	wall->SetPos(Vector3(WALL_THICKNESS * 0.5, viewHeight * 0.5f));
	wall->SetScale(Vector2(WALL_THICKNESS, viewHeight));
	wall->InitPhysics2D(1.0f, true, Vector2::ZERO_VECTOR, Vector2(1.0f, 0.0f));
	wall->SetMesh(m__circuitWallMesh);

	// Top
	wall = fetchObject();
	wall->SetPos(Vector3(viewWidth * 0.5f, viewHeight - WALL_THICKNESS * 0.5));
	wall->SetScale(Vector2(WALL_THICKNESS, viewWidth));
	wall->InitPhysics2D(1.0f, true, Vector2::ZERO_VECTOR, Vector2(0.0f, -1.0f));
	wall->SetMesh(m__circuitWallMesh);

	// Bottom
	wall = fetchObject();
	wall->SetPos(Vector3(viewWidth * 0.5f, WALL_THICKNESS * 0.5));
	wall->SetScale(Vector2(WALL_THICKNESS, viewWidth));
	wall->InitPhysics2D(1.0f, true, Vector2::ZERO_VECTOR, Vector2(0.0f, 1.0f));
	wall->SetMesh(m__circuitWallMesh);

	// Right Exit Wall
	m__exitWall = fetchObject();
	m__exitWall->SetPos(Vector3(viewWidth - EXIT_WALL_THICKNESS * 0.5, viewHeight * 0.5f));
	m__exitWall->SetScale(Vector2(EXIT_WALL_THICKNESS, viewHeight));
	m__exitWall->InitPhysics2D(1.0f, true, Vector2::ZERO_VECTOR, Vector2(-1.0f, 0.0f));
	m__exitWall->SetMesh(m__restrictedWallMesh);

	// Generate balls
	int numLargeBalls = 0;
	for (int ball = 0; ball < MAX_BALLS; ++ball)
	{
		PhysicalObject* sBall = fetchObject();

		sBall->SetPos(Vector3(200.0f, (viewHeight - MIN_BALL_RADIUS) * 0.5));

		Vector2 pos(
			Math::RandFloatMinMax(BALL_SPAWN_MIN_POS.x, viewWidth + BALL_SPAWN_MAX_POS_OFFSET.x),
			Math::RandFloatMinMax(BALL_SPAWN_MIN_POS.y, viewHeight + BALL_SPAWN_MAX_POS_OFFSET.y)
			);
		Vector2 vel(
			Math::RandFloatMinMax(BALL_SPAWN_MIN_VEL.x, BALL_SPAWN_MAX_VEL.x),
			Math::RandFloatMinMax(BALL_SPAWN_MIN_VEL.y, BALL_SPAWN_MAX_VEL.y)
			);

		float radius;

		if (numLargeBalls < MAX_LARGE_BALLS)
		{
			radius = Math::RandFloatMinMax(MIN_LARGE_BALL_RADIUS, MAX_BALL_RADIUS);
			++numLargeBalls;
		}
		else
		{
			radius = Math::RandFloatMinMax(MIN_BALL_RADIUS, MIN_LARGE_BALL_RADIUS);
		}

		Vector2 scale(
			radius,
			radius
			);

		sBall->SetPos(pos);
		sBall->SetScale(scale);
		sBall->InitPhysics2D(MIN_BALL_MASS, false, vel);
		sBall->SetMesh(m__shadowBallMesh);
	}
}

bool SpectreHexGame::IsLoss() const
{
	return m_state == GS_END_IN_LOSS;
}

bool SpectreHexGame::IsVictory() const
{
	return m_state == GS_END_IN_WIN;
}

void SpectreHexGame::Move(bool left, bool right, bool up, bool down, double dt)
{
	// Don't attempt to move 
	if (m__player == NULL || (m_state != GS_PLAYING && m_state != GS_START))
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

vector<GameObject2D*> SpectreHexGame::GetRenderObjects(void) const
{
	vector<GameObject2D*> renderList;

	// Display the background at the back
	renderList.push_back(m__background);

	// Display all the objects
	for (vector<PhysicalObject*>::const_iterator ball = m_ballList.begin(); ball != m_ballList.end(); ++ball)
	{
		if ((*ball)->GetActive())
		{
			renderList.push_back(*ball);
		}
	}

	// Display lose screen during losing ceremony
	if (m_state == GS_LOSE_CEREMONY || m_state == GS_END_IN_LOSS)
	{
		renderList.push_back(m__loseScreen);
	}

	return renderList;
}

PhysicalObject* SpectreHexGame::fetchObject(void)
{
	// Retrieve a ball
	for (vector<PhysicalObject*>::iterator ball = m_ballList.begin(); ball != m_ballList.end(); ++ball)
	{
		if (!((*ball)->GetActive()))
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

	PhysicalObject* ball = m_ballList.back();
	ball->SetActive(true);

	return ball;
}

void SpectreHexGame::startUpdate(double dt)
{
	// First Run
	static bool firstRun = true;

	// Forces
	static const Vector2 INITIAL_PUSH_PLAYER(0.0f);
	static const float INITIAL_PUSH_X = 0.0f;

	// Timers
	static double timer = 0.0;				// Timer for the balls to be introduced with the introduction shot
	static const double TIME_LIMIT = 3.0;		// The time to wait before players can start playing

	// Invuln Indicator
	static const double BLINK_TIME = 0.25;
	static double blinkTimer = 0.0;
	static bool blink = false;

	// Air Bubble
	static const float AIR_BUBBLE_RADIUS = 400.0f;
	static PhysicalObject* _airBubble = fetchObject();

	// Initialize the air bubble at first run
	if (firstRun)
	{
		_airBubble->InitPhysics2D(0.00001f, true);
		_airBubble->SetScale(Vector2(AIR_BUBBLE_RADIUS, AIR_BUBBLE_RADIUS));

		firstRun = false;
	}

	// Blink the player
	blinkTimer += dt;
	if (blinkTimer > BLINK_TIME)
	{
		if (blink)
		{
			m__player->SetMesh(NULL);
		}
		else
		{
			m__player->SetMesh(m__playerBallMesh);
		}

		blinkTimer = 0.0;
		blink = !blink;
	}

	timer += dt;

	if (timer > TIME_LIMIT)
	{
		// Stop the shooting
		m_state = GS_PLAYING;

		// Reset static variables for next run
		firstRun = true;
		timer = 0.0f;

		// Ensure that the mesh of the player is not NULL when it leaves
		m__player->SetMesh(m__playerBallMesh);

		// Disable the air bubble
		_airBubble->SetActive(false);
	}

	/* 
	 * Shoot the shadow balls in (entrance)
	 */
	for (vector<PhysicalObject*>::iterator phyObj = m_ballList.begin(); phyObj != m_ballList.end(); ++phyObj)
	{
		PhysicalObject* po = static_cast<PhysicalObject *>(*phyObj);

		if (!po->GetActive())
		{
			continue;
		}
		
		if (po == m__player)
		{
			po->AddForce(INITIAL_PUSH_PLAYER, dt);
		}

		// Update this Physical Object
		po->UpdatePhysics(dt);

		if (po == m__player)
		{
			continue;
		}

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

			// If it is a collision with the air bubble revert the impact
			if (poA == _airBubble || poB == _airBubble)
			{
				// Shift the air bubble back to the player's position
				_airBubble->SetPos(m__player->GetTransform().Translation);
			}
		}
	}
}

void SpectreHexGame::playingUpdate(double dt)
{
	// Switch the right wall's mesh if player is big enough
	if (m__exitWall->GetMesh() != m__destroyedWallMesh && m__player->GetTransform().Scale.x >= MIN_PLAYER_EXIT_RADIUS)
	{
		m__exitWall->SetMesh(m__destroyedWallMesh);
	}

	// Limit the player's velocity
	if (m__player->GetVelocity().LengthSquared() > MAX_PLAYER_VEL * MAX_PLAYER_VEL)
	{
		m__player->SetVelocity(m__player->GetVelocity().Normalized() * MAX_PLAYER_VEL);
	}

	// Simulate all the walls and balls in the world
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
				// Check if player is involved in this collision
				PhysicalObject* otherObj = NULL;

				if (poA == m__player)
				{
					otherObj = poB;
				}
				else if (poB == m__player)
				{
					otherObj = poA;
				}
				else	// Player is not involved in this collision
				{
					poA->CollideRespondTo(poB);
				}

				// Player is involved!
				if (otherObj != NULL)
				{
					if (otherObj == m__exitWall)
					{
						// Check if player can exit
						if (m__player->GetTransform().Scale.x < MIN_PLAYER_EXIT_RADIUS)
						{
							// If cannot, do normal collision
							poA->CollideRespondTo(poB);
						}
						else
						{
							// Go to the win state
							m_state = GS_WIN_CEREMONY;
						}
					}
					else if (otherObj->GetKinematic() == false)
					{
						if (m__player->GetTransform().Scale.x > otherObj->GetTransform().Scale.x)
						{
							// Absorb the enemies
							m__player->SetMass(m__player->GetMass() + otherObj->GetMass() * 0.5);
							m__player->SetScale(m__player->GetTransform().Scale + otherObj->GetTransform().Scale * 0.5);
							otherObj->SetActive(false);
						}
						else if (m__player->GetTransform().Scale.x < otherObj->GetTransform().Scale.x)
						{
							// Absorb the enemies
							otherObj->SetMass(otherObj->GetMass() + m__player->GetMass() * 0.5);
							otherObj->SetScale(otherObj->GetTransform().Scale + m__player->GetTransform().Scale * 0.5);
							m__player->SetActive(false);

							// Player Loses
							m_state = GS_LOSE_CEREMONY;
						}
					}
					else
					{
						poA->CollideRespondTo(poB);
					}
				}
			}
		}
	}
}

void SpectreHexGame::winCeremonyUpdate(double dt)
{
	static bool firstFrame = true;						// Ensure only initialization portion is run once
	static const Vector2 LEAVE_FORCE(5.0f, 0.0f);
	static const Vector2 PLAYER_LEAVE_FORCE(100.0f, 0.0f);
	
	// Timer for whole ceremony
	static double timer = 0.0;
	static const double WAIT_TIME = 1.0;

	// Initialization for this portion
	if (firstFrame)
	{
		firstFrame = false;

		// Force the player to fly to the right instantly when a force is applied next
		m__player->SetVelocity(Vector2::ZERO_VECTOR);
	}

	// Update all the particles
	for (vector<PhysicalObject*>::iterator phyObj = m_ballList.begin(); phyObj != m_ballList.end(); ++phyObj)
	{
		PhysicalObject* po = static_cast<PhysicalObject*>(*phyObj);

		if (po == m__player)
		{
			po->AddForce(PLAYER_LEAVE_FORCE, dt);
			po->UpdatePhysics(dt);

			continue;
		}

		if (po->GetActive() && po->GetNormal() == Vector2::ZERO_VECTOR)
		{
			po->AddForce(LEAVE_FORCE, dt);
			po->UpdatePhysics(dt);
		}
	}

	// End the ceremony when it's done
	timer += dt;
	if (timer > WAIT_TIME)
	{
		// Reset Static Vars
		firstFrame = true;
		timer = 0.0;

		m_state = GS_END_IN_WIN;
	}
}

void SpectreHexGame::loseCeremonyUpdate(double dt)
{
	static bool firstFrame = true;
	static const float SCREEN_FALL_SPEED = 400.0f;

	// Timer for whole ceremony
	static double timer = 0.0;
	static const double WAIT_TIME = 0.5;

	if (firstFrame)
	{
		firstFrame = false;
		
		// Set all the walls to restricted mode
		for (vector<PhysicalObject*>::iterator phyObj = m_ballList.begin(); phyObj != m_ballList.end(); ++phyObj)
		{
			PhysicalObject* po = static_cast<PhysicalObject*>(*phyObj);

			if (po->GetActive() && po->GetNormal() != Vector2::ZERO_VECTOR)
			{
				po->SetMesh(m__restrictedWallMesh);
			}
		}
	}

	Transform loseScreenT = m__loseScreen->GetTransform();
	if (loseScreenT.Translation.y > -Math::EPSILON)
	{
		loseScreenT.Translation.y -= SCREEN_FALL_SPEED * dt;
		m__loseScreen->SetPos(loseScreenT.Translation);
	}
	else
	{
		m__loseScreen->SetPos(Vector2::ZERO_VECTOR);

		// End the ceremony when it's done
		timer += dt;
		if (timer > WAIT_TIME)
		{
			// Reset Static Vars
			firstFrame = true;
			timer = 0.0;

			m_state = GS_END_IN_LOSS;
		}
	}
}
