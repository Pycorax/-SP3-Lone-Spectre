#include "CharObject.h"

#include "SpriteAnimation.h"
#include "ElementalWeapon.h"

CharObject::CharObject() : PhysicsObject()
	, m_bInMidAir_Down(false)
	, m_bInMidAir_Up(false)
	, m_jumpSpeed(0)
	, m_moveSpeed(0)
	, m_type(TOP_DOWN_PLAYER)
	, m_weapon(NULL)
	, m_health(0)
	, m_maxHealth(0)
	, m_animCount(0)
{
}


CharObject::~CharObject()
{
	if (m_weapon != NULL)
	{
		delete m_weapon;
		m_weapon = NULL;
	}
}

void CharObject::Init(CHAR_TYPE type, Mesh * mesh, Vector3 startPos, Vector3 scale, int moveSpeed, int maxHealth)
{
	PhysicsObject::Init(startPos, Vector3::ZERO_VECTOR, scale);
	SetMesh(mesh);
	m_type = type;
	m_moveSpeed = moveSpeed;
	m_lookDir.Set(1.0f, 0.0f);
	m_health = m_maxHealth = maxHealth;
}

void CharObject::Update(MapLayer * map, double dt)
{
	// Jump Controller
	if (m_type == SIDE_SCROLL_PLAYER)
	{
		static Vector3 oldPos = m_transforms.Translation;

		// Check if the hero can move up into mid air...
		int playerPosX = static_cast<int>((m_transforms.Translation.x + map->mapOffset.x) / map->map->GetTileSize());
		int playerPosY = static_cast<int>(map->map->GetNumOfTiles_Height() - static_cast<int>(ceil((m_transforms.Translation.y + map->mapOffset.y) / map->map->GetTileSize())));
		
		if (IsJumpUpwards())
		{
			updateJumpUpwards();

			// Check if the hero can move up into mid air...
			int playerPosX = static_cast<int>((m_transforms.Translation.x + map->mapOffset.x) / map->map->GetTileSize());
			int playerPosY = static_cast<int>(map->map->GetNumOfTiles_Height() - static_cast<int>(ceil((m_transforms.Translation.y + map->mapOffset.y) / map->map->GetTileSize())));
			playerPosX = Math::Clamp(playerPosX, 0, map->map->GetNumOfTiles_MapWidth());
			playerPosY = Math::Clamp(playerPosY, 0, map->map->GetNumOfTiles_MapHeight());

			if (
				map->map->TileAtPosIsWall(playerPosX, playerPosY - 1)
				||
				map->map->TileAtPosIsWall(playerPosX + 1, playerPosY - 1)
				)
			{
				// Since the new position does not allow the hero to move into, then go back to the old position
				m_transforms.Translation = oldPos;
				m_bInMidAir_Up = false;
				m_jumpSpeed = 0;
			}
		}
		else if (IsFreeFall())
		{
			updateFreefall();

			// Check if the hero can move up into mid air...
			int playerPosX = static_cast<int>((m_transforms.Translation.x + map->mapOffset.x) / map->map->GetTileSize());
			int playerPosY = static_cast<int>(map->map->GetNumOfTiles_Height() - static_cast<int>(ceil((m_transforms.Translation.y + map->mapOffset.y) / map->map->GetTileSize())));
			playerPosX = Math::Clamp(playerPosX, 0, map->map->GetNumOfTiles_MapWidth());
			playerPosY = Math::Clamp(playerPosY, 0, map->map->GetNumOfTiles_MapHeight());

			if (
				map->map->TileAtPosIsWall(playerPosX, playerPosY)
				||
				map->map->TileAtPosIsWall(playerPosX + 1, playerPosY)
				)
			{
				// Since the new position does not allow the hero to move into, then go back to the old position 
				m_transforms.Translation = oldPos;
				m_transforms.Translation.y = static_cast<int>(m_transforms.Translation.y) / map->map->GetTileSize() * map->map->GetTileSize() + 0.0001;
				m_bInMidAir_Down = false;
				m_jumpSpeed = 0;
			}
		}
		else if			// If is standing on nothing
			(
				map->map->TileAtPosIsWall(playerPosX, playerPosY + 1) == false
				&&
				map->map->TileAtPosIsWall(playerPosX + 1, playerPosY + 1) == false
			)
		{
			// Set to fall
			m_bInMidAir_Up = false;
			m_bInMidAir_Down = true;
		}

		oldPos = m_transforms.Translation;
	}

	// Update the Weapon
	m_weapon->Update(dt);
}

void CharObject::updateJumpUpwards(void)
{
	m_transforms.Translation.y += m_jumpSpeed;
	m_jumpSpeed -= 1;

	if (m_jumpSpeed <= 0)
	{
		m_jumpSpeed = 0;
		m_bInMidAir_Up = false;
		m_bInMidAir_Down = true;
	}
}

