#ifndef ENEMY_CLASS_H
#define ENEMY_CLASS_H

// STL Includes
#include <stack>

// Inheritance Include
#include "CChar.h"

// Other Includes
#include "TileMap.h"
#include "Viewer.h"
#include "PathFinder.h"
#include "SpriteAnimation.h"

// Using Directives
using std::stack;

class NPC : public Character, public Viewer
{
public:
	enum E_NPC_TYPE
	{
		NT_ENEMY = 0,
		NT_HOSTAGE,
		NT_TARGET,
		NUM_NPC_TYPE,
	};

	enum E_ENEMY_STATE
	{
		ES_PATROL = 0,
		ES_SCAN,
		ES_SPOTTED,
		ES_ATTACK,
		ES_GOSTAN,
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
	// NPC Type
	E_NPC_TYPE m_npcType;

	//spawn position
	Vector2 m_oldPos;

	int m_alertLevel;
	bool m_bPossesion;
	bool m_bMoving;

	//Scan the area around the enemy, to see if the Spectre is still there
	float m_checkAround; 

	//stores target location;
	vector<Vector2> m_pathWay;
	//time need before next move
	float m_moveTime;

	float m_moveToDist; // dist between points
	float m_moveDist;
	//keep track 
	int m_pathPointCounter;
	//use to check if mode start patrolling
	bool m_bReachPos;

	//timer before enemy attacks player
	double m_AttackCountdown;
	//timer to assume that the player is within view - will limit with the time needed for enemy to attack
	double m_ViewingTimer; 

	//sprite animation vector
	Animation* m__animationList[NUM_ENEMY_ACTION];
	float m_animTime;

	// For Gostan Mode
	bool m_wasPossessed;					// Stores if this enemy had been possessed. To be toggled false after gostan finished
	stack<Vector2> m_possessedTourStops;	// vector of tile positions that the enemy has went to while being possessed

	Character* _player;
public:
	NPC(void);
	virtual ~NPC(void);
	static NPC* CreateCopy(NPC npcToCopy);

	void Init(Vector2 pos, Mesh* _mesh);
	bool Update(double dt, TileMap* _map);
	
	void AddPatrolPoint(Vector2 pos);
	//moving to selected location - return true if reached
	bool MoveTo(Vector2 Endpos, TileMap* _map, double dt);

	void SetMoveToDist(Vector2 pos,float TileSize);
	
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
	void SetPlayerPtr(Character* _player);
	bool AttackingInView(Character* _go);

	void SetNPCType(E_NPC_TYPE type);
	E_NPC_TYPE GetNPCType(void);

	vector<Vector2> GetPathWay(void)const;
protected:
	// Function to allow the viewer to get the child class's transform details
	virtual Vector2 viewer_GetTilePos(void);
	// Function to allow the viewer to get the child class's facing direction
	virtual Vector2 viewer_GetDirection(void);
};

#endif
