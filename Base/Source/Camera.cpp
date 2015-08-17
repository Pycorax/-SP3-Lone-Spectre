#include "Camera.h"
#include "Mtx44.h"

Camera::Camera()
	: perspective(true)
	, fov(45.0f)
	, near(0.0f)
	, far(1.0f)
{
	Reset();
}

Camera::~Camera()
{
}

void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up, const float fov)
{
	this->position = pos;
	this->target = target;
	this->up = up;
	SetFOV(fov);
	perspective = true;

	// Initialize Camera Movement Flags
	for (size_t i = 0; i < NUM_KEYS; ++i)
	{
		m_keys[i] = false;
	}
}

void Camera::Reset()
{
	position.Set(1, 0, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}

void Camera::Update(double dt)
{
	static const float CAMERA_SPEED = 20.f;
	if(m_keys['A'])
	{
		position.x -= (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(m_keys['D'])
	{
		position.x += (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(m_keys['W'])
	{
		position.y += (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(m_keys['S'])
	{
		position.y -= (float)(CAMERA_SPEED * 0.2 * dt);
	}
}

Vector3 Camera::GetPosition()
{
	return position;
}

Vector3 Camera::GetTarget()
{
	return target;
}

Vector3 Camera::GetUp()
{
	return up;
}

float Camera::GetFOV(void)
{
	return fov;
}

bool Camera::IsPerspective()
{
	return perspective;
}

float Camera::GetNear()
{
	return near;
}

float Camera::GetFar()
{
	return far;
}

void Camera::SetPosition(Vector3 pos)
{
	position = pos;
}

void Camera::SetTarget(Vector3 tgt)
{
	target = tgt;
}

void Camera::SetUp(Vector3 _up)
{
	up = _up;
}

void Camera::SetFOV(float _fov)
{
	fov = _fov;
}

void Camera::SetPerspective(bool persp)
{
	perspective = persp;
}

void Camera::SetNear(float _near)
{
	near = _near;
}

void Camera::SetFar(float _far)
{
	far = _far;
}

void Camera::UpdateStatus(const unsigned char key)
{
	m_keys[key] = true;
}
