#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include <vector>

using std::vector;

class Camera3 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	enum CAM_TYPE
	{
		LAND_CAM,
		AIR_CAM,
		NUM_CAM_TYPE
	};

	Camera3();
	~Camera3();

	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up, const float fov = 45);
	virtual void Update(double dt);
	virtual void Reset();

	// Camera Type
	void SetCameraType(CAM_TYPE sCamType);
	CAM_TYPE GetCameraType(void);

	// Update Camera status
	virtual void UpdateMove(const double dt);
	virtual void UpdateJump(const double dt);

	/*
	 * Basic Methods
	 */
	// Movement
	virtual void MoveForward(const double dt);
	virtual void MoveBackward(const double dt);
	virtual void MoveLeft(const double dt);
	virtual void MoveRight(const double dt);
	virtual void TurnLeft(const double dt);
	virtual void TurnRight(const double dt);
	virtual void LookUp(const double dt);
	virtual void LookDown(const double dt);

	/*
	 * Applied Methods
	 */
	virtual void Pitch(const double dt);
	virtual void Yaw(const double dt);
	virtual void Walk(const double dt);
	virtual void Strafe(const double dt);
	virtual void Jump();
	virtual void Sprint();
	virtual void Crouch();
	virtual void Prone();

	// Camera Switching
	virtual void SwitchCamera(CAM_TYPE type);

	// Push Camera Target
	void PushTarget(Vector3 push);

	// Getters
	Vector3 GetRotation(void);
	Vector3 GetView(void);
	float GetPitch(void);
	float GetYaw(void);
	bool IsWalking(void);
	bool IsSprinting(void);

protected:
	// Crouching, proning and sprinting
	enum CAM_MOVE_MODE
	{
		DEFAULT_MOVE,
		SPRINT_MOVE,
		CROUCH_MOVE,
		PRONE_MOVE,
		JUMP_MOVE,
		NUM_MOVE_MODE
	};

	CAM_TYPE sCameraType;

	// Camera Height
	static const float CAMERA_HEIGHT[NUM_MOVE_MODE];

	// Camera Speeds
	static const float CAMERA_SPEED[NUM_MOVE_MODE];
	static const float MAX_CAMERA_SPEED[NUM_MOVE_MODE];
	static const float CAM_TURN_SPEED;
	static const float DECEL_SPEED[NUM_MOVE_MODE];
	CAM_MOVE_MODE m_camMoveMode;
	Vector3 m_vel;

	static const double MAX_SPRINT_TIME;
	double m_sprintTimer;

	// Camera Roation as Angles
	double m_camPitch;
	double m_camYaw;

	// Limit Camera Pitching
	static const double MAX_PITCH;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	// Jumping
	float GRAVITY;
	float m_fJumpVel;
	float JUMP_MAX_SPEED, JUMP_ACCEL;
};

#endif