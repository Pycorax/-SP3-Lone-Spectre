#include "NPC.h"

NPC::NPC(void)
{
}

NPC::~NPC(void)
{
}


void NPC::Update(double dt)
{
	Character::Update();
}

bool NPC::GetHostile(void)
{
	return this->m_aggressive;
}