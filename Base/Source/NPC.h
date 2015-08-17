#ifndef NPC_CLASS_H
#define NPC_CLASS_H

#include"CChar.h"

class NPC : public Character
{
private:
	bool m_aggressive;

public:
	NPC(void);
	virtual void Update(double dt);
	virtual ~NPC(void);

	//check if is hostile - if true == Enemy
	bool GetHostile(void);
};

#endif