#include "MVC_Model_Mario.h"

#include "Strategy_Kill.h"
#include "Strategy_Safe.h"

#include "TreasureGoodie.h"
#include "HealthGoodie.h"

const float MVC_Model_Mario::GHOST_MOVE_SPEED = 25.0f;
const float MVC_Model_Mario::BULLET_LIFETIME = 10.0f;
const float MVC_Model_Mario::CONTACT_DMG_MULTIPLIER = 2.0f;
const float MVC_Model_Mario::VULNERABLE_DMG_MULTIPLIER = 2.5f;
const Vector3 MVC_Model_Mario::PLAYER_BULLET_SCALE(1.0f, 1.0f, 1.0f);
const Vector3 MVC_Model_Mario::ENEMY_BULLET_SCALE(2.0f, 2.0f, 2.0f);

MVC_Model_Mario::MVC_Model_Mario(string configSONFile) : MVC_Model(configSONFile), MVC_MAddon_TileSys()
	, m_gameState(LEVEL_STATE)
	, m_currentLevel(0)
	, m_levelType(CharObject::TOP_DOWN_PLAYER)
{
}


MVC_Model_Mario::~MVC_Model_Mario()
{
}

void MVC_Model_Mario::Init(void)
{
	MVC_Model::Init();

	// Init Level List
	m_levelPaths.push_back("Levels//Level0.son");
	m_levelPaths.push_back("Levels//Level1.son");
	m_levelPaths.push_back("Levels//Level2.son");
	m_levelPaths.push_back("Levels//Level3.son");
	m_levelPaths.push_back("Levels//Level6.son");

	// Init Buttons
	initButtons();

	// Init Bullet Meshes
	m_bulletMesh[Elemental::NORMAL_TYPE] = GetMeshResource("Bullet");
	m_bulletMesh[Elemental::LIGHTNING_TYPE] = GetMeshResource("LightningBullet");
	m_bulletMesh[Elemental::WATER_TYPE] = GetMeshResource("WaterBullet");
	m_bulletMesh[Elemental::EARTH_TYPE] = GetMeshResource("EarthBullet");

	// Init Enemy Meshes
	m_enemyMesh[Elemental::NORMAL_TYPE] = GetMeshResource("Ghost");
	m_enemyMesh[Elemental::LIGHTNING_TYPE] = GetMeshResource("Ghost_Lightning");
	m_enemyMesh[Elemental::WATER_TYPE] = GetMeshResource("Ghost_Water");
	m_enemyMesh[Elemental::EARTH_TYPE] = GetMeshResource("Ghost_Earth");

	// Init HP Bar Mesh
	m_hpBarMesh = GetMeshResource("HealthBar");

	// Init Target Meshes
	m_targetMesh[Elemental::NORMAL_TYPE] = GetMeshResource("Target");
	m_targetMesh[Elemental::LIGHTNING_TYPE] = GetMeshResource("LightningTarget");
	m_targetMesh[Elemental::WATER_TYPE] = GetMeshResource("WaterTarget");
	m_targetMesh[Elemental::EARTH_TYPE] = GetMeshResource("EarthTarget");

	// Init Hero Meshes
	m_heroMesh[CharObject::TOP_DOWN_PLAYER][Elemental::NORMAL_TYPE] = GetMeshResource("TopDown_Hero_0");
	m_heroMesh[CharObject::TOP_DOWN_PLAYER][Elemental::LIGHTNING_TYPE] = GetMeshResource("TopDown_Hero_Lightning_0");
	m_heroMesh[CharObject::TOP_DOWN_PLAYER][Elemental::WATER_TYPE] = GetMeshResource("TopDown_Hero_Water_0");
	m_heroMesh[CharObject::TOP_DOWN_PLAYER][Elemental::EARTH_TYPE] = GetMeshResource("TopDown_Hero_Earth_0");
	m_heroMesh[CharObject::SIDE_SCROLL_PLAYER][Elemental::NORMAL_TYPE] = GetMeshResource("SideScroll_Hero");
	m_heroMesh[CharObject::SIDE_SCROLL_PLAYER][Elemental::LIGHTNING_TYPE] = GetMeshResource("SideScroll_Hero_Lightning");
	m_heroMesh[CharObject::SIDE_SCROLL_PLAYER][Elemental::WATER_TYPE] = GetMeshResource("SideScroll_Hero_Water");
	m_heroMesh[CharObject::SIDE_SCROLL_PLAYER][Elemental::EARTH_TYPE] = GetMeshResource("SideScroll_Hero_Earth");

	// Init Hero Weapons
	m_heroWeapon[Elemental::NORMAL_TYPE] = new ElementalWeapon("Cannon", 34.0f, 500.0f, 800.0f, 2.0f, 3.0f, 500.0f, 999, 9999, Elemental::NORMAL_TYPE);
	m_heroWeapon[Elemental::LIGHTNING_TYPE] = new ElementalWeapon("Lightning Cannon", 20.0f, 1000.0f, 800.0f, 2.0f, 3.0f, 1000.0f, 999, 9999, Elemental::LIGHTNING_TYPE);
	m_heroWeapon[Elemental::WATER_TYPE] = new ElementalWeapon("Water Cannon", 30.0f, 500.0f, 500.0f, 2.0f, 3.0f, 750.0f, 999, 9999, Elemental::WATER_TYPE);;
	m_heroWeapon[Elemental::EARTH_TYPE] = new ElementalWeapon("Earth Cannon", 45.0f, 500.0f, 300.0f, 2.0f, 3.0f, 200.0f, 999, 9999, Elemental::EARTH_TYPE);;

	// Init Hero Position
	m_hero->Init(CharObject::TOP_DOWN_PLAYER, NULL, Vector3::ZERO_VECTOR, Vector2(10.0f, 10.0f), 200, 100);
	// TODO: Remove god mode weapon
	m_hero->SetWeapon(m_heroWeapon[Elemental::NORMAL_TYPE]);

	// Init Hero Character
	m_heroGO = new GameObject(m_heroMesh[m_levelType][m_hero->GetWeaponElement()], false);
	m_heroHPBar = new GameObject(m_hpBarMesh, false);

	m_hero->AddAnimMesh(GetMeshResource("TopDown_Hero_0"), Elemental::NORMAL_TYPE);
	m_hero->AddAnimMesh(GetMeshResource("TopDown_Hero_1"), Elemental::NORMAL_TYPE);
	m_hero->AddAnimMesh(GetMeshResource("TopDown_Hero_2"), Elemental::NORMAL_TYPE);

	m_hero->AddAnimMesh(GetMeshResource("TopDown_Hero_Lightning_0"), Elemental::LIGHTNING_TYPE);
	m_hero->AddAnimMesh(GetMeshResource("TopDown_Hero_Lightning_1"), Elemental::LIGHTNING_TYPE);
	m_hero->AddAnimMesh(GetMeshResource("TopDown_Hero_Lightning_2"), Elemental::LIGHTNING_TYPE);

	m_hero->AddAnimMesh(GetMeshResource("TopDown_Hero_Water_0"), Elemental::WATER_TYPE);
	m_hero->AddAnimMesh(GetMeshResource("TopDown_Hero_Water_1"), Elemental::WATER_TYPE);
	m_hero->AddAnimMesh(GetMeshResource("TopDown_Hero_Water_2"), Elemental::WATER_TYPE);

	m_hero->AddAnimMesh(GetMeshResource("TopDown_Hero_Earth_0"), Elemental::EARTH_TYPE);
	m_hero->AddAnimMesh(GetMeshResource("TopDown_Hero_Earth_1"), Elemental::EARTH_TYPE);
	m_hero->AddAnimMesh(GetMeshResource("TopDown_Hero_Earth_2"), Elemental::EARTH_TYPE);

	initEnvironment();
	
	// Set up the first state
	switchState(m_gameState);
}

