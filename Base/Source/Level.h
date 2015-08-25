#ifndef LEVEL_H
#define LEVEL_H

#include "TileMap.h"

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
	
	// Level Timer
	double m_levelTimer;
	double m_levelMaxTime;
	
	// Level Mission Type
	LEVEL_MISSION_TYPE m_missionType;

	// TODO: List of enemies

	// Level Score
	int m_score;

public:
	Level(void);
	~Level(void);

	void Load(string levelFilePath, int viewWidth, int viewHeight, vector<Mesh*>& meshList);
	void InitMap(Vector2 numMapTile, int viewWidth, int viewHeight, float tileSize, string filePath, vector<Mesh*> &meshList);
	void Update(double dt);
	void Clear(void);

	TileMap* GetTileMap();
};

#endif