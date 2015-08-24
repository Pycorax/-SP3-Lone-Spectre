#ifndef VIEWER_H
#define VIEWER_H

#include "Vector2.h"

class Viewer
{
private:		// Variables
	int m_viewSideGrowthX;		// The increase of width of the field of view on each side per range increase by 1 unit
	int m_maxRangeY;		// The maximum range of the view in tiles in the vertical axis (viewer's bottom as reference)

public:			// Functions
	Viewer(void);
	virtual ~Viewer(void);

	void Init(int maxRangeX, int maxRangeY);

	/*
	 * Getters
	 */
	int GetViewSideGrowthX(void);
	int GetMaxRangeY(void); 

	/*
	 *	Abstract Functions
	 *	These functions need to be defined in child classes and serves as the interface between child classes and this class.
	 */
	// Function to allow the viewer to get the child class's transform details
	virtual Vector2 viewer_GetTilePos(void) = 0;
	// Function ti allow the viewer to get the child class's facing direction
	virtual Vector2 viewer_GetDirection(void) = 0;
	// Function to be called by the tile that stores the Viewers, to inform the Viewer of the Character that has entered the Viewer's viewing box
	virtual void SpottedTarget(Vector2 pos) = 0;
};

#endif