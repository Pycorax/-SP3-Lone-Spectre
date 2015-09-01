#ifndef LEVEL_H
#define LEVEL_H

#include "TileMap.h"
#include "NPC.h"
#include "Objective.h"

class Level
{
public:
	enum LEVEL_MISSION_TYPE				// Enum stating the different types of missions
	{
		LM_ESCAPE,						// Simply escape the level
		LM_COLLECT,						// Collect a package mission
		LM_ASSASSINATE,					// Assassinate a target mission
		LM_DEFUSE_BOMB,					// Defuse a bomb mission
		LM_PLANT_BOMB,					// Plant a bomb mission
		LM_HOSTAGE,						// Save hostage mission
		NUM_LEVEL_MISSIONS
	};
	static const Vector2 S_M_SCREEN_SIZE;

private:
	// TileMap
	TileMap* m__map;
	
	// Messages
	string m_messagesFile;				// A string that contains the file path to a message file with messages to load into a MessageManager

	// Sound
	string m_bgmName;					// Stores an ID to the background music

	// Level Timer
	double m_levelTimer;				// A timer for the level
	double m_levelMaxTime;				// The max time for this level
	
	// Level Mission Type
	LEVEL_MISSION_TYPE m_missionType;	// Stores the type of mission that this level will have

	// List of enemies
	vector<NPC*> m_enemyList;			// A list of enemy templates for this level

	// Level Score
	int m_score;						// Stores the score of this level

	//Objective of current levels
	Objective* m__objective;			// Stores a pointer to an Objective Manager that manages this level's objectives

public:
	Level(void);
	~Level(void);

	void Load(string levelFilePath, int viewWidth, int viewHeight, vector<Mesh*>& meshList, Vector2 numScreenTile);
	void InitMap(Vector2 numMapTile, int viewWidth, int viewHeight, float tileSize, string filePath, vector<Mesh*> &meshList, Vector2 numScreenTile);
	void Update(double dt);
	void Clear(void);

	void ActivateObjective(void);
	void UpdateObjective(double dt);
	void ResetObjective();
	/*
	 * Getter Functions
	 */
	Objective* GetObjective(void) const;
	bool GetObjectiveComplete(void) const;
	bool GetActiveObjective(void) const;
	string GetMessagesFile(void) const;
	string GetBGMName(void) const;

	TileMap* GetTileMap();
	vector<NPC*> GetEnemyList(void);
};

#endif