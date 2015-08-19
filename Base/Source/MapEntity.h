#ifndef MAP_ENTITY_H
#define MAP_ENTITY_H

#include "Vector2.h"

class MapEntity
{
private:	// Variables
	Vector2 m_mapPos;

public:		// Functions
	MapEntity(Vector2 mapPos);
	virtual ~MapEntity(void);

	// Use this function with MapEntities to set the position on the map and automatically update the screen position.
	void SetMapPosition(Vector2 pos, Vector2 scrollOffset);
	Vector2 GetMapPos();

protected:
	// Returns the screen pos in relation to m_tilePos. Use this function in updateScreenPos() to set the screen pos.
	Vector2 calcScreenPos(Vector2 scrollOffset);			

	/*
	 *	Abstract Functions
	 *	These functions need to be defined in child classes and serves as the interface between child classes and this class.
	 */
	// Function that updates the screen position by setting the screen position in a child class to getScreenPos()
	virtual void updateScreenPos(Vector2 pos, Vector2 scrollOffset) = 0;
};

#endif