void CharObject::updateFreefall(void)
{
	m_transforms.Translation.y -= m_jumpSpeed;
	m_jumpSpeed += 1;
}


void CharObject::SetJumpUpwards(bool up)
{
	m_bInMidAir_Up = up;
}

void CharObject::SetFreeFall(bool down)
{
	m_bInMidAir_Down = down;
}

void CharObject::SetJumpSpeed(int speed)
{
	m_jumpSpeed = speed;
}

void CharObject::SetMoveSpeed(int speed)
{
	m_moveSpeed = speed;
}

void CharObject::SetType(CHAR_TYPE type)
{
	m_type = type;
}

void CharObject::SetWeapon(RangedWeapon* gun)
{
	m_weapon = gun;
}

void CharObject::AddToHealth(int healthDiff)
{
	m_health = Math::Clamp(m_health + healthDiff, 0, m_maxHealth);
}

void CharObject::SetHealth(int health)
{
	m_health = health;
}

void CharObject::SetMaxHealth(int maxHealth)
{
	m_maxHealth = maxHealth;
}

void CharObject::Kill(void)
{
	m_health = 0;
}

void CharObject::Revive(void)
{
	if (m_health <= 0)
	{
		m_health = m_maxHealth;
	}
}

// Getters

CharObject::CHAR_TYPE CharObject::GetCharType(void)
{
	return m_type;
}

Vector2 CharObject::GetTilePosition(MapLayer* map) const
{
	int checkPosition_X = static_cast<int>((map->mapOffset.x + m_transforms.Translation.x) / map->map->GetTileSize());
	int checkPosition_Y = static_cast<int>(map->map->GetNumOfTiles_Height() - static_cast<int>((m_transforms.Translation.y + map->map->GetTileSize()) / map->map->GetTileSize()));

	return Vector2(checkPosition_X, checkPosition_Y);
}

bool CharObject::IsOnGround() const
{
	return (m_bInMidAir_Up == false && m_bInMidAir_Down == false);
}

bool CharObject::IsJumpUpwards(void) const
{
	return (m_bInMidAir_Up == true && m_bInMidAir_Down == false);
}

bool CharObject::IsFreeFall(void) const
{
	return (m_bInMidAir_Up == false && m_bInMidAir_Down == true);
}

int CharObject::GetJumpSpeed(void) const
{
	return m_jumpSpeed;
}

int CharObject::GetMoveSpeed(void) const
{
	return m_moveSpeed;
}

Vector2 CharObject::GetLookDir(void)
{
	if (m_lookDir != Vector2::ZERO_VECTOR)
	{
		m_lookDir.Normalize();
	}

	return m_lookDir;
}

float CharObject::GetWeaponBulletSpeed(void) const
{
	if (m_weapon == NULL)
	{
		return 0.0f;
	}

	return m_weapon->GetBulletSpeed();
}

float CharObject::GetWeaponDamage(void) const
{
	if (m_weapon == NULL)
	{
		return 0.0f;
	}

	return m_weapon->GetDamage();
}

Vector3 CharObject::GetWeaponBulletScale(void) const
{
	if (m_weapon == NULL)
	{
		return Vector3::ZERO_VECTOR;
	}

	return m_weapon->GetBulletScale();
}

int CharObject::GetHealth(void) const
{
	return m_health;
}

int CharObject::GetMaxHealth(void) const
{
	return m_maxHealth;
}

bool CharObject::IsActive(void) const
{
	return m_health > 0;
}

