#include "SoundPlayer2D.h"

using irrklang::ISoundSource;

SoundPlayer2D::SoundPlayer2D(void) : m_musicController(NULL)
{

}

SoundPlayer2D::~SoundPlayer2D(void)
{
}

void SoundPlayer2D::init(ISoundEngine* engine, ISoundSource* snd)
{
	m_musicController = engine->play2D(snd, false, true, true);
}

void SoundPlayer2D::Play(bool loop)
{
	m_musicController->setIsLooped(loop);
	m_musicController->setIsPaused(false);
}

void SoundPlayer2D::Stop(void)
{
	m_musicController->stop();
	m_musicController = NULL;
}

void SoundPlayer2D::Pause(void)
{
	m_musicController->setIsPaused(true);
}