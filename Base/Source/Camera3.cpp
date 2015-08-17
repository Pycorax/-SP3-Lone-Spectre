#include "Camera3.h"
#include "Mtx44.h"

const float Camera3::CAMERA_SPEED[NUM_MOVE_MODE] = { 500.0f, 1500.0f, 250.0f, 125.0f, 200.0f };
const float Camera3::MAX_CAMERA_SPEED[NUM_MOVE_MODE] = { 100.0f, 300.0f, 50.0f, 25.0f, 300.0f };
const float Camera3::DECEL_SPEED[NUM_MOVE_MODE] = { 150.0f, 450.0f, 75.0f, 37.5f, 60.0f };
const float Camera3::CAMERA_HEIGHT[NUM_MOVE_MODE] = { 25.0f, 25.0f, 12.5f, 6.25f, 25.0f };
const double Camera3::MAX_SPRINT_TIME = 2.5;
const float Camera3::CAM_TURN_SPEED = 250.0f;
const double Camera3::MAX_PITCH = 60.0;

Camera3::Camera3()
{
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up, const float fov)
{
	// Init the camera type
	sCameraType = LAND_CAM;

	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();

	SetFOV(fov);

	// Initialize Camera Movement Flags
	for (size_t i = 0; i < NUM_KEYS; ++i)
	{
		m_keys[i] = false;
	}

	// Initialize Jump Variables
	m_fJumpVel = 0.0f;
	JUMP_MAX_SPEED = 30.0f;
	JUMP_ACCEL = 30.0f;
	GRAVITY = -50.0f;

	// Movement Variables
	m_camMoveMode = DEFAULT_MOVE;
	m_vel.Set();
	m_sprintTimer = MAX_SPRINT_TIME;

	// Camera Rotation as Angles
	m_camPitch = 0.0;
	m_camYaw = 0.0;
}

