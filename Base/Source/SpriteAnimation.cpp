#include "SpriteAnimation.h"

SpriteAnimation::SpriteAnimation(const std::string &meshName, int row, int col)
	: Mesh(meshName)
	, m_row(row)
	, m_col(col)
	, m_currentTime(0)
	, m_currentFrame(0)
	, m_playCount(0)
{
	m_anim = NULL;
}

SpriteAnimation::~SpriteAnimation()
{
	if (m_anim)
	{
		delete m_anim;
	}
}

void SpriteAnimation::Update(double dt)
{
	if (m_anim) // If animation happens
	{
		// Animation current time, m_currentTime increments by delta time, dt
		m_currentTime += static_cast<float>(dt);

		// Variable int numFrame is set to maximum value between (1,(endFrame – startFrame + 1))
		int numFrame = Math::Max(1, m_anim->endFrame - m_anim->startFrame + 1);
		
		// Variable float frameTime = animTime / numFrame
		float frameTime = m_anim->animTime / numFrame;
		
		// Animation current frame, m_currentFrame is set to the minimum value between(endFrame, (startFrame + (m_currentTime / frameTime)))
		m_currentFrame = Math::Min(m_anim->endFrame, static_cast<int>(m_anim->startFrame + (m_currentTime / frameTime)));

		// If the m_currentTime is greater than animTime
		if (m_currentTime > m_anim->animTime)
		{
			//Set animation end flag, ended to true
			m_anim->ended = true;

			//If m_playCount + 1 is less than repeatCount or repeatCount == 0
			if (m_anim->repeatCount == 0 || m_playCount + 1 < m_anim->repeatCount)
			{
				//then increment m_playCount
				++m_playCount;
				//reset m_currentTime
				m_currentTime = 0.0f;

				//set m_currentFrame = startFrame
				m_currentFrame = m_anim->startFrame;
			}			
		}	
	}
}

void SpriteAnimation::Render()
{
	Mesh::Render(m_currentFrame * 6, 6);
}

void SpriteAnimation::Reset()
{
	m_anim->ended = false;
	m_currentTime = 0;
	m_currentFrame = m_anim->startFrame;
}