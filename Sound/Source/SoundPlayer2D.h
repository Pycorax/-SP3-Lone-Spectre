/******************************************************************************/
/*!
\file	SoundSource.h
\author Tng Kah Wei
\brief
Class to define background audio.

This sound system is based on Irrklang and is a wrapper for Irrklang so that the
implementation in the Base project need not change should there be changes to 
Irrklang's API
*/
/******************************************************************************/

#ifndef SOUND_SOURCE_H
#define SOUND_SOURCE_H

#include <string>
#include "irrKlang.h"
#include <vector>

#pragma comment(lib, "irrKlang.lib")

using std::string;
using std::vector;

using irrklang::ISoundEngine;
using irrklang::createIrrKlangDevice;
using irrklang::ISound;
using irrklang::ISoundSource;

class SoundPlayer2D
{
	friend class SoundEngine;

protected:	// Variables
	ISound* m__musicController;

public:		// Functions
	void Play(bool loop);
	void Stop(void);
	void Pause(void);

protected:	// Functions
	SoundPlayer2D(void);
	virtual ~SoundPlayer2D(void);

private:	// Functions
	void init(ISoundEngine* _engine, ISoundSource* _snd);
};

#endif