void MVC_Model_Mario::Update(double dt)
{
	MVC_Model::Update(dt);
	MVC_MAddon_TileSys::Update();
	
	switch (m_gameState)
	{
		case LEVEL_STATE:
			levelUpdate(dt);
			break;
		case WIN_STATE:
			winUpdate(dt);
			break;
		case LOSE_STATE:
			loseUpdate(dt);
			break;
	}
}

void MVC_Model_Mario::Exit(void)
{
	// Delete Enemies
	while (m_ghosts.size() > 0)
	{
		if (m_ghosts.back() != NULL)
		{
			delete m_ghosts.back();
			m_ghosts.pop_back();
		}
	}

	// Delete Targets
	while (m_targets.size() > 0)
	{
		if (m_targets.back() != NULL)
		{
			delete m_targets.back();
			m_targets.pop_back();
		}
	}

	// Delete Hero Weapons
	for (size_t i = 0; i < Elemental::NUM_ELEMENTS; ++i)
	{
		if (m_heroWeapon[i] != NULL)
		{
			delete m_heroWeapon[i];
			m_heroWeapon[i] = NULL;
		}
	}

	// Background, hero and MapLayers will be deleted in MVC_MAddon_TileSys
	MVC_MAddon_TileSys::Exit();
	MVC_Model::Exit();
}

void MVC_Model_Mario::levelInit(void)
{
	if (m_currentLevel > m_levelPaths.size())
	{
		m_currentLevel = 0;
	}

	initLevel(m_levelPaths[m_currentLevel]);
}

