#ifndef MVC_MODEL_SPECTRE_H
#define MVC_MODEL_SPECTRE_H

#include "mvc_model.h"
#include "Level.h"
#include "Player.h"
#include "Collider2D.h"

class MVC_Model_Spectre : public MVC_Model
{
private:	// Variables
	//TileMap *_test;
	Level* m__testLevel;
	Player* m__player;
	GameObject2D* m__testGO;
	vector<Collider2D *> m__collidorList;
public:
	MVC_Model_Spectre(string configSONFile);
	virtual ~MVC_Model_Spectre(void);

	virtual void Init();
	virtual void Update(double dt);
	virtual void Exit();
	void TileMapToRender(TileMap* _ToRender);

	void processKeyAction(double dt);
};

#endif