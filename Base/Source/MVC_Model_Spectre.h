#ifndef MVC_MODEL_SPECTRE_H
#define MVC_MODEL_SPECTRE_H

#include "mvc_model.h"
#include "TileMap.h"
#include "GameObject2D.h"

class MVC_Model_Spectre : public MVC_Model
{
private:	// Variables
	TileMap *test;

public:
	MVC_Model_Spectre(string configSONFile);
	virtual ~MVC_Model_Spectre(void);

	virtual void Init();
	virtual void Update(double dt);
	virtual void Exit();

	void processKeyAction(double dt);

private:
};

#endif