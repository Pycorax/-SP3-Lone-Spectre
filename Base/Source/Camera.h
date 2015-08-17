#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"

class Camera
{
protected:

	Vector3 position;
	Vector3 target;
	Vector3 up;
	bool perspective;

	// Perspective
	float fov;

	// Orthographic
	float near;
	float far;

	// Input
	static const unsigned NUM_KEYS = 255;
	bool m_keys[NUM_KEYS];

public:
	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up, const float fov = 45);
	virtual void Reset();
	virtual void Update(double dt);

	Vector3 GetPosition(void);
	Vector3 GetTarget(void);
	Vector3 GetUp(void);
	float GetFOV(void);
	bool IsPerspective(void);
	float GetNear(void);
	float GetFar(void);

	void SetPosition(Vector3 pos);
	void SetTarget(Vector3 tgt);
	void SetUp(Vector3 _up);
	void SetFOV(float _fov);
	void SetPerspective(bool persp);
	void SetNear(float _near);
	void SetFar(float _far);

	virtual void UpdateStatus(const unsigned char key);
};

#endif