#ifndef MVC_MODEL_SPECTRE_H
#define MVC_MODEL_SPECTRE_H

#include "mvc_model.h"
#include "Level.h"
#include "Player.h"
#include "Collider2D.h"
#include "SpectreHexGame.h"
#include "PhysicalObject.h"
#include "Enemy.h"
#include "SecurityCamera.h"
#include "MessageManager.h"
#include "Objective.h"
#include "ObjectiveCollect.h"

class MVC_Model_Spectre : public MVC_Model
{
private:
	enum TILE_MARKER_TYPE
	{
		TM_SHADOW,
		TM_VIEWED,
		NUM_TILE_MARKER
	};
public:
	enum E_APP_STATE
	{
		APP_MENU_MAIN,
		APP_MENU_LEVEL_SELECT,
		APP_GAME_MAIN,
		APP_GAME_HACK,
	};
private:	// Variables
	// Levels
	Level* m__currentLevel;
	int m_currentLevelID;
	vector<string> m_levelFiles;

	//TileMap/Over-world
	vector<GameObject2D*> m__tileMarkers;					// A vector of lights that can be used to render a "lighted" effect
	Mesh* m__tileMarkerMesh[NUM_TILE_MARKER];									// The meshes used for marking areas (shadows, areas being viewed)

	// Characters
	Player* m__player;
	vector<Enemy*> m_enemyList;

	// Messaging System
	MessageManager m_messenger;
	vector<GameObject2D*> m_messageObjects;

	// Alert
	float m_alertLevel;

	// Cameras
	vector<SecurityCamera*> m_cameraList;

	// Collision
	vector<Collider2D *> m__colliderList;

	// Spectral HexText MiniGame 
	bool m_hackMode;					// Controls whether hack mode is enabled
	SpectreHexGame m_hackingGame;		// Controls the hacking mini-game for the spectre hex-tech skill

	// Other/Debug
	bool m_enableShadow;				// Controls if shadows will be rendered

	//Objectives
	Objective* m_objective;
public:
	MVC_Model_Spectre(string configSONFile);
	virtual ~MVC_Model_Spectre(void);

	virtual void Init();
	virtual void Update(double dt);
	virtual void Exit();

protected:
	//init player and animations
	void InitPlayer(void);

	// Load entities to lists
	void loadToList(TileMap* _map);

	// Process Key Action Abstract Function
	void processKeyAction(double dt);

	// Levels
	int findLevelFiles(string folderPath);
	void loadLevel(string levelMapFile);

	// MessageManager
	void pushMessageToRender(void);

	// Cameras
	void updateCamera(double dt);
	void clearCameraList();

	// Light Resource
	void resetTileMarkers(void);
	GameObject2D* fetchTileMarker(TILE_MARKER_TYPE type);

	// Hack Mode (Spectre HexTech MiniGame)
	void updateHackMode(const double DT);
	void startHackMode(void);

	// Tile-map rendering
	void tileMapToRender(TileMap* _ToRender);

	// Screen resizing code
	void onResolutionChanged(int oldViewWidth, int oldViewHeight);
	void resizeTileMap(int oldViewWidth);
};

#endif
