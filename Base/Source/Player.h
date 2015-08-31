#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H

#include "CChar.h"
#include "TileMap.h"
#include "GameObject2D.h"
#include "NPC.h"
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
			/*PS_IDLE = 0,
			PS_WALK,*/
			PS_IDLE_UP = 0,
			PS_IDLE_LEFT,
			PS_IDLE_RIGHT,
			PS_IDLE_DOWN,
			PS_WALK_UP,
			PS_WALK_DOWN,
			PS_WALK_LEFT,
			PS_WALK_RIGHT,
			PS_SPECTRAL_DIVE,
			
			PS_SPECTRAL_DIVE_UP,
			PS_SPECTRAL_DIVE_DOWN,
			PS_SPECTRAL_DIVE_LEFT,
			PS_SPECTRAL_DIVE_RIGHT,
			
			PS_SPECTRAL_JUMP,
			PS_SPECTRAL_HOST,
			PS_SPECTRAL_HAX,
			//diving into shadow state - diving in animation
			PS_SPECTRAL_DIVING_UP,
			PS_SPECTRAL_DIVING_DOWN,
			PS_SPECTRAL_DIVING_LEFT,
			PS_SPECTRAL_DIVING_RIGHT,
	
			//Objective Interaction
			PS_SPECTRAL_ASSASSINATE,
			PS_SPECTRAL_COLLECT,
			PS_SPECTRAL_DEFUSE,
			PS_SPECTRAL_SETBOMB,

			// Escape
			PS_SPECTRAL_ESCAPE,

			PS_INACTIVE,
			NUM_PLAYERSTATE,
		};
		enum E_INTERACTION
		{
			INTERACT_DIVE,
			INTERACT_HOST,
			INTERACT_JUMP,
			INTERACT_HAX,

			//Objective Interaction
			INTERACT_ASSASSINATE,
			INTERACT_COLLECT,
			INTERACT_DEFUSE,
			INTERACT_SETBOMB,

			// Escape
			INTERACT_ESCAPE,

			NUM_INTERACT,
		};

		//player move speed
		static const float S_PLAYER_MOVE_SPEED;
		static const float S_PLAYER_JUMP_SPEED;
		static const float S_SPECTRE_DIVE_LIGHT_LIMIT;
		static const float S_SPECTRE_DIVE_COOLDOWN;
		static const float S_SPECTRE_JUMP_COOLDOWN;
		static const int S_MAX_JUMP_RANGE;
	private:
		Player(void);
		
		// Singleton
		static const int S_MAX_INSTANCES = 1;
		static Player* s_instances;
		
		//player currentState
		E_PLAYER_STATE m_currentState;
		bool m_inShadow; // Player in shadow

		// Moving
		bool m_moving;
		float m_moveDist;
		float m_currentSpeed;

		// Spectral dive
		bool m_diving;
		float m_diveTimer;

		// Spectral jump
		bool m_jumping;
		float m_jumpTimer;
		int m_tileMoved;

		//spectral host
		bool m_hosting;
		float m_hostingTimeLimit;
		NPC* m__host;

		Tile* m__tile;
		
		//sprite animation vector
		Animation* m__animationList[NUM_PLAYERSTATE];
		float m_animTime;

		bool m_spectreMode;
	public:
		static Player* GetInstance(int instance = 0);
		static void Clear();

		virtual ~Player(void);

		void Init(Mesh* _mesh);
		void AddAnimation(Animation* _anim, E_PLAYER_STATE playerState);
		void Update(double dt, TileMap* _map);

		void SetHostPTR(NPC* _enemy);
		void SetMove(Vector2 dir);
		void SetDive(void);
		void SetJump(void);
		void SetHost(void);

		void SetState(E_PLAYER_STATE currentState);
		E_PLAYER_STATE GetState(void)const;

		// Actions
		E_PLAYER_STATE Interact(E_INTERACTION interact, TileMap* _map);

		void SetInShadow(bool inShadow);
		bool GetInShadow();

		/*
		 * Getters
		 */
		float GetDiveTimer();
		float GetJumpTimer();
		bool GetHosting(void)const;
	private:
		void forceSetMove(Vector2 dir);
		void move(double dt, TileMap* _map);
		void dive(double dt, TileMap* _map);
		void jump(double dt, TileMap* _map);
		void UpdateHost(double dt, TileMap* _map);
		void resetMove(void);
		void resetDive(void);
		void resetJump(void);
		void resetHost(void);

};

#endif
