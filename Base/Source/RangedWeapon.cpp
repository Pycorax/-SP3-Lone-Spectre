#include "RangedWeapon.h"

const double RangedWeapon::NO_ADS_FOV = -1.0;
const short RangedWeapon::MAX_UPGRADES = 3;

RangedWeapon::RangedWeapon() : Weapon()
	, m_fBulletSpeed(1000.0f)
	, m_bulletScale(Vector3(1.0f, 1.0f, 1.0f))
	, m_sCurrentMag(0)
	, m_fLastShotDT(0.0f)
	, m_fReloadSpeed(0.0f)
	, m_fFullReloadSpeed(0.0f)
	, m_bReloading(false)
	, m_fReloadTimer(0.0f)
	, m_fFireRate(200.0f)
	, m_scope(NULL)
	, m_adsFOV(NO_ADS_FOV)
	, m_upgradeTimes(0)
{
	m_type = RANGED_TYPE;
}

RangedWeapon::RangedWeapon(string name, float damage, float range, float bulletSpeed, float reloadSpeed, float fullReloadSpeed, float fireRate, short magSize, short reserveMag, Vector3 bulletScale /*= Vector3(1.0f, 1.0f, 1.0f)*/, Mesh* scopeMesh /* = NULL*/) : Weapon(name, damage, range)
	, m_fBulletSpeed(bulletSpeed)
	, m_bulletScale(bulletScale)
	, m_scope(scopeMesh)
	, m_sCurrentMag(0)
	, m_fLastShotDT(0.0f)
	, m_fReloadSpeed(reloadSpeed)
	, m_fFullReloadSpeed(fullReloadSpeed)
	, m_bReloading(false)
	, m_fReloadTimer(0.0f)
	, m_adsFOV(NO_ADS_FOV)
	, m_upgradeTimes(0)
{
	m_type = RANGED_TYPE;
	SetFireRate(fireRate);
	SetMagSize(magSize, reserveMag);
}

RangedWeapon::~RangedWeapon()
{
}

// Setters
void RangedWeapon::SetBullet(float bulletSpeed /* = 1000.0f */, Vector3 bulletScale /* = Vector3() */)
{
	m_fBulletSpeed = bulletSpeed;
	m_bulletScale = bulletScale;
}

void RangedWeapon::SetReloadSpeed(float reloadSpeed/* = 0.0f */, float fullReloadSpeed/* = 0.0f*/)
{
	if (reloadSpeed < 0.0f || fullReloadSpeed < 0.0f)
	{
		reloadSpeed = 0.0f;
		fullReloadSpeed = 0.0f;
	}
	
	m_fReloadSpeed = reloadSpeed;
	m_fFullReloadSpeed = fullReloadSpeed;
}

void RangedWeapon::SetFireRate(float fireRate/* = 200.0f*/)
{
	if (fireRate < 0)
	{
		fireRate = 200.0f;
	}

	m_fFireRate = fireRate;
	m_fShotsDT = 60.0f /*Seconds in a minute*/ / m_fFireRate;
}

void RangedWeapon::SetMagSize(short magSize, int reserveMag)
{
	if (magSize < 0 || reserveMag < 0)
	{
		magSize = 0;
		reserveMag = 0;
	}

	m_sMagSize = magSize;
	m_nReserveMag = m_nDefaultReserveMag = reserveMag;

	reload();
}

void RangedWeapon::SetScope(Mesh * mesh)
{
	m_scope = mesh;
}

void RangedWeapon::SetADSFOV(double fov)
{
	m_adsFOV = fov;
}

void RangedWeapon::SetADSOffset(Vector3 offset)
{
	m_adsOffset = offset;
}

// Getters
float RangedWeapon::GetBulletSpeed(void)
{
	return m_fBulletSpeed;
}

Vector3 RangedWeapon::GetBulletScale(void)
{
	return m_bulletScale;
}

short RangedWeapon::GetMagSize(void)
{
	return m_sMagSize;
}

float RangedWeapon::GetReloadSpeed()
{
	if (m_sCurrentMag <= 0)
	{
		return m_fFullReloadSpeed;
	}
	else
	{
		return m_fReloadSpeed;
	}
}

float RangedWeapon::GetFireRate(void)
{
	return m_fFireRate;
}

short RangedWeapon::GetCurrentMag(void)
{
	return m_sCurrentMag;
}

short RangedWeapon::GetReserveMag(void)
{
	return m_nReserveMag;
}

Mesh * RangedWeapon::GetScope(void)
{
	return m_scope;
}

double RangedWeapon::GetADSFOV(void)
{
	return m_adsFOV;
}

Vector3 RangedWeapon::GetADSOffset(void)
{
	return m_adsOffset;
}

// Others
bool RangedWeapon::Attack(void)
{
	if (m_fLastShotDT > m_fShotsDT && m_sCurrentMag > 0)
	{
		m_fLastShotDT = 0.0f;
		--m_sCurrentMag;

		return true;
	}

	return false;
}

void RangedWeapon::StartReload()
{
	if (!m_bReloading)
	{
		// Don't allow reload if mag is full or no more bullets
		if (m_sCurrentMag == m_sMagSize || m_nReserveMag <= 0)
		{
			return;
		}

		// Set to reload
		m_bReloading = true;

		if (m_sCurrentMag > 0)			// If it is a tactical reload (mag size > 0)
		{
			m_fReloadTimer = m_fReloadSpeed;
		}
		else
		{
			m_fReloadTimer = m_fFullReloadSpeed;
		}
	}
}

bool RangedWeapon::RefillAmmo(void)
{
	if (m_nReserveMag == m_nDefaultReserveMag)
	{
		return false;
	}
	else
	{
		m_nReserveMag = m_nDefaultReserveMag;
		return true;
	}
}

bool RangedWeapon::Upgrade(void)
{
	if (m_upgradeTimes >= MAX_UPGRADES)
	{
		return false;
	}

	if (m_upgradeTimes == 0)
	{
		m_name += " Mk. I";
	}
	else
	{
		m_name += "I";
	}

	const float MAGSIZE_MULT = 1.10f;
	const float DAMAGE_MULT = 1.20f;
	const float FIRERATE_MULT = 1.10f;
	const float RELOAD_MULT = 0.5f;

	m_sMagSize = m_sMagSize * MAGSIZE_MULT + 0.5;
	m_fDamage = m_fDamage * DAMAGE_MULT;
	m_fFireRate = m_fFireRate * FIRERATE_MULT;
	m_fReloadSpeed = m_fReloadSpeed * RELOAD_MULT;
	m_fFullReloadSpeed = m_fFullReloadSpeed * RELOAD_MULT;

	++m_upgradeTimes;

	return true;
}

void RangedWeapon::Update(double dt)
{
	// Controls fire rate
	m_fLastShotDT += static_cast<float>(dt);

	// Reload Timer
	if (m_bReloading)
	{
		// *Reload Animation*
		m_fReloadTimer -= dt;

		// Finished "animation"
		if (m_fReloadTimer <= 0.0f)
		{
			// Stop the reload process
			m_bReloading = false;

			reload();
		}
	}
}

void RangedWeapon::reload(void)
{
	// Dump current into reserve
	m_nReserveMag += m_sCurrentMag;

	// From the resulting reserve, take 1 set out
	if (m_nReserveMag >= m_sMagSize)		// If got 1 or more magazine remaining
	{
		m_nReserveMag -= m_sMagSize;
		m_sCurrentMag = m_sMagSize;
	}
	else									// If got less than 1 magazine remaining
	{
		m_sCurrentMag = m_nReserveMag;
		m_nReserveMag = 0;
	}
}
