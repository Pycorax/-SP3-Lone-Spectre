#ifndef LEVEL_H
#define LEVEL_H

#include "TileMap.h"

class Level
{
private:
	TileMap* m__map;
	// TODO: Enemy patrol list
	// TODO: Character starting info

public:
	Level(void);
	~Level(void);

	void InitMap(Vector2 numMapTile, Vector2 numScreenTile, float tileSize, string filePath, vector<Mesh*> &meshList);\
	void Update(double dt);
	void Clear(void);
};

#endif