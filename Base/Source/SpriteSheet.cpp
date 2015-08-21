#include "SpriteSheet.h"


SpriteSheet::SpriteSheet(string meshName) : Mesh(meshName)
{
}


SpriteSheet::~SpriteSheet(void)
{
}

void SpriteSheet::changeFrame(int frame)
{
	if (frame < m_row * m_col)
	{
		m_currentFrame = frame;
	}
}

void SpriteSheet::render()
{
	Mesh::Render(m_currentFrame * 6, 6);
}