void CharObject::Move(Vector2 dir, double dt, MapLayer* map)
{
	static Vector3 oldPos = m_transforms.Translation;

	if (dir.x < -Math::EPSILON)		// Left
	{
			m_transforms.Translation.x = m_transforms.Translation.x - static_cast<int>(m_moveSpeed * dt);

			if (m_type == SIDE_SCROLL_PLAYER)
			{
				// Flip the player around if going the other direction
				m_transforms.Rotation.y = 180.0f;
			}
			else if (m_type == TOP_DOWN_PLAYER)
			{
				m_transforms.Rotation.z = 90.0f;
			}

			// Animation
			SpriteAnimation* sa = dynamic_cast<SpriteAnimation*>(m_mesh);
			
			if (sa != NULL)
			{
				sa->PrevFrame();
			}

			int playerPosX = static_cast<int>((m_transforms.Translation.x + map->mapOffset.x) / map->map->GetTileSize());
			int playerPosY = static_cast<int>(map->map->GetNumOfTiles_Height() - static_cast<int>(ceil((m_transforms.Translation.y + map->mapOffset.y) / map->map->GetTileSize())));

			if (
				map->map->TileAtPosIsWall(playerPosX, playerPosY)
				||
				map->map->TileAtPosIsWall(playerPosX, playerPosY - 1)
				)
			{
				m_transforms.Translation.x = oldPos.x;
			}
	}
	else if (dir.x > Math::EPSILON)				// Right
	{
			m_transforms.Translation.x = m_transforms.Translation.x + static_cast<int>(m_moveSpeed * dt);

			if (m_type == SIDE_SCROLL_PLAYER)
			{
				// Flip the player around if going the other direction
				m_transforms.Rotation.y = 0.0f;
			}
			else if (m_type == TOP_DOWN_PLAYER)
			{
				m_transforms.Rotation.z = -90.0f;
			}

			// Animation
			SpriteAnimation* sa = dynamic_cast<SpriteAnimation*>(m_mesh);

			if (sa != NULL)
			{
				sa->NextFrame();
			}

			int playerPosX = static_cast<int>((m_transforms.Translation.x + map->mapOffset.x) / map->map->GetTileSize());
			int playerPosY = static_cast<int>(map->map->GetNumOfTiles_Height() - static_cast<int>(ceil((m_transforms.Translation.y + map->mapOffset.y) / map->map->GetTileSize())));

			if (
				map->map->TileAtPosIsWall(playerPosX + 1, playerPosY)
				||
				map->map->TileAtPosIsWall(playerPosX + 1, playerPosY - 1)
				)
			{
				m_transforms.Translation.x = oldPos.x;
			}
	}

	if (dir.y > Math::EPSILON)	// Up
	{
		m_transforms.Translation.y = m_transforms.Translation.y + static_cast<int>(m_moveSpeed * dt);

		if (m_type == TOP_DOWN_PLAYER)
		{
			m_transforms.Rotation.z = 0.0f;
		}

		int playerPosX = static_cast<int>((m_transforms.Translation.x + map->mapOffset.x) / map->map->GetTileSize());
		int playerPosY = static_cast<int>(map->map->GetNumOfTiles_Height() - static_cast<int>(ceil((m_transforms.Translation.y + map->mapOffset.y) / map->map->GetTileSize())));

		if (
			map->map->TileAtPosIsWall(playerPosX, playerPosY - 1)
			||
			map->map->TileAtPosIsWall(playerPosX + 1, playerPosY - 1)
			)
		{
			m_transforms.Translation.y = oldPos.y;
		}
	}
	else if (dir.y < -Math::EPSILON)		// Down
	{
		m_transforms.Translation.y = m_transforms.Translation.y - static_cast<int>(m_moveSpeed * dt);

		if (m_type == TOP_DOWN_PLAYER)
		{
			m_transforms.Rotation.z = 180.0f;
		}

		int playerPosX = static_cast<int>((m_transforms.Translation.x + map->mapOffset.x) / map->map->GetTileSize());
		int playerPosY = static_cast<int>(map->map->GetNumOfTiles_Height() - static_cast<int>(ceil((m_transforms.Translation.y + map->mapOffset.y) / map->map->GetTileSize())));

		if (
			map->map->TileAtPosIsWall(playerPosX, playerPosY)
			||
			map->map->TileAtPosIsWall(playerPosX + 1, playerPosY)
			)
		{
			m_transforms.Translation.y = oldPos.y;
		}
	}

	oldPos = m_transforms.Translation;

	// Controls m_lookDir
	static Vector2 oldLookDir(1.0f, 0.0f);

	if (dir == Vector2::ZERO_VECTOR)		// If direction was not changed this update, keep old direction
	{
		m_lookDir = oldLookDir;
	}
	else									// If direction was changed, update the old direction and the current direction
	{
		m_lookDir = oldLookDir = dir.Normalized();

		Elemental::ELEMENT_TYPE element = Elemental::NORMAL_TYPE;
		ElementalWeapon* weapon = dynamic_cast<ElementalWeapon*>(m_weapon);

		if (weapon != NULL)
		{
			element = weapon->GetElement();
		}

		m_animCount = Math::Wrap(++m_animCount, 0, static_cast<int>(m_animMeshes[element].size() - 1));
	}
}

void CharObject::Jump(void)
{
	if (m_type == SIDE_SCROLL_PLAYER && m_bInMidAir_Up == false && m_bInMidAir_Down == false)
	{
		m_bInMidAir_Up = true;
		m_jumpSpeed = INITIAL_JUMP_SPEED;
	}
}

bool CharObject::Attack(void)
{
	if (m_weapon == NULL)
	{
		return false;
	}

	return m_weapon->Attack();
}

void CharObject::Reload(void)
{
	if (m_weapon != NULL)
	{
		m_weapon->StartReload();
	}
}

void CharObject::AddAnimMesh(Mesh * mesh, Elemental::ELEMENT_TYPE element)
{
	m_animMeshes[element].push_back(mesh);
}

Mesh * CharObject::GetAnimMesh(void)
{
	Elemental::ELEMENT_TYPE element = Elemental::NORMAL_TYPE;
	ElementalWeapon* weapon = dynamic_cast<ElementalWeapon*>(m_weapon);

	if (weapon != NULL)
	{
		element = weapon->GetElement();
	}

	return m_animMeshes[element][m_animCount];
}
