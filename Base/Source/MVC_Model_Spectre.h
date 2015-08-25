#ifndef MVC_MODEL_SPECTRE_H
#define MVC_MODEL_SPECTRE_H

#include "mvc_model.h"
#include "Level.h"
#include "Player.h"
#include "Collider2D.h"
#include "SpectreHexGame.h"
#include "PhysicalObject.h"
#include "Enemy.h"

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
	//TileMap/Overworld
	Level* m__testLevel;
	vector<GameObject2D*> m__tileMarkers;					// A vector of lights that can be used to render a "lighted" effect
	Mesh* m__tileMarkerMesh[NUM_TILE_MARKER];									// The meshes used for marking areas (shadows, areas being viewed)

	// Characters
	Player* m__player;
	Enemy* m__testEnemy;

	// Collision
	vector<Collider2D *> m__colliderList;

	// Spectral HexText MiniGame 
	bool m_hackMode;					// Controls whether hack mode is enabled
	SpectreHexGame m_hackingGame;		// Controls the hacking minigame for the spectre hextech skill

	// Other/Debug
	bool m_enableShadow;				// Controls if shadows will be rendered
	// Keep track of resolution change
	Vector2 resolution;

public:
	MVC_Model_Spectre(string configSONFile);
	virtual ~MVC_Model_Spectre(void);

	virtual void Init();
	virtual void Update(double dt);
	virtual void Exit();

protected:
	// Process Key Action Abstract Function
	void processKeyAction(double dt);

	// Light Resource
	void resetLightResources(void);
	GameObject2D* fetchTileMarker(TILE_MARKER_TYPE type);

	// Hack Mode (Spectre HexTech MiniGame)
	void updateHackMode(const double DT);
	void startHackMode(void);
	
	// Tilemap rendering
	void tileMapToRender(TileMap* _ToRender);

	// Screen resizing
	void resizeScreen();
};

#endif