void MVC_Model_Mario::initButtons(void)
{
	const Vector3 NORMAL_BUTTON_SIZE(250, 50);

	m_buttons[CONTINUE_BUTTON] = new Button();
	m_buttons[CONTINUE_BUTTON]->button.Init(NULL, Vector3((m_viewWidth - NORMAL_BUTTON_SIZE.x) * 0.5, (m_viewHeight - NORMAL_BUTTON_SIZE.y) * 0.5), NORMAL_BUTTON_SIZE);
	m_buttons[CONTINUE_BUTTON]->buttonRep = new GameObject(GetMeshResource("BT_Continue"));
	m_buttons[CONTINUE_BUTTON]->buttonRep->SetPos(m_buttons[CONTINUE_BUTTON]->button.GetPosition());
}

void MVC_Model_Mario::clearLevel(void)
{
	// Reset all Bullets
	for (vector<BulletObject*>::iterator bullet = m_bulletList.begin(); bullet != m_bulletList.end(); ++bullet)
	{
		(*bullet)->SetActive(false);
	}

	// Reset all Enemies
	for (vector<Ghost*>::iterator ghostBox = m_ghosts.begin(); ghostBox != m_ghosts.end(); ++ghostBox)
	{
		(*ghostBox)->ghost->Kill();
	}

	// Reset all Targets
	for (vector<Target*>::iterator targetBox = m_targets.begin(); targetBox != m_targets.end(); ++targetBox)
	{
		(*targetBox)->target->Kill();
	}

	// Delete the TileMaps
	while (m_mapLayers.size() > 0)
	{
		delete m_mapLayers.back();
		m_mapLayers.pop_back();
	}
}

void MVC_Model_Mario::initLevel(string levelFile)
{
	clearLevel();
	loadLevel(levelFile);

	// Set the hero type
	m_hero->SetType(m_levelType);

	// Set the Spawn Position
	m_hero->SetPos(m_map->map->GetSpawnPos());

	// Reset Rotation
	m_hero->SetRot(Vector3::ZERO_VECTOR);

	// Reset hero health
	m_hero->SetHealth(m_hero->GetMaxHealth());

	// Update the hero mesh
	m_heroGO->SetMesh(m_heroMesh[m_levelType][m_hero->GetWeaponElement()]);

	// Init Targets
	vector<Vector2> targetPos = m_map->map->GetTargetPositions();
	vector<Elemental::ELEMENT_TYPE> targetElements = m_map->map->GetTargetElements();
	vector<Elemental::ELEMENT_TYPE>::iterator element = targetElements.begin();
	for (vector<Vector2>::iterator target = targetPos.begin(); target != targetPos.end(); ++target, ++element)
	{
		spawnTarget(*target, 16, TARGET_HEALTH, TARGET_KILL_SCORE, *element);
	}
}

void MVC_Model_Mario::loadLevel(string levelFile)
{
	// Retrieve data as a Branch object from the SONFile
	Branch level = SONIO::LoadSON(levelFile);

	const string ROOT_NAME = "LevelContainer";

	if (level.name == ROOT_NAME)
	{
		for (vector<Attribute>::iterator attrib = level.attributes.begin(); attrib != level.attributes.end(); ++attrib)
		{
			if (attrib->name == "LevelType")
			{
				if (attrib->value == "TOP_DOWN_TYPE")
				{
					m_levelType = CharObject::TOP_DOWN_PLAYER;
				}
				else if (attrib->value == "SIDE_SCROLL_TYPE")
				{
					m_levelType = CharObject::SIDE_SCROLL_PLAYER;
				}
			}
			else if (attrib->name == "MaxGhosts")
			{
				m_levelMaxEnemies = stoi(attrib->value);
			}
		}

		// Maps
		for (vector<Branch>::iterator branch = level.childBranches.begin(); branch != level.childBranches.end(); ++branch)
		{
			if (branch->name == "Map")
			{
				// Init Tile Map
				MapLayer* levelMap = loadMapLayer(*branch);
				levelMap->isLevelMap = true;
				m_mapLayers.push_back(levelMap);

				// Assign the handle to the levelMap
				m_map = levelMap;
			}
			else if (branch->name == "BGMap")
			{
				// Init BG Tile Map
				MapLayer* backBG = loadMapLayer(*branch);
				m_mapLayers.push_back(backBG);
			}
		}
	}
}

