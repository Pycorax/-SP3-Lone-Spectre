#include "SoundEngine.h"

void SoundEngine::StartSoundEngine()
{
	s__engine = createIrrKlangDevice();

	if (s__engine == NULL)
	{
		throw std::runtime_error("Failed to create SoundEngine!");
	}
}

void SoundEngine::StopSoundEngine()
{
	if (s__engine != NULL)
	{
		s__engine->drop();
	}
}

unsigned SoundEngine::AddSoundSource(string filePath)
{
	ISoundSource* snd = s__engine->addSoundSourceFromFile(filePath.c_str());
	s_soundLibrary.push_back(snd);

	return s_soundLibrary.size() - 1;
}

void SoundEngine::ClearSoundLibrary(void)
{
	while (s_soundLibrary.size() > 0)
	{
		s_soundLibrary.back()->drop();
		s_soundLibrary.pop_back();
	}
}

SoundPlayer2D * SoundEngine::CreateSound2D(unsigned soundID)
{
	if (soundID > s_soundLibrary.size() - 1)
	{
		throw std::out_of_range("Sound ID provided is out of range!");
	}

	SoundPlayer2D* _sndPlayer = new SoundPlayer2D();
	_sndPlayer->init(s__engine, s_soundLibrary.at(soundID));

	return _sndPlayer;
}

SoundPlayer3D * SoundEngine::CreateSound3D(unsigned soundID, Vector3 pos)
{
	if (soundID > s_soundLibrary.size() - 1)
	{
		throw std::out_of_range("Sound ID provided is out of range!");
	}

	SoundPlayer3D* _sndPlayer = new SoundPlayer3D();
	_sndPlayer->init(s__engine, s_soundLibrary.at(soundID), pos);

	return _sndPlayer;
}

void SoundEngine::StopAll(void)
{
	s__engine->stopAllSounds();
}