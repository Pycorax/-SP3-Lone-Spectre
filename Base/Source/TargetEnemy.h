#ifndef TARGET_ENEMY_H
#define TARGET_ENEMY_H

// Inheritance
#include "EnemyObject.h"
#include "Elemental.h"

class TargetEnemy : public EnemyObject, public Elemental
{
	int m_rewardScore;
	Vector3 m_originalPos;

	public:
		TargetEnemy();
		virtual ~TargetEnemy();

		virtual void Init(CHAR_TYPE type, Mesh* mesh, Vector3 startPos, Vector3 scale, int moveSpeed, int maxHealth, int rewardScore = 100, ELEMENT_TYPE element = NORMAL_TYPE);
		Vector3 GetOriginalPos(void);
};

#endif