MapLayer * MVC_Model_Mario::loadMapLayer(Branch mapLayer)
{
	string filePath;
	float multiplier = 1.0f;

	enum MAPLAYER_PROPERTY_TYPE
	{
		TILE_MAP_WIDTH_PROP,
		TILE_MAP_HEIGHT_PROP,
		MAP_WIDTH_PROP,
		MAP_HEIGHT_PROP,
		TILE_SIZE_PROP,
		NUM_PROP
	};

	string mapLayerPropNames[NUM_PROP] =
	{
		"TileMapWidth",
		"TileMapHeight",
		"MapWidth",
		"MapHeight",
		"TileSize"
	};
	int mapLayerProps[NUM_PROP];
	
	MapLayer* levelMap = new MapLayer;
	levelMap->map = new CMap;

	for (vector<Attribute>::iterator attrib = mapLayer.attributes.begin(); attrib != mapLayer.attributes.end(); ++attrib)
	{
		if (attrib->name == "FilePath")
		{
			filePath = attrib->value;
		}
		else if (attrib->name == "ScrollMultiplier")
		{
			multiplier = stof(attrib->value);
		}
		else
		{
			for (size_t i = 0; i < NUM_PROP; ++i)
			{
				if (attrib->name == mapLayerPropNames[i])
				{
					mapLayerProps[i] = stoi(attrib->value);
				}
			}
		}
	}

	levelMap->map->Init(m_viewHeight, m_viewWidth, mapLayerProps[TILE_MAP_HEIGHT_PROP], mapLayerProps[TILE_MAP_WIDTH_PROP], mapLayerProps[MAP_HEIGHT_PROP], mapLayerProps[TILE_MAP_WIDTH_PROP], mapLayerProps[TILE_SIZE_PROP]);
	levelMap->scrollModifier = multiplier;
	levelMap->map->LoadMap(filePath);

	return levelMap;
}

void MVC_Model_Mario::initEnvironment(void)
{
	// Init Background
	m_background = new GameObject(GetMeshResource("Background"), false);

	// Init Tile Meshes
	m_tileMesh[TILE_GROUND] = GetMeshResource("Tile_Ground");
	m_tileMesh[TILE_STRUCTURE] = GetMeshResource("Tile_Structure");
	m_tileMesh[TILE_DARKBLOCK] = GetMeshResource("Tile_DarkBlock");
	m_tileMesh[TILE_INVISIWALL] = NULL;
	m_tileMesh[TILE_EXIT] = GetMeshResource("Tile_Exit");
}

void MVC_Model_Mario::levelUpdate(double dt)
{
	levelController();

	// Don't run everything else if this level is already done
	if (m_gameState != LEVEL_STATE)
	{
		return;
	}

	heroUpdate(dt);
	updateHeroHealthBar();

	sidescrollUpdate();

	enemyUpdate(dt);
	targetUpdate(dt);

	// Cout the current tile i'm on
	Vector3 pos = m_hero->GetTransform().Translation;

	// Render the background
	m_renderList2DFirst.push_back(m_background);
	// Render the FPS Count
	m_renderList2D.push_back(m_fpsCount);

	// Render the goodies
	for (vector<GoodieObject*>::iterator it = goodieList.begin(); it != goodieList.end(); ++it)
	{
		m_renderList2D.push_back(*it);
	}

	// Update bullets and send it to be rendered
	bulletUpdate(dt);

	// Render the hero
	m_renderList2D.push_back(m_heroGO);
	m_renderList2D.push_back(m_heroHPBar);

	// Render Targets
	pushTargetsToRender();

	// Render Enemies
	pushEnemiesToRender();
}

void MVC_Model_Mario::winUpdate(double dt)
{
	if (m_buttons[CONTINUE_BUTTON]->button.GetState() == UIButton::DOWN_STATE)
	{
		switchState(LEVEL_STATE);
	}

	// Render Background
	m_renderList2DFirst.push_back(m_background);

	// Render the Button
	pushUIButtonToRender(m_buttons[CONTINUE_BUTTON]);
}

void MVC_Model_Mario::loseUpdate(double dt)
{
	if (m_buttons[CONTINUE_BUTTON]->button.GetState() == UIButton::DOWN_STATE)
	{
		switchState(LEVEL_STATE);
	}

	// Render Background
	m_renderList2DFirst.push_back(m_background);

	// Render the Button
	pushUIButtonToRender(m_buttons[CONTINUE_BUTTON]);
}

void MVC_Model_Mario::levelKeyUpdate(double dt)
{
	Vector2 direction;

	if (m_bKeyPressed[INTERACT_NEXT_KEY])
	{
		m_currentLevel = Math::Wrap(++m_currentLevel, 0, static_cast<int>(m_levelPaths.size() - 1));
		switchState(LOSE_STATE);
	}
	else if (m_bKeyPressed[INTERACT_PREV_KEY])
	{
		m_currentLevel = Math::Wrap(--m_currentLevel, 0, static_cast<int>(m_levelPaths.size() - 1));
		switchState(LOSE_STATE);
	}

	if (m_bKeyPressed[MOVE_FORWARD_KEY])
	{
		if (m_levelType == CharObject::TOP_DOWN_PLAYER)
		{
			direction.y = 1.0f;
		}
		else if (m_levelType == CharObject::SIDE_SCROLL_PLAYER)
		{
			m_hero->Jump();
		}
	}
	if (m_bKeyPressed[MOVE_BACKWARD_KEY])
	{
		if (m_levelType == CharObject::TOP_DOWN_PLAYER)
		{
			direction.y = -1.0f;
		}
	}
	if (m_bKeyPressed[MOVE_LEFT_KEY])
	{
		direction.x = -1.0f;
	}
	if (m_bKeyPressed[MOVE_RIGHT_KEY])
	{
		direction.x = 1.0f;
	}
	if (m_bKeyPressed[MOVE_JUMP_KEY])
	{
		heroAttack();
	}
	if (m_bKeyPressed[INTERACT_RELOAD_KEY])
	{
		m_hero->Reload();
	}

	// Weapon Switching
	if (m_bKeyPressed[MISC_1_KEY])
	{
		changeHeroWeaponType(Elemental::NORMAL_TYPE);
	}
	else if (m_bKeyPressed[MISC_2_KEY])
	{
		changeHeroWeaponType(Elemental::LIGHTNING_TYPE);
	}
	else if (m_bKeyPressed[MISC_3_KEY])
	{
		changeHeroWeaponType(Elemental::WATER_TYPE);
	}
	else if (m_bKeyPressed[MISC_4_KEY])
	{
		changeHeroWeaponType(Elemental::EARTH_TYPE);
	}

	m_hero->Move(direction, dt, m_map);
}

