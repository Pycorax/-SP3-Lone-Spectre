#ifndef TILE_H
#define TILE_H

#include "Mesh.h"
#include "GameObject2D.h"
#include "MapEntity.h"
#include "Viewer.h"

//STL
#include <vector>
using std::vector;

class Tile : public GameObject2D, public MapEntity
{
public:
	enum E_TILE_TYPE
	{
		TILE_FLOOR = 0,	// Floor tile
		TILE_INVISIBLE_WALL,
		TILE_WALL,
		TILE_OBJ_CABINET,
		TILE_OBJ_CHAIR_LEFT,
		TILE_OBJ_CHAIR_RIGHT,

		TILE_OBJ_COMPUTER_1_1, // Top half
		TILE_OBJ_COMPUTER_2_1, // Bottom half
		TILE_OBJ_CUPBOARD,
		TILE_OBJ_SINK,

		TILE_OBJ_TABLE_1_1, // Top left 
		TILE_OBJ_TABLE_1_2, // Top right
		TILE_OBJ_TABLE_2_1, // Bottom left
		TILE_OBJ_TABLE_2_2, // Bottom right

		TILE_OBJ_CAMERA_ON_1_1, // facing down
		TILE_OBJ_CAMERA_ON_1_2, // facing left
		TILE_OBJ_CAMERA_ON_1_3, // facing up
		TILE_OBJ_CAMERA_ON_1_4, // facing right

		TILE_OBJ_CAMERA_OFF_1_1, // facing down
		TILE_OBJ_CAMERA_OFF_1_2, // facing left
		TILE_OBJ_CAMERA_OFF_1_3, // facing up
		TILE_OBJ_CAMERA_OFF_1_4, // facing right

		TILE_LIGHT,
		TILE_DOCUMENT,
		TILE_BOMB,
		TILE_SETBOMBAREA,
		TILE_EXTRACTION,
		NUM_TILE,
	};

	static const bool S_IS_TILE_SOLID[NUM_TILE];
	static const int MAX_LIGHT_LEVEL = 10;

private:
	E_TILE_TYPE m_type;
	vector<Viewer *> m__viewerList;
	int m_lightLevel;					// Spans from 0 to MAX_LIGHT_LEVEL. Only a light tile can have MAX_LIGHT_LEVEL levels by default.

public:
	Tile(Vector2 mapPos = Vector2(0,0), Vector3 scale = Vector3(1,1,1), E_TILE_TYPE type = TILE_FLOOR, Mesh* _mesh = NULL);
	~Tile(void);

	void AddViewer(Viewer* _viewer);
	void RemoveViewer(Viewer* _viewer);
	void ResetViewers(void);
	bool IsViewed(void);
	bool IsViewedByNPC(void);
	void NotifyViewer(Vector2 targetTilePos, float &alertLevel, double dt);

	void AddLight(int lightValue);
	void ResetLighting(void);

	void SetType(E_TILE_TYPE type);
	E_TILE_TYPE GetType(void);
	int GetLightLevel(void);

protected:
	virtual void updateScreenPos(Vector2 pos);
};

#endif