void Camera3::Update(double dt)
{
	// Deceleration
	if (sCameraType != JUMP_MOVE)
	{
		if (m_vel.z > Math::EPSILON)
		{
			m_vel.z -= DECEL_SPEED[sCameraType] * dt;

			if (m_vel.z < Math::EPSILON)
			{
				m_vel.z = 0.0f;
			}
		}
		else if (m_vel.z < -Math::EPSILON)
		{
			m_vel.z += DECEL_SPEED[sCameraType] * dt;

			if (m_vel.z > Math::EPSILON)
			{
				m_vel.z = 0.0f;
			}
		}

		if (m_vel.x > Math::EPSILON)
		{
			m_vel.x -= DECEL_SPEED[sCameraType] * dt;

			if (m_vel.x < Math::EPSILON)
			{
				m_vel.x = 0.0f;
			}
		}
		else if (m_vel.x < -Math::EPSILON)
		{
			m_vel.x += DECEL_SPEED[sCameraType] * dt;

			if (m_vel.x > Math::EPSILON)
			{
				m_vel.x = 0.0f;
			}
		}
	}
	
	// Movement
	if (m_keys['w'] == true)
	{
		Walk(dt);
	}
	if (m_keys['s'] == true)
	{
		Walk(-dt);
	}
	if (m_keys['a'] == true)
	{
		Strafe(-dt);
	}
	if (m_keys['d'] == true)
	{
		Strafe(dt);
	}

	// Look
	if (m_keys['i'] == true)
	{
		Pitch(dt);
	}
	if (m_keys['k'] == true)
	{
		Pitch(-dt);
	}
	if (m_keys['j'] == true)
	{
		Yaw(-dt);
	}
	if (m_keys['l'] == true)
	{
		Yaw(dt);
	}

	/*
	 * To stop sprinting when player stops pressing movement keys
	 */
	bool notMoving = m_vel.x == 0.0f && m_vel.z == 0.0f;

	if (notMoving && IsSprinting())
	{
		m_camMoveMode = DEFAULT_MOVE;
	}

	// Camera Switch
	if (m_keys[0] == true)
	{
		SwitchCamera(AIR_CAM);
	}

	if (m_keys[1] == true)
	{
		if (sCameraType == AIR_CAM)
		{
			// Allows the player to fall down gracefully if he is in the air
			Jump();
		}

		SwitchCamera(LAND_CAM);
	}
	
	if (sCameraType == LAND_CAM)
	{
		// Jump
		if (m_keys[' '] == true)
		{
			Jump();
		}

		// Sprint
		if (m_keys['x'] == true)
		{
			Sprint();
		}
		// Crouch
		else if (m_keys['c'] == true)
		{
			Crouch();
		}
		// Prone
		else if (m_keys['z'] == true)
		{
			Prone();
		}
	}
	
	UpdateMove(dt);
	UpdateJump(dt);

	// Sprint Limiter
	if (sCameraType == LAND_CAM)
	{
		if (IsSprinting())
		{
			m_sprintTimer -= dt;

			if (m_sprintTimer <= 0.0)
			{
				m_camMoveMode = DEFAULT_MOVE;
			}
		}
		else
		{
			if (m_sprintTimer < MAX_SPRINT_TIME)
			{
				m_sprintTimer += dt;
			}
			else
			{
				m_sprintTimer = MAX_SPRINT_TIME;
			}
		}
	}

	// Reset all the button states
	for (size_t i = 0; i < NUM_KEYS; ++i)
	{
		m_keys[i] = false;
	}
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

void Camera3::SetCameraType(CAM_TYPE sCamType)
{
	sCameraType = sCamType;
}

Camera3::CAM_TYPE Camera3::GetCameraType(void)
{
	return sCameraType;
}

void Camera3::UpdateMove(const double dt)
{
	Vector3 view = (target - position).Normalized();
	Vector3 deltaMove;
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();

	// Forward and Backward
	deltaMove = view * m_vel.z * static_cast<float>(dt);

	// Strafing
	deltaMove += right * m_vel.x * static_cast<float>(dt);

	// Prevent flying
	if (sCameraType == LAND_CAM)
	{
		deltaMove.y = 0;
	}

	// Apply the change in movement
	position += deltaMove;
	target += deltaMove;
}

void Camera3::UpdateJump(const double dt)
{
	if (m_camMoveMode == JUMP_MOVE)
	{
		// Factor in gravity
		m_fJumpVel += GRAVITY * static_cast<float>(dt);
		// Update the camera and target position
		position.y += m_fJumpVel * static_cast<float>(dt);
		target.y += m_fJumpVel * static_cast<float>(dt);

		// Check if the camera has reached the ground
		if (position.y <= CAMERA_HEIGHT[m_camMoveMode])
		{
			float dtMove = CAMERA_HEIGHT[m_camMoveMode] - position.y;
			position.y = CAMERA_HEIGHT[m_camMoveMode];
			target.y += dtMove;
			m_fJumpVel = 0.0f;
			m_camMoveMode = DEFAULT_MOVE;
		}
	}
}

/******************************************************************************
Move the camera forward
******************************************************************************/
void Camera3::MoveForward(const double dt)
{
	Vector3 oldVel = m_vel;

	// Increase the velocity
	m_vel.z += static_cast<float>(dt) * CAMERA_SPEED[m_camMoveMode];

	// Check if velocity > max velocity
	if (m_vel.z > MAX_CAMERA_SPEED[m_camMoveMode])
	{
		m_vel.z = MAX_CAMERA_SPEED[m_camMoveMode];
	}
}
/******************************************************************************
Move the camera backward
******************************************************************************/
void Camera3::MoveBackward(const double dt)
{
	if (m_camMoveMode == SPRINT_MOVE)
	{
		m_camMoveMode = DEFAULT_MOVE;
	}

	Vector3 oldVel = m_vel;

	// Increase the velocity
	m_vel.z -= static_cast<float>(dt) * CAMERA_SPEED[m_camMoveMode];

	// Check if velocity > max velocity
	if (m_vel.z < -MAX_CAMERA_SPEED[m_camMoveMode])
	{
		m_vel.z = -MAX_CAMERA_SPEED[m_camMoveMode];
	}
}
/******************************************************************************
Move the camera left
******************************************************************************/
void Camera3::MoveLeft(const double dt)
{
	Vector3 oldVel = m_vel;

	// Increase the velocity
	m_vel.x -= static_cast<float>(dt) * CAMERA_SPEED[m_camMoveMode];

	// Check if velocity > max velocity
	if (m_vel.x < -MAX_CAMERA_SPEED[m_camMoveMode])
	{
		m_vel.x = -MAX_CAMERA_SPEED[m_camMoveMode];
	}
}
/******************************************************************************
Move the camera right
******************************************************************************/
void Camera3::MoveRight(const double dt)
{
	Vector3 oldVel = m_vel;

	// Increase the velocity
	m_vel.x += static_cast<float>(dt) * CAMERA_SPEED[m_camMoveMode];

	// Check if velocity > max velocity
	if (m_vel.x > MAX_CAMERA_SPEED[m_camMoveMode])
	{
		m_vel.x = MAX_CAMERA_SPEED[m_camMoveMode];
	}
}

void Camera3::TurnLeft(const double dt)
{
	Vector3 view = (target - position).Normalized();
	float yaw = (float)(-CAM_TURN_SPEED * (float)dt);
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	view = rotation * view;
	target = position + view;
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();

	m_camYaw += yaw;
}

void Camera3::TurnRight(const double dt)
{
	Vector3 view = (target - position).Normalized();
	float yaw = (float)(-CAM_TURN_SPEED * (float)dt);
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	view = rotation * view;
	target = position + view;
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();

	m_camYaw += yaw;
}

void Camera3::LookDown(const double dt)
{
	float pitch = static_cast<float>(CAM_TURN_SPEED * static_cast<float>(dt));
	
	if (m_camPitch + pitch < -MAX_PITCH)
	{
		return;
	}

	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	view = rotation * view;
	target = position + view;

	m_camPitch += pitch;
}

void Camera3::LookUp(const double dt)
{
	float pitch = static_cast<float>(CAM_TURN_SPEED * static_cast<float>(dt));

	if (m_camPitch + pitch > MAX_PITCH)
	{
		return;
	}

	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	view = rotation * view;
	target = position + view;

	m_camPitch += pitch;
}

void Camera3::Pitch(const double dt)
{
	if (dt > Math::EPSILON)
	{
		LookUp(dt);
	}
	else if (dt < -Math::EPSILON)
	{
		LookDown(dt);
	}
}

void Camera3::Yaw(const double dt)
{
	if (dt > Math::EPSILON)
	{
		TurnRight(dt);
	}
	else if (dt < -Math::EPSILON)
	{
		TurnLeft(dt);
	}
}

void Camera3::Walk(const double dt)
{
	if (dt > 0)
	{
		MoveForward(dt);
	}
	else if (dt < 0)
	{
		MoveBackward(abs(dt));
	}
}

void Camera3::Strafe(const double dt)
{
	if (dt > 0)
	{
		MoveRight(dt);
	}
	else if (dt < 0)
	{
		MoveLeft(abs(dt));
	}
}

void Camera3::Jump()
{
	if (m_camMoveMode != JUMP_MOVE)
	{
		m_camMoveMode = JUMP_MOVE;
		// Calculate the jump velocity
		m_fJumpVel = JUMP_ACCEL;// * dt;
		// Factor in maximum speed limit
		if (m_fJumpVel > JUMP_MAX_SPEED)
		{
			m_fJumpVel = JUMP_MAX_SPEED;
		}
	}
}

void Camera3::Sprint()
{
	if (m_camMoveMode != JUMP_MOVE)
	{
		if (m_camMoveMode != SPRINT_MOVE && m_sprintTimer > 0.0)	// If not sprinting, sprint
		{
			m_camMoveMode = SPRINT_MOVE;
		}
		else	// If sprint button pressed when sprinting, stop sprinting
		{
			m_camMoveMode = DEFAULT_MOVE;
		}
	}
}

void Camera3::Crouch()
{
	if (m_camMoveMode != JUMP_MOVE)
	{
		if (m_camMoveMode != CROUCH_MOVE)	// If not crouching, crouch
		{
			m_camMoveMode = CROUCH_MOVE;
		}
		else	// If crouch button pressed when crouching, stop crouching
		{
			m_camMoveMode = DEFAULT_MOVE;
		}
	}
}

void Camera3::Prone()
{
	if (m_camMoveMode != JUMP_MOVE)
	{
		if (m_camMoveMode != PRONE_MOVE)	// If not proning, prone
		{
			m_camMoveMode = PRONE_MOVE;
		}
		else	// If prone button pressed when proning, stop proning
		{
			m_camMoveMode = DEFAULT_MOVE;
		}
	}
}

void Camera3::SwitchCamera(CAM_TYPE type)
{
	switch (sCameraType)
	{
		case LAND_CAM:
			if (type == AIR_CAM)
			{
				m_camMoveMode = SPRINT_MOVE;
			}
			break;
		case AIR_CAM:
			if (type == LAND_CAM)
			{
				m_camMoveMode = JUMP_MOVE;
			}
			break;
	}

	sCameraType = type;
}

void Camera3::PushTarget(Vector3 push)
{
	float oriDistToTarget = (position - target).Length();
	Vector3 oldTarget = target;

	// Push the target
	target += push;

	// Calculate pitch change and add to m_camPitch
	float targetDT = (oldTarget - target).Length();
	float pitch = Math::RadianToDegree(atan(targetDT / oriDistToTarget));

	// Ensure that pushing the target doesn't overshoot the MAX_PITCH and break your neck so to speak
	if (m_camPitch + pitch > MAX_PITCH || m_camPitch + pitch < -MAX_PITCH)
	{
		target = oldTarget;
	}
	else
	{
		m_camPitch += pitch;
	}
}

Vector3 Camera3::GetRotation(void)
{
	Vector3 result;

	// Rotation on Y-Axis
	float deltaX = target.x - position.x;
	float deltaZ = target.z - position.z;
	result.y = atan2(deltaX, deltaZ) * 180 / Math::PI;

	// Rotation on X-Axis
	Vector3 T = target, P = position;
	float O = T.y - P.y;

	T.y = P.y = 0;

	float A = (T - P).Length();
	result.x = (atan(O / A) * 180 / Math::PI);

	return result;
}

Vector3 Camera3::GetView(void)
{
	return (target - position).Normalized();
}

float Camera3::GetPitch(void)
{
	return m_camPitch;
}

float Camera3::GetYaw(void)
{
	return m_camYaw;
}

bool Camera3::IsWalking(void)
{
	return m_camMoveMode == DEFAULT_MOVE;
}

bool Camera3::IsSprinting(void)
{
	return m_camMoveMode == SPRINT_MOVE;
}
