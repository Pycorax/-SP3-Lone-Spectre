#include "SoundEngine.h"

void SoundEngine::StartSoundEngine()
{
	engine = createIrrKlangDevice();

	if (engine == NULL)
	{
		throw std::runtime_error("Failed to create SoundEngine!");
	}
}

void SoundEngine::StopSoundEngine()
{
	if (engine != NULL)
	{
		engine->drop();
	}
}

unsigned SoundEngine::AddSoundSource(string filePath)
{
	ISoundSource* snd = engine->addSoundSourceFromFile(filePath.c_str());
	soundLibrary.push_back(snd);

	return soundLibrary.size() - 1;
}

void SoundEngine::ClearSoundLibrary(void)
{
	while (soundLibrary.size() > 0)
	{
		soundLibrary.back()->drop();
		soundLibrary.pop_back();
	}
}

SoundPlayer2D * SoundEngine::CreateSound2D(unsigned soundID)
{
	if (soundID > soundLibrary.size() - 1)
	{
		throw std::out_of_range("Sound ID provided is out of range!");
	}

	SoundPlayer2D* _sndPlayer = new SoundPlayer2D();
	_sndPlayer->init(engine, soundLibrary.at(soundID));

	return _sndPlayer;
}

SoundPlayer3D * SoundEngine::CreateSound3D(unsigned soundID, Vector3 pos)
{
	if (soundID > soundLibrary.size() - 1)
	{
		throw std::out_of_range("Sound ID provided is out of range!");
	}

	SoundPlayer3D* _sndPlayer = new SoundPlayer3D();
	_sndPlayer->init(engine, soundLibrary.at(soundID), pos);

	return _sndPlayer;
}

void SoundEngine::StopAll(void)
{
	engine->stopAllSounds();
}