void MVC_Model_Mario::winKeyUpdate(void)
{
	m_buttons[CONTINUE_BUTTON]->button.UpdateState(m_bKeyPressed[INTERACT_ATTACK_1_KEY], m_mousePosX, m_mousePosY);
}

void MVC_Model_Mario::loseKeyUpdate(void)
{
	m_buttons[CONTINUE_BUTTON]->button.UpdateState(m_bKeyPressed[INTERACT_ATTACK_1_KEY], m_mousePosX, m_mousePosY);
}

void MVC_Model_Mario::levelController(void)
{
	Vector3 playerPos = m_hero->GetTransform().Translation;
	playerPos.x = static_cast<int>((playerPos.x + m_map->mapOffset.x) / m_map->map->GetTileSize());
	playerPos.y = static_cast<int>(m_map->map->GetNumOfTiles_Height() - static_cast<int>(ceil((playerPos.y + m_map->mapOffset.y) / m_map->map->GetTileSize())));


	// Check for Wining Condition
	if (countTargets() == 0 && m_map->map->GetMapAt(playerPos.x, playerPos.y))
	{
		// Win
		m_currentLevel = Math::Wrap(++m_currentLevel, 0, static_cast<int>(m_levelPaths.size() - 1));			// Go to next level
		switchState(WIN_STATE);		// Switch to the win state
	}

	// Spawn Enemies

	// Health/Lost Manager
	if (m_hero->IsActive() == false)
	{
		// Lose
		switchState(LOSE_STATE);
	}
}

void MVC_Model_Mario::enemyUpdate(double dt)
{
	// Spawn more if not enough
	if (countEnemies() < m_levelMaxEnemies)
	{
		spawnEnemy();
	}

	// Enemy Update
	for (vector<Ghost*>::iterator ghostBox = m_ghosts.begin(); ghostBox != m_ghosts.end(); ++ghostBox)
	{
		GhostEnemy* ghost = (*ghostBox)->ghost;
		GameObject* ghostRep = (*ghostBox)->ghostRep;
		GameObject* hpBar = (*ghostBox)->hpBar;

		if (ghost->IsActive())
		{
			// Update the Ghost
			ghost->SetTarget(m_hero->GetTransform().Translation.ToVector2());
			ghost->Update(dt);

			// Check for collision with bullets
			for (vector<BulletObject*>::iterator bullet = m_bulletList.begin(); bullet != m_bulletList.end(); ++bullet)
			{
				if (ghost->CheckCollision(*bullet, dt))
				{
					ElementalBullet* eBullet = dynamic_cast<ElementalBullet*>(*bullet);

					if (eBullet == NULL || ghost->IsNeutralTo(eBullet->GetElement()))	// If the ghost's element is not it's own 
					{
						ghost->AddToHealth(-(*bullet)->GetDamage());
					}
					else if (ghost->IsVulnerableTo(eBullet->GetElement()))					// If the ghost's weak element
					{
						ghost->AddToHealth(-(*bullet)->GetDamage() * VULNERABLE_DMG_MULTIPLIER);
					}

					// Delete the bullet
					(*bullet)->SetActive(false);
				}
			}

			// If the ghost attacks
			if (ghost->HadAttacked())
			{
				// Create a bullet
				ElementalBullet* bullet = FetchElementalBullet();

				// Calculate Bullet Direction
				Vector3 bulletDir = m_hero->GetTransform().Translation - ghost->GetTransform().Translation;
				if (bulletDir != Vector3::ZERO_VECTOR)
				{
					bulletDir.Normalize();
				}

				// Calculate Bullet Offset
				Vector3 bulletPos = ghost->GetTransform().Translation + bulletDir * m_map->map->GetTileSize();

				// Initialize the Bullet
				bullet->Init(bulletPos, bulletDir * ghost->GetWeaponBulletSpeed(), ghost->GetWeaponBulletScale(), BULLET_LIFETIME, ghost->GetWeaponElement());
				bullet->SetMesh(m_bulletMesh[ghost->GetWeaponElement()]);
				bullet->SetDamage(ghost->GetWeaponDamage());
			}

			// Check for collision with player
			if (ghost->CheckCollision(m_hero, dt))
			{
				// Kill ghost
				ghost->Kill();

				// Injure player
				m_hero->AddToHealth(-(ghost->GetWeaponDamage() * CONTACT_DMG_MULTIPLIER));
			}

			// Update the GameObject
			Vector2 enemyPos(ghost->GetTransform().Translation.ToVector2());
			Vector3 pos(enemyPos.x, enemyPos.y);
			ghostRep->SetPos(pos);

			// Update the Healthbar
			// --Make the healthbar above the enemy
			Vector3 hpBarPos = ghost->GetTransform().Translation;
			hpBarPos.y += m_map->map->GetTileSize();

			hpBar->SetPos(hpBarPos);
			hpBar->SetScale(Vector3(ghost->GetHealth() / static_cast<float>(ghost->GetMaxHealth()), 1.0f, 1.0f));
		}
	}
}

