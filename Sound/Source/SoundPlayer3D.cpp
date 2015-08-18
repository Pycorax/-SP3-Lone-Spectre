#include "SoundPlayer3D.h"

vec3df SoundPlayer3D::ToVec3df(Vector3 vec)
{
	return vec3df(vec.x, vec.y, vec.z);
}

SoundPlayer3D::SoundPlayer3D(void)
{
}

SoundPlayer3D::~SoundPlayer3D(void)
{
}

void SoundPlayer3D::init(ISoundEngine* engine, ISoundSource* snd, Vector3 pos)
{
	m_musicController = engine->play3D(snd, ToVec3df(m_pos), false, true, true);
}

void SoundPlayer3D::SetPos(Vector3 pos)
{
	m_pos = pos;

	// Update the sound location in irrklang
	m_musicController->setPosition(ToVec3df(m_pos));
}
