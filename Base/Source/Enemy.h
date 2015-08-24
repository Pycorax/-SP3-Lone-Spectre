#ifndef ENEMY_CLASS_H
#define ENEMY_CLASS_H

#include "NPC.h"
#include <vector>
#include "TileMap.h"
#include "Viewer.h"
#include "Pathway.h"
#include "PathPoint.h"
#include "PathFinder.h"

class Enemy : public Character, public Viewer, public PathFinder
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
	bool m_bSearching;
	bool m_bChasing;
	bool m_bAttacking;
	bool m_bScanning;
	bool m_bPossesion;

	//Scan the area around the enemy, to see if the Spectre is still there
	float m_checkAround; 

	//m_patrolPointA -> the old target location, m_patrolPointB -> the target location
	Vector2 m_patrolPointA, m_patrolPointB; 
	//stores target location;
	Pathway m_pathWay;
	//keep track 
	int m_pathPointCounter;
	//use to check if mode start patrolling
	bool m_bReachPos;

	Vector2 m_spectralPositon;

public:
	Enemy(void);
	virtual ~Enemy(void);
	void Init(Vector2 pos, E_ENEMY_STATE enemyState);
	void Update(double dt, TileMap* _map);

	//void SetStartPatrolPoint(Vector2 pos);
	//void SetEndPatrolPoint(Vector2 pos);
	void AddPatrolPoint(Vector2 pos);
	//moving to selected location
	bool MoveTo(Vector2 Endpos, TileMap* _map, double dt);
	
	void SetAlertLevel(int alertlevel);
	int GetAlertLevel(void);

	void SetSpectrePosition(Vector2 spectralPosition);
	Vector2 GetSpectrePosition();

	E_ENEMY_STATE m_enemyState;

	void SpottedTarget(Vector2 pos);
private:
	bool checkTileMapCollision(TileMap* _map);

protected:
	// Function to get the player's current position
	virtual Vector2 pathFinder_getTilePosition(void);
	// Function to allow the viewer to get the child class's transform details
	virtual Vector2 viewer_GetTilePos(void);
	// Function ti allow the viewer to get the child class's facing direction
	virtual Vector2 viewer_GetDirection(void);
};

#endif
