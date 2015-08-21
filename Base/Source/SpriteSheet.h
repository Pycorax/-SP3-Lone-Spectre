#ifndef SPRITE_SHEET_H
#define SPRITE_SHEET_H

#include "Mesh.h"

class SpriteSheet : Mesh
{
private:
	int m_row, m_col;
	int m_currentFrame;
public:
	SpriteSheet(string meshName);
	virtual ~SpriteSheet(void);

	void changeFrame(int frame);
	virtual void render();
};

#endif