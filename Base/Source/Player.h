#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H

#include "CChar.h"

class Player : public Character
{
public:
	enum E_PLAYER_STATE
	{
		PS_IDLE = 0,
		PS_WALK,
		PS_SPECTRAL_DIVE,
		PS_SPECTRAL_JUMP,
		PS_SPECTRAL_HOST,
		PS_SPECTRAL_HAX,
		PS_CUTSCENE,
		PS_FOUND,
		PS_HIDE,
		PS_ATTACK,
		TOTAL_PLAYERSTATE,
	};
private:
	
	// Singleton
	static const int S_MAX_INSTANCES = 1;
	static Player* instances;
	Player(void);

	//player currentState
	E_PLAYER_STATE m_currentState;

public:

	static Player* GetInstance(int instance = 0);
	virtual ~Player(void);

	void Update(double dt);

	void SetState(E_PLAYER_STATE currentState);
	E_PLAYER_STATE GetState(void)const;
	
private:
	void ConstrainPlayer(void); //TODO : factor in map

};

#endif
