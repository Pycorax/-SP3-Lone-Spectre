#ifndef ENEMY_CLASS_H
#define ENEMY_CLASS_H

#include "NPC.h"
#include <vector>
#include "TileMap.h"
#include "Viewer.h"
#include "PathFinder.h"
#include "SpriteAnimation.h"
#include "MVC_Model.h"

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
		NUM_ENEMY_STATE,
	};

	enum E_ENEMY_ACTION
	{
		EA_IDLE_UP = 0,
		EA_IDLE_DOWN,
		EA_IDLE_LEFT,
		EA_IDLE_RIGHT,
		EA_WALK_UP,
		EA_WALK_DOWN,
		EA_WALK_LEFT,
		EA_WALK_RIGHT,
		NUM_ENEMY_ACTION,
	};

private:
	//spawn position
	Vector2 m_oldPos;

	int m_alertLevel;
	bool m_bAlerted;
	bool m_bChasing;
	bool m_bAttacking;
	bool m_bScanning;
	bool m_bPossesion;
	bool m_bMoving;

	//Scan the area around the enemy, to see if the Spectre is still there
	float m_checkAround; 

	//m_patrolPointA -> the old target location, m_patrolPointB -> the target location
	Vector2 m_patrolPointA, m_patrolPointB; 
	//stores target location;
	vector<Vector2> m_pathWay;
	//time need before next move
	float m_moveTime;

	//keep track 
	int m_pathPointCounter;
	//use to check if mode start patrolling
	bool m_bReachPos;

	//sprite animation vector
	Animation* m__animationList[NUM_ENEMY_ACTION];
	float m_animTime;

public:
	Enemy(void);
	virtual ~Enemy(void);
	void Init(Vector2 pos, Mesh* _mesh);
	void Update(double dt, TileMap* _map);
	
	void AddPatrolPoint(Vector2 pos);
	//moving to selected location - return true if reached
	bool MoveTo(Vector2 Endpos, TileMap* _map, double dt);
	
	void SetAlertLevel(int alertlevel);
	int GetAlertLevel(void);

	E_ENEMY_ACTION m_enemyAction;

	void SetPossesion(bool state);

	E_ENEMY_STATE m_enemyState;
	void ForceSetEnemyState(E_ENEMY_STATE enemyState);
	void SpottedTarget(Vector2 pos, float &alertLevel, double dt);

	//init enemy and animations
	void AddAnimation(Animation* _anim, E_ENEMY_ACTION enemyState);
	void ChangeAnimation(double dt);

	//if given location is in view, use this to attack
	bool AttackingInView(Character* _go);
protected:
	// Function to allow PathFinder to obtain the tile position of the child via reference
	virtual void pathFinder_getTilePosition(unsigned& tileXPos, unsigned& tileYPos) const;
	// Function to allow the viewer to get the child class's transform details
	virtual Vector2 viewer_GetTilePos(void);
	// Function to allow the viewer to get the child class's facing direction
	virtual Vector2 viewer_GetDirection(void);
};

#endif
