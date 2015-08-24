#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H

#include "CChar.h"
#include "TileMap.h"
#include "GameObject2D.h"
#include "Enemy.h"
#include "Tile.h"
#include "SpriteAnimation.h"

class Player : public Character
{
	public:
		enum E_PLAYER_ACTION
		{
			PA_IDLE = 0,
			PA_MOVE_UP,
			PA_MOVE_DOWN,
			PA_MOVE_LEFT,
			PA_MOVE_RIGHT,
			PA_INTERACT,
			NUM_PLAYER_ACTIONS,
		};
		// used to tell which state player is in and change the rendering of mesh
		enum E_PLAYER_STATE
		{
			PS_IDLE_UP = 0,
			PS_IDLE_LEFT,
			PS_IDLE_RIGHT,
			PS_IDLE_DOWN,
			PS_WALK_UP,
			PS_WALK_DOWN,
			PS_WALK_LEFT,
			PS_WALK_RIGHT,
			PS_SPECTRAL_DIVE,
			PS_SPECTRAL_JUMP,
			PS_SPECTRAL_HOST,
			PS_SPECTRAL_HAX,
			PS_INACTIVE,
			NUM_PLAYERSTATE,
		};

		//player move speed
		static float s_playerMoveSpeed;
	private:
		Player(void);
		
		// Singleton
		static const int S_MAX_INSTANCES = 1;
		static Player* s_instances;
		
		//player currentState
		E_PLAYER_STATE m_currentState;

		// For reference
		bool m_actions[NUM_PLAYER_ACTIONS];
		bool m_moving;
		float m_moveDist;

		GameObject2D* m__host;
		Tile* m__tile;
		
		//sprite animation vector
		Mesh* m__saList[NUM_PLAYERSTATE];
		int m_saStateCounter;
	public:
		static Player* GetInstance(int instance = 0);
		static void Clear();

		virtual ~Player(void);

		void Init(Mesh* _mesh);
		void AddMesh(Mesh* _mesh, E_PLAYER_STATE playerState);
		void Update(double dt, TileMap* _map);
		void UpdateHost(double dt);

		void SetState(E_PLAYER_STATE currentState);
		E_PLAYER_STATE GetState(void)const;
	
		void SetActions(E_PLAYER_ACTION type, bool status = true);

		// Actions
		E_PLAYER_STATE Interact(TileMap* _map);


		//getters
		Mesh* GetSA(void) const;
	private:
		void move(double dt, TileMap* _map);
		void resetMove();

};

#endif
