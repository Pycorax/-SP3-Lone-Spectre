#include "SoundPlayer2D.h"

using irrklang::ISoundSource;

void SoundPlayer2D::Play(bool loop)
{
	// Reload the Sound
	if (m__musicController->isFinished())
	{
		m__musicController->drop();
		m__musicController = NULL;
		m__musicController = m__parentEngine->play2D(m__soundSource, false, true, true);
	}

	m__musicController->setIsLooped(loop);
	m__musicController->setIsPaused(false);
}

void SoundPlayer2D::Stop(void)
{
	m__musicController->stop();
	m__musicController->drop();
	m__musicController = NULL;
}

void SoundPlayer2D::Pause(void)
{
	m__musicController->setIsPaused(true);
}

SoundPlayer2D::SoundPlayer2D(void) : m__musicController(NULL)
{

}

SoundPlayer2D::~SoundPlayer2D(void)
{
}

void SoundPlayer2D::init(ISoundEngine* _engine, ISoundSource* _snd)
{
	// Store the sound for reloading if necessary
	m__soundSource = _snd;
	m__parentEngine = _engine;

	// Load the sound
	m__musicController = m__parentEngine->play2D(m__soundSource, false, true, true);
}