void MVC_Model_Mario::targetUpdate(double dt)
{
	for (vector<Target*>::iterator targetBox = m_targets.begin(); targetBox != m_targets.end(); ++targetBox)
	{
		TargetEnemy* target = (*targetBox)->target;
		GameObject* targetRep = (*targetBox)->targetRep;
		GameObject* hpBar = (*targetBox)->hpBar;

		if (target->IsActive())
		{
			// Move the target according to mapOffset
			Vector3 targetPos = target->GetTransform().Translation;
			targetPos.x = target->GetOriginalPos().x - m_map->mapOffset.x;
			target->SetPos(targetPos);

			// Check for collision with bullets
			for (vector<BulletObject*>::iterator bullet = m_bulletList.begin(); bullet != m_bulletList.end(); ++bullet)
			{
				if (target->CheckCollision(*bullet, dt))
				{
					ElementalBullet* eBullet = dynamic_cast<ElementalBullet*>(*bullet);

					if (eBullet == NULL || target->IsNeutralTo(eBullet->GetElement()))		// If the ghost's element is not it's own
					{
						target->AddToHealth(-(*bullet)->GetDamage());
					}
					else if (target->IsVulnerableTo(eBullet->GetElement()))					// If the ghost's weak element
					{
						target->AddToHealth(-(*bullet)->GetDamage() * VULNERABLE_DMG_MULTIPLIER);
					}

					// Delete the bullet
					(*bullet)->SetActive(false);

					// If target isn't alive anymore
					if (target->IsActive() == false)
					{
						// Change the current position to a normal hole
						Vector2 targetPos = target->GetTilePosition(m_map);
					}
				}
			}

			// Update Target GO Position
			Vector2 targetGOPos(target->GetTransform().Translation.ToVector2());
			Vector3 pos(targetGOPos.x, targetGOPos.y);
			targetRep->SetPos(pos);

			// Update Target HP Bar
			// --Make the healthbar above the target
			Vector3 hpBarPos = target->GetTransform().Translation;
			hpBarPos.x -= m_map->map->GetTileSize() * 0.5;
			hpBarPos.y += m_map->map->GetTileSize() * 0.5;

			hpBar->SetPos(hpBarPos);
			hpBar->SetScale(Vector3(target->GetHealth() / static_cast<float>(target->GetMaxHealth()), 1.0f, 1.0f));
		}
	}
}

void MVC_Model_Mario::bulletUpdate(double dt)
{
	for (std::vector<BulletObject *>::iterator it = m_bulletList.begin(); it != m_bulletList.end(); ++it)
	{
		BulletObject *bullet = (BulletObject *)*it;
		if (bullet->IsActive())
		{
			bullet->Update(dt);

			// Check if bullet is in a wall
			Vector3 bulletPos = bullet->GetTransform().Translation;
			bulletPos.x = static_cast<int>((bulletPos.x + m_map->mapOffset.x) / m_map->map->GetTileSize());
			bulletPos.y = static_cast<int>(m_map->map->GetNumOfTiles_Height() - static_cast<int>(ceil((bulletPos.y + m_map->mapOffset.y) / m_map->map->GetTileSize())));

			// Only affect the player's bullets (PLAYER_BULLET_SCALE)
			if (bullet->GetTransform().Scale == PLAYER_BULLET_SCALE && m_map->map->TileAtPosIsWall(bulletPos.x, bulletPos.y))
			{
				// If it is, destroy it
     				bullet->SetActive(false);
				continue;
			}

			m_renderList2D.push_back(bullet);
		}
	}
}

