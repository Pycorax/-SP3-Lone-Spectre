#ifndef VIEWER_H
#define VIEWER_H

#include "CChar.h"

class Viewer
{
private:		// Variables
	int m_maxRangeX;		// The maximum range of the view in tiles in the horizontal axis (viewer's front as reference)
	int m_maxRangeY;		// The maximum range of the view in tiles in the vertical axis (viewer's front as reference)

public:			// Functions
	Viewer(void);
	virtual ~Viewer(void);

	void Init(int maxRangeX, int maxRangeY);

	/*
	 * Getters
	 */
	int GetMaxRangeX(void);	
	int GetMaxRangeY(void);

	/*
	 *	Abstract Functions
	 *	These functions need to be defined in child classes and serves as the interface between child classes and this class.
	 */
	virtual void SpottedTarget(Character* _target) = 0;		// Function to be called by the tile that stores the Viewers, to inform the Viewer of the Character that has entered the Viewer's viewing box

};

#endif