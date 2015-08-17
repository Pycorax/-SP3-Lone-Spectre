#include "TargetEnemy.h"



TargetEnemy::TargetEnemy()
	: m_rewardScore(100)
{
}


TargetEnemy::~TargetEnemy()
{
}

void TargetEnemy::Init(CHAR_TYPE type, Mesh * mesh, Vector3 startPos, Vector3 scale, int moveSpeed, int maxHealth, int rewardScore, ELEMENT_TYPE element)
{
	EnemyObject::Init(type, mesh, startPos, scale, moveSpeed, maxHealth);
	m_rewardScore = rewardScore;
	m_element = element;
	m_originalPos = startPos;
}

Vector3 TargetEnemy::GetOriginalPos(void)
{
	return m_originalPos;
}