void MVC_Model_Mario::updateHeroHealthBar(void)
{
	// Make the healthbar above the player
	Vector3 hpBarPos = m_hero->GetTransform().Translation;
	hpBarPos.y += m_map->map->GetTileSize();

	m_heroHPBar->SetPos(hpBarPos);
	m_heroHPBar->SetScale(Vector3(m_hero->GetHealth() / static_cast<float>(m_hero->GetMaxHealth()), 1.0f, 1.0f));
}

ElementalBullet * MVC_Model_Mario::FetchElementalBullet()
{
	// Look for an inactive bullet that can be used
	for (std::vector<BulletObject *>::iterator it = m_bulletList.begin(); it != m_bulletList.end(); ++it)
	{
		ElementalBullet *bullet = dynamic_cast<ElementalBullet*>(*it);

		if (bullet == NULL)
		{
			// Only accept elemental bullets
			continue;
		}

		if (!bullet->IsActive())
		{
			bullet->SetActive(true);
			return bullet;
		}
	}

	const unsigned BATCH_PRODUCE = 10;

	// Produce some so that we don't have to keep producing everytime we need one
	ElementalBullet *bullet;
	for (unsigned i = 0; i < BATCH_PRODUCE; ++i)
	{
		bullet = new ElementalBullet();
		m_bulletList.push_back(bullet);
	}

	// Return the last bullet to the caller
	bullet->SetActive(true);

	return bullet;
}

void MVC_Model_Mario::pushUIButtonToRender(Button* btn)
{
	m_renderList2D.push_back(btn->buttonRep);
}

void MVC_Model_Mario::pushEnemiesToRender(void)
{
	// Render the enemy
	for (vector<Ghost*>::iterator ghostBox = m_ghosts.begin(); ghostBox != m_ghosts.end(); ++ghostBox)
	{
		if ((*ghostBox)->ghost->IsActive())
		{
			m_renderList2D.push_back((*ghostBox)->ghostRep);
			m_renderList2D.push_back((*ghostBox)->hpBar);
		}
	}
}

void MVC_Model_Mario::pushTargetsToRender(void)
{
	for (vector<Target*>::iterator targetBox = m_targets.begin(); targetBox != m_targets.end(); ++targetBox)
	{
		TargetEnemy* target = (*targetBox)->target;
		GameObject* targetRep = (*targetBox)->targetRep;
		GameObject* hpBar = (*targetBox)->hpBar;

		if (target->IsActive())
		{
			m_renderList2D.push_back(targetRep);

			if (target->GetHealth() < target->GetMaxHealth())
			{
				m_renderList2D.push_back(hpBar);
			}
		}
	}
}

void MVC_Model_Mario::processKeyAction(double dt)
{
	switch (m_gameState)
	{
		case LEVEL_STATE:
			levelKeyUpdate(dt);
			break;
		case WIN_STATE:
			winKeyUpdate();
			break;
		case LOSE_STATE:
			loseKeyUpdate();
			break;
	}
}

void MVC_Model_Mario::heroAttack(void)
{
	if (m_hero->Attack())
	{
		// Create a bullet
		ElementalBullet* bullet = FetchElementalBullet();
		Vector2 heroMidPos = m_hero->GetTransform().Translation.ToVector2() + m_map->map->GetTileSize() * 0.5;

		// Calculate Bullet Offset
		Vector3 bulletPos = heroMidPos + m_hero->GetLookDir() * m_map->map->GetTileSize();

		// Initialize the Bullet
		bullet->Init(bulletPos, m_hero->GetLookDir() * m_hero->GetWeaponBulletSpeed(), m_hero->GetWeaponBulletScale(), BULLET_LIFETIME, m_hero->GetWeaponElement());
		bullet->SetMesh(m_bulletMesh[m_hero->GetWeaponElement()]);
		bullet->SetDamage(m_hero->GetWeaponDamage());
	}
}

void MVC_Model_Mario::changeHeroWeaponType(Elemental::ELEMENT_TYPE element)
{
	if (element >= Elemental::NUM_ELEMENTS)
	{
		return;
	}

	// Change the weapon
	m_hero->SetWeapon(m_heroWeapon[element]);

	// Set the hero mesh accordinginly
	m_heroGO->SetMesh(m_heroMesh[m_levelType][element]);
}

int MVC_Model_Mario::countTargets(void)
{
	int count = 0;

	for (vector<Target*>::iterator targetBox = m_targets.begin(); targetBox != m_targets.end(); ++targetBox)
	{
		TargetEnemy* target = (*targetBox)->target;

		if (target->IsActive())
		{
			++count;
		}
	}

	return count;
}

void MVC_Model_Mario::switchState(GAME_STATE state)
{
	clearLevel();

	switch (state)
	{
		case LEVEL_STATE:
			levelInit();
			break;
	}

	m_gameState = state;
}

