#include "GhostEnemy.h"

const Vector3 GhostEnemy::BULLET_SCALE(2.0f, 2.0f, 2.0f);

const ElementalWeapon GhostEnemy::defaultWeapon[NUM_ELEMENTS] =
{
	ElementalWeapon("NormalWeapon", 10.0f, 500.0f, 400.0f, 0.0f, 0.0f, 10.0f, 9999, 9999, Elemental::NORMAL_TYPE, BULLET_SCALE),
	ElementalWeapon("LightningWeapon", 10.0f, 500.0f, 400.0f, 0.0f, 0.0f, 10.0f, 9999, 9999, Elemental::LIGHTNING_TYPE, BULLET_SCALE),
	ElementalWeapon("WaterWeapon", 10.0f, 500.0f, 400.0f, 0.0f, 0.0f, 10.0f, 9999, 9999, Elemental::WATER_TYPE, BULLET_SCALE),
	ElementalWeapon("EarthWeapon", 10.0f, 500.0f, 400.0f, 0.0f, 0.0f, 10.0f, 9999, 9999, Elemental::EARTH_TYPE, BULLET_SCALE),
};

GhostEnemy::GhostEnemy()
	: m_attacked(false)
{
}


GhostEnemy::~GhostEnemy()
{
}

void GhostEnemy::Init(CHAR_TYPE type, Mesh * mesh, Vector3 startPos, Vector3 scale, int moveSpeed, int maxHealth, ELEMENT_TYPE element/* = NORMAL_TYPE*/)
{
	CharObject::Init(type, mesh, startPos, scale, moveSpeed, maxHealth);
	m_element = element;

	// Give a weapon according to the element
	m_weapon = new ElementalWeapon(defaultWeapon[element]);
}

void GhostEnemy::Update(double dt)
{
	// Get a direction to the target
	Vector2 moveVector = Vector3(m_target.x, m_target.y) - m_transforms.Translation;
	if (moveVector != Vector2::ZERO_VECTOR)
	{
		moveVector.Normalize();
	}

	// Set the length to move
	moveVector = moveVector * m_moveSpeed * dt;

	m_transforms.Translation += moveVector;

	// Update the weapon
	if (m_weapon != NULL)
	{
		m_weapon->Update(dt);
	}

	// Shoot when possitble
	m_attacked = Attack();
}

bool GhostEnemy::Attack(void)
{
	if (Math::RandIntMinMax(0, 100) <= CHANCE_OF_ATTACKING)
	{
		return CharObject::Attack();
	}

	return false;
}

bool GhostEnemy::HadAttacked(void)
{
	return m_attacked;
}

GhostEnemy::ELEMENT_TYPE GhostEnemy::GetWeaponElement(void)
{
	ElementalWeapon* ew = dynamic_cast<ElementalWeapon*>(m_weapon);

	if (ew)
	{
		return ew->GetElement();
	}
	else
	{
		return NORMAL_TYPE;
	}
}