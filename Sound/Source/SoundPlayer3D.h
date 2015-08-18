/******************************************************************************/
/*!
\file	SoundSource.h
\author Tng Kah Wei
\brief
Class to define audio from a specific point

This sound system is based on Irrklang and is a wrapper for Irrklang so that the
implementation in the Base project need not change should there be changes to
Irrklang's API
*/
/******************************************************************************/
#ifndef SOUND_POINT_H
#define SOUND_POINT_H

#include "SoundPlayer2D.h"
#include "Vector3.h"

using irrklang::vec3df;

class SoundPlayer3D : SoundPlayer2D
{
	private:
		Vector3 m_pos;

		static vec3df ToVec3df(Vector3 vec);

	public:
		SoundPlayer3D(void);
		~SoundPlayer3D(void);

		void init(ISoundEngine* engine, ISoundSource* snd, Vector3 pos);
		void SetPos(Vector3 pos);
};

#endif