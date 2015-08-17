#include "MVC_MAddon_TileSys.h"

const float MVC_MAddon_TileSys::SCROLL_SPEED = 400.0f;

MVC_MAddon_TileSys::MVC_MAddon_TileSys(void)
	: m_background(NULL)
	, m_hero(PlayerObject::GetInstance())
{	
}

MVC_MAddon_TileSys::~MVC_MAddon_TileSys()
{
}

void MVC_MAddon_TileSys::Update(void)
{
	if (m_renderList2DFirst.size() > 0)
	{
		m_renderList2DFirst.clear();
	}
}

void MVC_MAddon_TileSys::Exit(void)
{
	if (m_background)
	{
		delete m_background;
		m_background = NULL;
	}

	if (m_heroGO)
	{
		delete m_heroGO;
		m_heroGO = NULL;
	}

	for (vector<MapLayer*>::iterator it = m_mapLayers.begin(); it != m_mapLayers.end(); ++it)
	{
		delete (*it)->map;
	}

	while (goodieList.size() > 0)
	{
		delete goodieList.back();
		goodieList.pop_back();
	}
}


Mesh * MVC_MAddon_TileSys::GetTileMesh(TILE_TYPE tileType)
{
	if (tileType >= 0 && tileType < NUM_TILE)
	{
		return m_tileMesh[tileType];
	}
	else
	{
		return NULL;
	}
}

vector<MapLayer*> MVC_MAddon_TileSys::GetMapLayers()
{
	return m_mapLayers;
}

const vector<GameObject*>& MVC_MAddon_TileSys::Get2DFirstRenderList(void) const
{
	return m_renderList2DFirst;
}

void MVC_MAddon_TileSys::heroUpdate(double dt)
{
	m_hero->Update(m_map, dt);

	// Update the GameObject with the new postion
	Vector2 heroPosV2 = m_hero->GetTransform().Translation.ToVector2();
	m_heroGO->SetPos(heroPosV2);
	m_heroGO->SetRot(m_hero->GetTransform().Rotation);

	// Shift position to correct position if rotated
	if (m_heroGO->GetTransform().Rotation.y == 180.0f)
	{
		heroPosV2.x += m_map->map->GetTileSize();
	}

	if (m_heroGO->GetTransform().Rotation.z == 90.0f)
	{
		heroPosV2.x += m_map->map->GetTileSize();
	}
	else if (m_heroGO->GetTransform().Rotation.z == -90.0f)
	{
		heroPosV2.y += m_map->map->GetTileSize();
	}
	else if (m_heroGO->GetTransform().Rotation.z == 180.0f)
	{
		heroPosV2.x += m_map->map->GetTileSize();
		heroPosV2.y += m_map->map->GetTileSize();
	}

	m_heroGO->SetPos(heroPosV2);

	// Update the Hero Mesh
	if (m_hero->GetCharType() == CharObject::TOP_DOWN_PLAYER)
	{
		m_heroGO->SetMesh(m_hero->GetAnimMesh());
	}
	
	// Check if collision with bullets
	for (std::vector<BulletObject *>::iterator it = m_bulletList.begin(); it != m_bulletList.end(); ++it)
	{
		BulletObject *bullet = (BulletObject *)*it;
		if (m_hero->CheckCollision(bullet, dt))
		{
			m_hero->AddToHealth(-(bullet->GetDamage()));
			bullet->SetActive(false);
		}
	}
}

void MVC_MAddon_TileSys::sidescrollUpdate()
{
	for (vector<MapLayer*>::iterator it = m_mapLayers.begin(); it != m_mapLayers.end(); ++it)
	{
		(*it)->Update(m_map);
	}
}

void MVC_MAddon_TileSys::bulletUpdate(double dt)
{
	for (std::vector<BulletObject *>::iterator it = m_bulletList.begin(); it != m_bulletList.end(); ++it)
	{
		BulletObject *bullet = (BulletObject *)*it;
		if (bullet->IsActive())
		{
			bullet->Update(dt);
		}
	}
}

BulletObject * MVC_MAddon_TileSys::FetchBullet()
{
	// Look for an inactive bullet that can be used
	for (std::vector<BulletObject *>::iterator it = m_bulletList.begin(); it != m_bulletList.end(); ++it)
	{
		BulletObject *bullet = (BulletObject *)*it;
		if (!bullet->IsActive())
		{
			bullet->SetActive(true);
			return bullet;
		}
	}

	const unsigned BATCH_PRODUCE = 10;

	// Produce some so that we don't have to keep producing everytime we need one
	BulletObject *bullet;
	for (unsigned i = 0; i < BATCH_PRODUCE; ++i)
	{
		bullet = new BulletObject();
		m_bulletList.push_back(bullet);
	}

	// Return the last bullet to the caller
	bullet->SetActive(true);

	return bullet;
}

Vector2 MVC_MAddon_TileSys::screenToTileSpace(Vector2 pos)
{
	Vector2 result;
	result.x = pos.x / m_map->map->GetTileSize();
	result.y = m_map->map->GetNumOfTiles_Height() - static_cast<int>(ceil(pos.y / m_map->map->GetTileSize()));

	return result;
}