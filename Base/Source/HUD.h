#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "GameObject2D.h"

class HUD : public GameObject2D
{
public:
	HUD(void);
	virtual ~HUD(void);

	virtual void Init(Mesh* _mesh, Vector2 pos, Vector2 scale);
	virtual void Update(double dt);
	virtual void Clear();
};

#endif