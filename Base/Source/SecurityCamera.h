#ifndef SECURITY_CAMERA_H
#define SECURITY_CAMERA_H

#include "GameObject2D.h"
#include "MapEntity.h"
#include "ViewerUpdater.h"

class SecurityCamera : public GameObject2D, public MapEntity, public Viewer
{
public:
	enum E_CAM_MESH
	{
		CAM_ON_UP = 0,
		CAM_ON_DOWN,
		CAM_ON_LEFT,
		CAM_ON_RIGHT,
		CAM_OFF_UP,
		CAM_OFF_DOWN,
		CAM_OFF_LEFT,
		CAM_OFF_RIGHT,
		NUM_CAM_MESH,
	};
	static const int S_M_CAMERA_VIEW_WIDTH = 1;
	static const int S_M_CAMERA_VIEW_DIST = 2;
	static const float S_M_CAMERA_MAX_TIMER;
	static Mesh* s_camMeshList[NUM_CAM_MESH];
private:
	Vector2 m_dir; // Direction in which the camera is facing
	bool m_state; // False = off | True = on
	float m_timer; // Countdown timer to activate camera when not on
public:
	SecurityCamera(Vector2 mapPos, Vector2 dir, bool state, float tileSize = 1, int maxRangeX = S_M_CAMERA_VIEW_WIDTH, int maxRangeY = S_M_CAMERA_VIEW_DIST);
	~SecurityCamera(void);
	virtual Vector2 viewer_GetTilePos(void);
	virtual Vector2 viewer_GetDirection(void);
	virtual void SpottedTarget(Vector2 targetTilePos); // Response
	virtual void updateScreenPos(Vector2 pos);

	void Init(Vector2 dir, bool state, int maxRangeX, int maxRangeY);
	static void InitCamMeshList(const vector<Mesh*>& meshList);
	void Update(double dt, TileMap* _map);
	void GenerateViewBox(TileMap* _map);
	void DestroyViewBox(TileMap* _map);

	/*
	 * Setters
	 */
	void SetDir(Vector2 dir);
	void SetState(bool state);

	/*
	 * Getters
	 */
	Vector2 GetDir();
	bool GetState();
};

#endif