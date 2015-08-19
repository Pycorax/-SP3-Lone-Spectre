#ifndef ENEMY_CLASS_H
#define ENEMY_CLASS_H

#include "NPC.h"
#include <vector>

class Enemy : public NPC
{


public:
	enum E_ENEMY_STATE
	{
		ES_PATROL = 0,
		ES_SCAN,
		ES_CHASE,
		ES_ATTACK,
		ES_REVEAL,
		ES_KNOCKOUT,
	};

private:
	int m_alertLevel;
	Vector2 oldPos;
	bool alerted;

public:
	Enemy(void);
	virtual ~Enemy(void);
	
	void update(double dt);

	//moving to selected location
	bool MoveTo(Vector2 pos);
	
	void SetAlertLevel(int alertlevel);
	int GetAlertLevel(void);
	
	E_ENEMY_STATE m_enemyState;

};

#endif
