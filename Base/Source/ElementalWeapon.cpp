#include "ElementalWeapon.h"

ElementalWeapon::ElementalWeapon(void)
{

}

ElementalWeapon::ElementalWeapon(string name, float damage, float range, float bulletSpeed, float reloadSpeed, float fullReloadSpeed, float fireRate, short magSize, short reserveMag, ELEMENT_TYPE element, Vector3 bulletScale, Mesh * scopeMesh) : RangedWeapon(name, damage, range, bulletSpeed, reloadSpeed, fullReloadSpeed, fireRate, magSize, reserveMag, bulletScale, scopeMesh)
{
	m_element = element;
}

ElementalWeapon::~ElementalWeapon(void)
{

}