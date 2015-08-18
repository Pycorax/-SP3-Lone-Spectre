#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H

#include "CChar.h"
#include "TileMap.h"

class Player : public Character
{
	public:
		enum E_PLAYER_ACTION
		{
			PA_MOVE_UP = 0,
			PA_MOVE_DOWN,
			PA_MOVE_LEFT,
			PA_MOVE_RIGHT,
			TOTAL_PLAYER_ACTIONS,
		};
		enum E_PLAYER_STATE
		{
			PS_IDLE = 0,
			PS_WALK,
			PS_SPECTRAL_DIVE,
			PS_SPECTRAL_JUMP,
			PS_SPECTRAL_HOST,
			PS_SPECTRAL_HAX,
			PS_CUTSCENE,
			TOTAL_PLAYERSTATE,
		};
	private:
		Player(void);
		
		// Singleton
		static const int S_MAX_INSTANCES = 1;
		static Player* instances;
		
		//player currentState
		E_PLAYER_STATE m_currentState;

		// For reference
		/*bool m_actions[TOTAL_PLAYER_ACTIONS];
		void SetActions(E_PLAYER_ACTION type, bool status = true)
		{
			m_actions[type] = status;
		}*/

		TileMap* _map;
		Transform transform;
	public:
		static Player* GetInstance(int instance = 0);
		virtual ~Player(void);

		void Init(void);

		void Update(double dt, TileMap* _map, E_PLAYER_ACTION movementKey);

		void SetState(E_PLAYER_STATE currentState);
		E_PLAYER_STATE GetState(void)const;
	
	private:
		void ConstrainPlayer(TileMap* _map); //TODO : factor in map

};

#endif
