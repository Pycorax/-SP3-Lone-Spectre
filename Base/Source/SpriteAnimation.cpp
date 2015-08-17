#include "SpriteAnimation.h"

SpriteAnimation::SpriteAnimation(const std::string &meshName, int row, int col, bool autoplay)
	: Mesh(meshName)
	, m_row(row)
	, m_col(col)
	, m_currentTime(0)
	, m_currentFrame(0)
	, m_playCount(0)
	, m_autoPlay(autoplay)
{
	m_animationList = NULL;
}

SpriteAnimation::~SpriteAnimation()
{
	if (m_animationList)
	{
		delete m_animationList;
	}
}

void SpriteAnimation::Update(double dt)
{
	if (m_animationList && m_autoPlay) // If animation happens
	{
		// Animation current time, m_currentTime increments by delta time, dt
		m_currentTime += static_cast<float>(dt);

		// Variable int numFrame is set to maximum value between (1,(endFrame – startFrame + 1))
		int numFrame = Math::Max(1, m_animationList->endFrame - m_animationList->startFrame + 1);
		
		// Variable float frameTime = animTime / numFrame
		float frameTime = m_animationList->animTime / numFrame;
		
		// Animation current frame, m_currentFrame is set to the minimum value between(endFrame, (startFrame + (m_currentTime / frameTime)))
		m_currentFrame = Math::Min(m_animationList->endFrame, static_cast<int>(m_animationList->startFrame + (m_currentTime / frameTime)));

		// If the m_currentTime is greater than animTime
		if (m_currentTime > m_animationList->animTime)	
		{
			//Set animation end flag, ended to true
			m_animationList->ended = true;

			//If m_playCount + 1 is less than repeatCount or repeatCount == 0
			if (m_animationList->repeatCount == 0 || m_playCount + 1 < m_animationList->repeatCount)	
			{
				//then increment m_playCount
				++m_playCount;
				//reset m_currentTime
				m_currentTime = 0.0f;

				//set m_currentFrame = startFrame
				m_currentFrame = m_animationList->startFrame;
			}			
		}	
	}
}

void SpriteAnimation::Render()
{
	Mesh::Render(m_currentFrame * 6, 6);
}

void SpriteAnimation::NextFrame(void)
{
	if (m_autoPlay)
	{
		return;
	}

	m_currentFrame = Math::Wrap(++m_currentFrame, m_animationList->startFrame, m_animationList->endFrame);
}

void SpriteAnimation::PrevFrame(void)
{
	if (m_autoPlay)
	{
		return;
	}

	m_currentFrame = Math::Wrap(++m_currentFrame, m_animationList->startFrame, m_animationList->endFrame);
}