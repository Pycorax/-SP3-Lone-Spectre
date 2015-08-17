#ifndef RANGED_WEAPON_H
#define RANGED_WEAPON_H

#include "Weapon.h"
#include "Vector3.h"

class RangedWeapon : public Weapon
{
	static const short MAX_UPGRADES;

	// Bullet Stats
	float	m_fBulletSpeed;
	Vector3 m_bulletScale;
	
	// Reload Speed
	float	m_fFullReloadSpeed;
	float	m_fReloadSpeed;
	bool	m_bReloading;
	float	m_fReloadTimer;

	// Fire rate
	float	m_fFireRate;
	float	m_fShotsDT;
	float	m_fLastShotDT;

	// Ammo
	short	m_sMagSize;
	short	m_sCurrentMag;
	short	m_nReserveMag;
	short	m_nDefaultReserveMag;

	// ADS
	Mesh*	m_scope;
	double	m_adsFOV;
	Vector3 m_adsOffset;

	// Upgrades
	short	m_upgradeTimes;

	public:
		static const double NO_ADS_FOV;

		RangedWeapon();
		RangedWeapon(string name, float damage, float range, float bulletSpeed, float reloadSpeed, float fullReloadSpeed, float fireRate, short magSize, short reserveMag, Vector3 bulletScale = Vector3(1.0f, 1.0f, 1.0f), Mesh* scopeMesh = NULL);
		virtual ~RangedWeapon();

		// Setters
		void SetBullet(float bulletSpeed = 1000.0f, Vector3 bulletScale = Vector3(1.0f, 1.0f, 1.0f));
		void SetReloadSpeed(float reloadSpeed = 0.0f, float fullReloadSpeed = 0.0f);
		void SetFireRate(float fireRate = 200.0f);
		void SetMagSize(short magSize, int reserveMag);
		void SetScope(Mesh* mesh = NULL);
		void SetADSFOV(double fov);
		void SetADSOffset(Vector3 offset);

		// Getters
		float GetBulletSpeed(void);
		Vector3 GetBulletScale(void);
		short GetMagSize(void);
		float GetReloadSpeed(void);
		float GetFireRate(void);
		short GetCurrentMag(void);
		short GetReserveMag(void);
		Mesh* GetScope(void);
		double GetADSFOV(void);
		Vector3 GetADSOffset(void);

		// Others
		bool Attack(void);
		void StartReload(void);
		bool RefillAmmo(void);
		bool Upgrade(void);

		void Update(double dt);

	private:
		void reload(void);
};

#endif