#ifndef GHOST_ENEMY_H
#define GHOST_ENEMY_H

#include "EnemyObject.h"
#include "Elemental.h"
#include "ElementalWeapon.h"

class GhostEnemy : public EnemyObject, public Elemental
{
	protected:
		static const int CHANCE_OF_ATTACKING = 1;
		static const Vector3 BULLET_SCALE;

		static const ElementalWeapon defaultWeapon[NUM_ELEMENTS];
		bool m_attacked;
		
	public:
		GhostEnemy();
		virtual ~GhostEnemy();

		virtual void Init(CHAR_TYPE type, Mesh* mesh, Vector3 startPos, Vector3 scale, int moveSpeed, int maxHealth, ELEMENT_TYPE element = NORMAL_TYPE);
		virtual void Update(double dt);

		virtual bool Attack(void);
		bool HadAttacked(void);		// Call this after Update() to check if an attack had been called

		ELEMENT_TYPE GetWeaponElement(void);
};

#endif