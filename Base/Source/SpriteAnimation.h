#ifndef SPRITE_ANIMATION_H
#define SPRITE_ANIMATION_H

#include <vector>

#include "Mesh.h"
#include "Vector3.h"

using std::vector;
using std::string;

struct Animation
{
	Animation()
	{
		Set(0, 1, 1, 1.0f);
	}

	void Set(int startFrame, int endFrame, int repeat, float time)
	{
		this ->startFrame = startFrame;
		this->endFrame = endFrame;
		this->repeatCount = repeat;
		this->animTime = time;
	}

	int startFrame;
	int endFrame;
	int repeatCount;
	float animTime;
	bool ended;
};

class SpriteAnimation : public Mesh
{
	public:
		int m_row;
		int m_col;
		float m_currentTime;
		int m_currentFrame;
		int m_playCount;
		Animation *m_anim;

		SpriteAnimation(const string &meshName, int row, int col);
		virtual ~SpriteAnimation();
	
		void Update(double dt);
		virtual void Render();
		void Reset();
};

#endif