void MVC_Model_Mario::spawnEnemy(void)
{
	// Get an enemy to spawn
	Ghost* ghostBox = fetchEnemy();

	// How far out off the screen to spawn
	const int OFFSET = m_map->map->GetTileSize() * 3;
	// Set a spawn position
	Vector3 spawnPos;
	
	// Determine which edge to spawn from
	// -- 0: Left, 1: Top, 2: Right, 3: Bottom
	int edge = Math::RandIntMinMax(0, 3);
	switch (edge)
	{
		case 0:
			spawnPos.x = -OFFSET;
			break;
		case 1:
			spawnPos.y = m_viewHeight + OFFSET;
			break;
		case 2:
			spawnPos.x = m_viewWidth + OFFSET;
			break;
		case 3:
			spawnPos.y = -OFFSET;
			break;
	}

	if (edge == 0 || edge == 2)
	{
		spawnPos.y = Math::RandFloatMinMax(0.0f, m_viewHeight);
	}
	else
	{
		spawnPos.x = Math::RandFloatMinMax(0.0f, m_viewWidth);
	}

	// Decide the enemy's element
	Elemental::ELEMENT_TYPE type = static_cast<Elemental::ELEMENT_TYPE>(Math::RandIntMinMax(Elemental::NORMAL_TYPE, Elemental::EARTH_TYPE));

	// Init Enemy
	ghostBox->ghost->Init(m_levelType, NULL, spawnPos, Vector3(32.0f, 32.0f), GHOST_MOVE_SPEED, GHOST_HEALTH, type);

	// Init Enemy GO
	ghostBox->ghostRep->SetMesh(m_enemyMesh[type]);
}

Ghost* MVC_Model_Mario::fetchEnemy(void)
{
	// Look for an Inactive Enemy to Use
	for (vector<Ghost*>::iterator ghostBox = m_ghosts.begin(); ghostBox != m_ghosts.end(); ++ghostBox)
	{
		GhostEnemy* ghost = (*ghostBox)->ghost;

		if (ghost->IsActive() == false)
		{
			// Activate this Ghost
			ghost->SetHealth(GHOST_HEALTH);

			return *ghostBox;
		}
	}

	// Generate 10 ghosts if none is available
	const int BATCH_PRODUCE = 10;
	for (int i = 0; i < BATCH_PRODUCE; ++i)
	{
		// -- Create Enemy
		GhostEnemy* ghost = new GhostEnemy;

		// -- Create Enemy GO
		GameObject* ghostRep = new GameObject;

		// -- Create Enemy HP Bar
		GameObject* hpBar = new GameObject(m_hpBarMesh);

		// --Push this in to a Ghost Struct
		m_ghosts.push_back(new Ghost(ghost, ghostRep, hpBar));
	}
	
	return m_ghosts.back();
}

int MVC_Model_Mario::countEnemies(void)
{
	int count = 0;

	for (vector<Ghost*>::iterator ghostBox = m_ghosts.begin(); ghostBox != m_ghosts.end(); ++ghostBox)
	{
		GhostEnemy* ghost = (*ghostBox)->ghost;

		if (ghost->IsActive())
		{
			++count;
		}
	}

	return count;
}

void MVC_Model_Mario::spawnTarget(Vector2 pos, float scale, int health, int rewardScore, Elemental::ELEMENT_TYPE element)
{
	// Get a Target to Spawn
	Target* targetBox = fetchTarget();

	// Init Target
	TargetEnemy* target = targetBox->target;
	float tilesizeHalf = m_map->map->GetTileSize() * 0.5;
	Vector2 posOffset(tilesizeHalf, -tilesizeHalf);
	target->Init(m_levelType, m_targetMesh[Elemental::NORMAL_TYPE], pos + posOffset, Vector3(scale, scale, scale), 0.0f, health, rewardScore, element);

	// Init TargetRep
	targetBox->targetRep->SetMesh(m_targetMesh[element]);
}

Target* MVC_Model_Mario::fetchTarget(void)
{
	// Look for an Inactive Target to Use
	for (vector<Target*>::iterator targetBox = m_targets.begin(); targetBox != m_targets.end(); ++targetBox)
	{
		TargetEnemy* target = (*targetBox)->target;

		if (target->IsActive() == false)
		{
			// Activate this Target
			target->SetHealth(GHOST_HEALTH);

			return *targetBox;
		}
	}

	// Generate 10 targets if none is available
	const int BATCH_PRODUCE = 10;
	for (int i = 0; i < BATCH_PRODUCE; ++i)
	{
		// -- Create Target
		TargetEnemy* target = new TargetEnemy;

		// -- Create Target GO
		GameObject* targetRep = new GameObject;

		// -- Create Target HP Bar
		GameObject* hpBar = new GameObject(m_hpBarMesh);

		// --Push this in to a Ghost Struct
		m_targets.push_back(new Target(target, targetRep, hpBar));
	}

	return m_targets.back();
}

