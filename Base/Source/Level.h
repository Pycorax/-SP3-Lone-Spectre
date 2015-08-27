#ifndef LEVEL_H
#define LEVEL_H

#include "TileMap.h"
#include "Enemy.h"
#include "ObjectiveAssassinate.h"
#include "ObjectiveCollect.h"
#include "ObjectiveDefuse.h"
#include "ObjectiveSetBomb.h"

class Level
{
public:
	enum LEVEL_MISSION_TYPE				// Enum stating the different types of missions
	{
		LM_COLLECT,						// Collect a package mission
		LM_ASSASSINATE,					// Assassinate a target mission
		LM_DEFUSE_BOMB,					// Defuse a bomb mission
		LM_PLANT_BOMB,					// Plant a bomb mission
		NUM_LEVEL_MISSIONS
	};

private:
	// TileMap
	TileMap* m__map;
	
	// Messages
	string m_messagesFile;				// A string that contains the file path to a message file with messages to load into a MessageManager

	// Level Timer
	double m_levelTimer;				// A timer for the level
	double m_levelMaxTime;				// The max time for this level
	
	// Level Mission Type
	LEVEL_MISSION_TYPE m_missionType;	// Stores the type of mission that this level will have

	// List of enemies
	vector<Enemy*> m_enemyList;			// A list of enemy templates for this level

	// Level Score
	int m_score;						// Stores the score of this level

	//Objective of current levels
	Objective* m__objective;			// Stores a pointer to an Objective Manager that manages this level's objectives

public:
	Level(void);
	~Level(void);

	void Load(string levelFilePath, int viewWidth, int viewHeight, vector<Mesh*>& meshList);
	void InitMap(Vector2 numMapTile, int viewWidth, int viewHeight, float tileSize, string filePath, vector<Mesh*> &meshList);
	void Update(double dt);
	void Clear(void);

	void ActivateObjective(void);
	void UpdateObjective(double dt);

	/*
	 * Getter Functions
	 */
	bool GetObjectiveComplete(void) const;
	bool GetActiveObjective(void) const;
	string GetMessagesFile(void) const;

	TileMap* GetTileMap();
	vector<Enemy*> GetEnemyList(void);
};

#endif