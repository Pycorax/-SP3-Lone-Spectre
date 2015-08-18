#include "SoundPlayer2D.h"

using irrklang::ISoundSource;

void SoundPlayer2D::Play(bool loop)
{
	m__musicController->setIsLooped(loop);
	m__musicController->setIsPaused(false);
}

void SoundPlayer2D::Stop(void)
{
	m__musicController->stop();
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
	m__musicController = _engine->play2D(_snd, false, true, true);
}