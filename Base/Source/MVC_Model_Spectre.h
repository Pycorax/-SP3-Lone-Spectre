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
private:	// Variables
	//TileMap/Overworld
	Level* m__testLevel;
	vector<GameObject2D*> m__lightResource;				// A vector of lights that can be used to render a "lighted" effect
	Mesh* m__lightMesh;									// The mesh used for m__lightResource

	// Characters
	Player* m__player;
	Enemy* m__testEnemy;

	// Collision
	vector<Collider2D *> m__colliderList;

	// Physics Test Code
	PhysicalObject* m__po1;
	PhysicalObject* m__po2;
 
	// Spectral HexText MiniGame 
	bool m_hackMode;					// Controls whether hack mode is enabled
	SpectreHexGame m_hackingGame;		// Controls the hacking minigame for the spectre hextech skill

	// Other/Debug
	GameObject2D* m__testGO;

public:
	MVC_Model_Spectre(string configSONFile);
	virtual ~MVC_Model_Spectre(void);

	virtual void Init();
	virtual void Update(double dt);
	virtual void Exit();
	void TileMapToRender(TileMap* _ToRender);

protected:
	// Process Key Action Abstract Function
	void processKeyAction(double dt);

	// Light Resource
	void resetLightResources(void);
	GameObject2D* fetchLight(void);

	// Hack Mode (Spectre HexTech MiniGame)
	void updateHackMode(const double DT);
	void startHackMode(void);
};

#endif
