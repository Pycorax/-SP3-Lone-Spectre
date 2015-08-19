#ifndef VIEWER_H
#define VIEWER_H

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
};

#endif