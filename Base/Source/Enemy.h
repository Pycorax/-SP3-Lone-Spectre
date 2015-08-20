#ifndef ENEMY_CLASS_H
#define ENEMY_CLASS_H

#include "NPC.h"
#include <vector>
#include "TileMap.h"
#include "Viewer.h"

class Enemy : public Character, public Viewer
{
public:
	enum E_ENEMY_STATE
	{
		ES_PATROL = 0,
		ES_SCAN,
		ES_CHASE,
		ES_ATTACK,
		ES_POSSESED,
		ES_KNOCKOUT,
	};

private:
	int m_alertLevel;
	Vector2 m_oldPos;
	bool m_bAlerted;

	//Sets the patrol position for the guard to stay in when in patrol state
	Vector2 m_patrolPointA, m_patrolPointB; 
	//use to check if mode start patrolling
	bool m_bReachPos;

public:
	Enemy(void);
	virtual ~Enemy(void);
	void Init(Vector2 pos, E_ENEMY_STATE enemyState);
	void update(double dt, TileMap* _map);

	void SetEndPatrolPoint(Vector2 pos);
	//moving to selected location
	bool MoveTo(Vector2 Startpos, Vector2 Endpos, TileMap* _map);
	
	void SetAlertLevel(int alertlevel);
	int GetAlertLevel(void);

	
	E_ENEMY_STATE m_enemyState;

};

#endif
