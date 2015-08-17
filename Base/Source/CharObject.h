#ifndef CHAR_OBJECT_H
#define CHAR_OBJECT_H

// Inheritance
#include "PhysicsObject.h"

// Others
#include "RangedWeapon.h"
#include "MapLayer.h"

class CharObject : public PhysicsObject
{
	public:
		enum CHAR_TYPE
		{
			SIDE_SCROLL_PLAYER,
			TOP_DOWN_PLAYER,
			NUM_PLAYER
		};

	protected:
		static const int INITIAL_JUMP_SPEED = 18;

		// Type
		CHAR_TYPE m_type;

		// Movement
		bool m_bInMidAir_Up;
		bool m_bInMidAir_Down;
		int m_jumpSpeed;
		int m_moveSpeed;

		// Weapon
		Vector2 m_lookDir;
		RangedWeapon* m_weapon;

		// Health
		int m_health;
		int m_maxHealth;

		// Animation
		vector<Mesh*> m_animMeshes[Elemental::NUM_ELEMENTS];
		int m_animCount;

	public:
		CharObject();
		virtual ~CharObject();

		virtual void Init(CHAR_TYPE type, Mesh* mesh, Vector3 startPos, Vector3 scale, int moveSpeed, int maxHealth);
		virtual void Update(MapLayer* map, double dt);
		
		// Setters
		// -- Movement/Position
		void SetJumpUpwards(bool up);
		void SetFreeFall(bool down);
		void SetJumpSpeed(int speed);
		void SetMoveSpeed(int speed);
		void SetType(CHAR_TYPE type);
		// -- For Weapons
		void SetWeapon(RangedWeapon* gun);
		// -- Health
		void AddToHealth(int healthDiff);
		void SetHealth(int health);
		void SetMaxHealth(int maxHealth);
		void Kill(void);
		void Revive(void);

		// Getters
		CHAR_TYPE GetCharType(void);
		// -- Movement/Position
		Vector2 GetTilePosition(MapLayer* map) const;
		bool IsOnGround(void) const;
		bool IsJumpUpwards(void) const;
		bool IsFreeFall(void) const;
		int GetJumpSpeed(void) const;
		int GetMoveSpeed(void) const;
		// -- For Weapons
		Vector2 GetLookDir(void);
		float GetWeaponBulletSpeed(void) const;
		float GetWeaponDamage(void) const;
		Vector3 GetWeaponBulletScale(void) const;
		// -- Health
		int GetHealth(void) const;
		int GetMaxHealth(void) const;

		virtual bool IsActive(void) const;

		// Actions
		void Move(Vector2 dir, double dt, MapLayer* map);
		void Jump(void);
		virtual bool Attack(void);
		void Reload(void);

		// Animation
		void AddAnimMesh(Mesh* mesh, Elemental::ELEMENT_TYPE);
		Mesh* GetAnimMesh(void);

	protected:
		// Update Helper Functions
		void updateJumpUpwards(void);
		void updateFreefall(void);
};

#endif