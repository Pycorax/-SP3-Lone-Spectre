#ifndef MVC_MODEL_SPECTRE_H
#define MVC_MODEL_SPECTRE_H

#include "mvc_model.h"
#include "Level.h"
#include "Player.h"
#include "Collider2D.h"
#include "SpectreHexGame.h"
#include "PhysicalObject.h"
#include "NPC.h"
#include "SecurityCamera.h"
#include "MessageManager.h"
#include "Objective.h"
#include "ObjectiveCollect.h"
#include "ObjectiveAssassinate.h"
#include "ObjectiveDefuse.h"
#include "ObjectiveSetBomb.h"
#include "ObjectiveHostage.h"
#include "HUD_Cooldown.h"
#include "SoundPlayer2D.h"
#include "MenuManager_Spectre.h"
#include "HUD_Objectives.h"

class MVC_Model_Spectre : public MVC_Model
{
private:
	enum TILE_MARKER_TYPE
	{
		TM_SHADOW,
		TM_VIEWED,
		TM_CAMERA,
		NUM_TILE_MARKER
	};

	enum SOUND_PLAYER_TYPE
	{
		SP_MAIN_MENU,
		SP_SKILL_DIVE_ENTER,
		SP_SKILL_DIVE_EXIT,
		SP_SKILL_HACK_START,
		SP_SKILL_HACK_STOP,
		SP_SKILL_SPECTRAL_JUMP,
		SP_SKILL_SPECTRAL_HOST_ENTER,
		SP_SKILL_SPECTRAL_HOST_EXIT,
		SP_OBJ_COMPLETE,
		SP_OBJ_BOMB_DEFUSING,
		SP_OBJ_BOMB_PLANTING,
		SP_OBJ_ASSASSINATE,
		SP_DEATH,
		SP_EXPLOSIVE_DEATH,
		NUM_SOUND_PLAYER
	};

public:
	enum E_APP_STATE
	{
		AS_MENU,
		AS_MAIN_GAME,
		AS_HACK_GAME,
		NUM_APP_STATE
	};

private:	// Static Constant
	static const Vector2 S_M_MESSAGE_OFFSET;
	static const double S_M_LIGHTING_UPDATE_FREQUENCY;
	static const float S_M_BGM_VOLUME;
	static const float S_M_MENU_KEYS_INPUT_DELAY;
	static const int S_M_PLAYER_MAX_HP;

public:		// Static Constant
	static const float S_M_MAX_ALERT;

private:	// Variables
	// Application/Game Life Cycle
	E_APP_STATE m_appState;

	// Menu
	MenuManager_Spectre* m__menu;
	float m_menuKeysInputTimer;
	float m_pauseTimer;
	
	// Light timer
	double m_lightUpdateTimer;

	// Levels
	Level* m__currentLevel;
	int m_currentLevelID;
	vector<string> m_levelFiles;
	SoundPlayer2D* m__bgm;

	// TileMap/Over-world
	vector<GameObject2D*> m__tileMarkers;					// A vector of lights that can be used to render a "lighted" effect
	Mesh* m__tileMarkerMesh[NUM_TILE_MARKER];				// The meshes used for marking areas (shadows, areas being viewed)
	Mesh* m__extractGoodMesh;								// Stores the mesh of the exit-able extraction zone
	Mesh* m__extractNoGoodMesh;								// Stores the mesh of the unexit-able extraction zone

	// Characters
	Player* m__player;
	vector<NPC*> m_enemyList;
	bool m_shadowMode;

	// Messaging System
	MessageManager m_messenger;
	vector<GameObject2D*> m_messageObjects;

	// Sound System
	SoundPlayer2D* m__soundPlayer[NUM_SOUND_PLAYER];			// Stores handles to sound players

	/*
	 * HUD
	 */
	// ALert
	float m_alertLevel;
	HUD_Cooldown* m__alert;

	// Skills
	HUD_Cooldown* m__spectreDive;
	HUD_Cooldown* m__spectreJump;
	HUD_Cooldown* m__spectreHost;
	HUD_Cooldown* m__defuseBomb;
	HUD_Cooldown* m__plantBomb;

	// Prompt action
	HUD* m__fKey[NUM_INPUT_DEVICE];
	HUD* m__kKey[NUM_INPUT_DEVICE];

	// Objectives
	HUD_Objectives* m__objectivesHUD;
	
	// Cameras
	vector<SecurityCamera*> m_cameraList;

	// Collision
	vector<Collider2D *> m__colliderList;

	// Spectral HexText MiniGame 
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
	void updateMainGame(double dt);
	void updateHackGame(double dt);
	//init player and animations
	void initPlayer(void);

	// Enemy
	void clearEnemyList();

	// Menu
	void initMenu(void);
	void updateMenu(double dt);

	// HUD
	void initHUD(void);
	void updateHUD(double dt);

	// Load entities to lists
	void loadToList(TileMap* _map);

	// Process Key Action Abstract Function
	void processKeyAction(double dt);
	void processMenuKeyAction(MenuManager::E_RETURN_STATE returnState);

	// Levels
	int findLevelFiles(string folderPath);
	void loadLevel(string levelMapFile);
	void nextLevel(void);
	void prevLevel(void);

	// MessageManager
	void pushMessageToRender(void);

	// Cameras
	void updateCamera(double dt);
	void clearCameraList();

	// Light Resource
	void resetTileMarkers(void);
	GameObject2D* fetchTileMarker(TILE_MARKER_TYPE type);

	// Hack Mode (Spectre HexTech MiniGame)
	void startHackMode(void);

	// Tile-map rendering
	void updateLighting(double dt);
	void tileMapToRender(TileMap* _ToRender);

	// Screen resizing code
	void onResolutionChanged(int oldViewWidth, int oldViewHeight);
	void resizeTileMap(float oldTileSize, float newTileSize);
};

#endif
