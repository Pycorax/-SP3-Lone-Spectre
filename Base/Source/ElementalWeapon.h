#ifndef ELEMENTAL_WEAPON_H
#define ELEMENTAL_WEAPON_H

#include "RangedWeapon.h"
#include "Elemental.h"

class ElementalWeapon : public RangedWeapon, public Elemental
{
	public:
		ElementalWeapon();
		ElementalWeapon(string name, float damage, float range, float bulletSpeed, float reloadSpeed, float fullReloadSpeed, float fireRate, short magSize, short reserveMag, ELEMENT_TYPE element, Vector3 bulletScale = Vector3(1.0f, 1.0f, 1.0f), Mesh* scopeMesh = NULL);
		virtual ~ElementalWeapon();
};

#endif