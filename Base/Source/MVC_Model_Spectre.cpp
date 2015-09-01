#include "MVC_Model_Spectre.h"

#include "SoundEngine.h"

const Vector2 MVC_Model_Spectre::S_M_MESSAGE_OFFSET(20.0f, 20.0f);
const double MVC_Model_Spectre::S_M_LIGHTING_UPDATE_FREQUENCY = 0.5;
const float MVC_Model_Spectre::S_M_BGM_VOLUME = 0.5f;
const float MVC_Model_Spectre::S_M_MAX_ALERT = 5.f;
const float MVC_Model_Spectre::S_M_MENU_KEYS_INPUT_DELAY = 0.2f;
const int MVC_Model_Spectre::S_M_PLAYER_MAX_HP = 1;

MVC_Model_Spectre::MVC_Model_Spectre(string configSONFile) : MVC_Model(configSONFile)
	, m_appState(AS_MENU)
	, m_currentLevelID(0)
	, m__currentLevel(NULL)
	, m__player(NULL)
	, m_enableShadow(true)
	, m_alertLevel(0.f)
	, m__alert(NULL)
	, m__spectreDive(NULL)
	, m__spectreHost(NULL)
	, m__spectreJump(NULL)
	, m__fKey(NULL)
	, m__kKey(NULL)
	, m__defuseBomb(NULL)
	, m__plantBomb(NULL)
	, m_objective(NULL)
	, m__bgm(NULL)
	, m__menu(NULL)
	, m_menuKeysInputTimer(0.f)
	, m_pauseTimer(0.f)
	, m_shadowMode(false)
	, m_lightUpdateTimer(S_M_LIGHTING_UPDATE_FREQUENCY)
	, m__objectivesHUD(NULL)
{
}


MVC_Model_Spectre::~MVC_Model_Spectre(void)
{
}

void MVC_Model_Spectre::processKeyAction(double dt)
{
	switch (m_appState)
	{
		case AS_MAIN_GAME:
		{
			if (m_bKeyPressed[MOVE_FORWARD_KEY])
			{
				m__player->SetMove(Direction::DIRECTIONS[Direction::DIR_UP]);
			}
			if (m_bKeyPressed[MOVE_BACKWARD_KEY])
			{
				m__player->SetMove(Direction::DIRECTIONS[Direction::DIR_DOWN]);
			}
			if (m_bKeyPressed[MOVE_LEFT_KEY])
			{
				m__player->SetMove(Direction::DIRECTIONS[Direction::DIR_LEFT]);
			}
			if (m_bKeyPressed[MOVE_RIGHT_KEY])
			{
				m__player->SetMove(Direction::DIRECTIONS[Direction::DIR_RIGHT]);
			}

			if (m_bKeyPressed[INTERACT_GENERIC_KEY] && m_shadowMode == false)
			{
				//This is to go to the next level
				if (m__player->Interact(Player::INTERACT_ESCAPE, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_ESCAPE
					&& m__currentLevel->GetObjectiveComplete() == true)
				{
					// Stage completed
					//nextLevel();
					m_appState = AS_MENU;
					m__menu->AssignCurrent(Menu::MENU_WIN_LEVEL);
					
				}

				//This is to trigger the activate objective
				if ((m__player->Interact(Player::INTERACT_COLLECT, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_COLLECT)
					|| (m__player->Interact(Player::INTERACT_DEFUSE, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_DEFUSE)
					|| (m__player->Interact(Player::INTERACT_SETBOMB, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_SETBOMB)
					|| (m__player->Interact(Player::INTERACT_HOSTAGE, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_HOSTAGE))
				{
					if (m__currentLevel->GetObjectiveComplete() == false && m__currentLevel->GetActiveObjective() == false)
					{
						m__currentLevel->ActivateObjective();
						
						if (m__player->Interact(Player::INTERACT_DEFUSE, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_DEFUSE)
						{
							m__soundPlayer[SP_OBJ_BOMB_DEFUSING]->Play(true);
						}
						else if (m__player->Interact(Player::INTERACT_SETBOMB, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_SETBOMB)
						{
							m__soundPlayer[SP_OBJ_BOMB_PLANTING]->Play(true);
						}
					}
				}

				
				
				if (m__currentLevel->GetObjectiveComplete() == true)
				{
					if ((m__player->Interact(Player::INTERACT_COLLECT, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_COLLECT)
						|| (m__player->Interact(Player::INTERACT_DEFUSE, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_DEFUSE)
						|| (m__player->Interact(Player::INTERACT_SETBOMB, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_SETBOMB))
					{
						// Force set tilemap
						Tile* _tileOnPlayer = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos());
						Tile* _tileInFrontPlayer = m__currentLevel->GetTileMap()->GetTileAt
							(m__player->GetMapPos() + m__player->GetLookDir() * m__currentLevel->GetTileMap()->GetTileSize());
						if (_tileOnPlayer->GetType() == Tile::TILE_BOMB || _tileOnPlayer->GetType() == Tile::TILE_DOCUMENT)
						{
							_tileOnPlayer->SetMesh(GetMeshResource("TILE_FLOOR"));
							_tileOnPlayer->SetType(Tile::TILE_FLOOR);
							m__soundPlayer[SP_OBJ_BOMB_DEFUSING]->Pause();
						}
						else if ((_tileInFrontPlayer->GetType() == Tile::TILE_BOMB || _tileInFrontPlayer->GetType() == Tile::TILE_DOCUMENT))
						{
							_tileInFrontPlayer->SetMesh(GetMeshResource("TILE_FLOOR"));
							_tileInFrontPlayer->SetType(Tile::TILE_FLOOR);
							m__soundPlayer[SP_OBJ_BOMB_DEFUSING]->Pause();
						}
						else if (_tileOnPlayer->GetType() == Tile::TILE_SETBOMBAREA)
						{
							_tileOnPlayer->SetMesh(GetMeshResource("TILE_BOMB"));
							_tileOnPlayer->SetType(Tile::TILE_BOMB);
							m__soundPlayer[SP_OBJ_BOMB_PLANTING]->Pause();
						}
						else if (_tileInFrontPlayer->GetType() == Tile::TILE_SETBOMBAREA)
						{
							_tileInFrontPlayer->SetMesh(GetMeshResource("TILE_BOMB"));
							_tileInFrontPlayer->SetType(Tile::TILE_BOMB);
							m__soundPlayer[SP_OBJ_BOMB_PLANTING]->Pause();
						}

						m__soundPlayer[SP_OBJ_COMPLETE]->Play(false);
					}
				}
			}
			else
			{
				//when key release - not setting or defusing bomb
				//GetActiveObjective - means objective is updating
				if (m__currentLevel->GetActiveObjective() && !m__currentLevel->GetObjectiveComplete())
				{
					m__currentLevel->ResetObjective();
					m__soundPlayer[SP_OBJ_BOMB_DEFUSING]->Pause();
					m__soundPlayer[SP_OBJ_BOMB_PLANTING]->Pause();
				}
			}	

			if (m_bKeyPressed[INTERACT_SKILL_2_KEY] && m_shadowMode == false) // Spectral Hax
			{
				if (m__player->Interact(Player::INTERACT_HAX, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_HAX)
				{
					m__player->SetState(Player::PS_SPECTRAL_HAX);
					startHackMode();
				}
			}

			// Spectral Dive
			if (m_bKeyPressed[INTERACT_SKILL_1_KEY] && m__player->Interact(Player::INTERACT_DIVE, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_DIVE) // Spectral Dive
			{
				if (m__player->GetState() == Player::PS_SPECTRAL_DIVE_UP 
					|| 
					m__player->GetState() == Player::PS_SPECTRAL_DIVE_DOWN 
					|| 
					m__player->GetState() == Player::PS_SPECTRAL_DIVE_LEFT 
					|| 
					m__player->GetState() == Player::PS_SPECTRAL_DIVE_RIGHT)
				{
					// If this is pressed while player is diving, means he is exiting
					m__soundPlayer[SP_SKILL_DIVE_EXIT]->Play(false);

					// If was hosting, pressing again means player is exiting
					if (m__player->GetHosting())
					{
						m__soundPlayer[SP_SKILL_SPECTRAL_HOST_EXIT]->Play(false);
					}
					m_shadowMode = false;
				}
				else
				{
					m__soundPlayer[SP_SKILL_DIVE_ENTER]->Play(false);
					m_shadowMode = true;
				}

				m__player->SetDive();

				//if player currently not hosting
				if (m__player->GetHosting() == false)
				{
					for (vector<NPC*>::iterator enemyIter = m_enemyList.begin(); enemyIter != m_enemyList.end(); ++enemyIter)
					{
						NPC* _enemy = *enemyIter;
						//if player dives within a tile distance from player
						if ((m__player->GetMapPos() - _enemy->GetMapPos()).LengthSquared() <= m__currentLevel->GetTileMap()->GetTileSize() *  m__currentLevel->GetTileMap()->GetTileSize())
						{
							m__player->SetHostPTR(_enemy);
							m__soundPlayer[SP_SKILL_SPECTRAL_HOST_ENTER]->Play(false);
							m_shadowMode = true;
							break;
						}
					}
				}
			}

			//if  objective hsve anyhting to update
			if(!m__currentLevel->GetObjectiveComplete() )
			{
				m__currentLevel->UpdateObjective(dt);
			}

			if (m_bKeyPressed[MOVE_JUMP_KEY] && m__player->Interact(Player::INTERACT_JUMP, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_JUMP) // Spectral Jump
			{
				m__player->SetJump();
				m__soundPlayer[SP_SKILL_SPECTRAL_JUMP]->Play(false);
			}
			
			// Pause game
			if (m_bKeyPressed[GAME_EXIT_KEY] && m_pauseTimer <= 0.f)
			{
				m_appState = AS_MENU;
				m__menu->AssignCurrent(Menu::MENU_PAUSE);
				m_pauseTimer = S_M_MENU_KEYS_INPUT_DELAY;
			}
			break;
		}

		case AS_HACK_GAME:
		{
			m_hackingGame.Move(m_bKeyPressed[MOVE_LEFT_KEY], m_bKeyPressed[MOVE_RIGHT_KEY], m_bKeyPressed[MOVE_FORWARD_KEY], m_bKeyPressed[MOVE_BACKWARD_KEY], dt);
			break;
		}
		
		// Menu
		case AS_MENU:
		{
			if (m_bKeyPressed[MOVE_BACKWARD_KEY] && m_menuKeysInputTimer <= 0.f)
			{
				m__menu->KeysUpdate(dt, false);
				m_menuKeysInputTimer = S_M_MENU_KEYS_INPUT_DELAY;
			}
			if (m_bKeyPressed[MOVE_FORWARD_KEY] && m_menuKeysInputTimer <= 0.f)
			{
				m__menu->KeysUpdate(dt, true);
				m_menuKeysInputTimer = S_M_MENU_KEYS_INPUT_DELAY;
			}
			if (m_bKeyPressed[INTERACT_ATTACK_1_KEY] && m_menuKeysInputTimer <= 0.f) // Left mouse click
			{
				processMenuKeyAction(m__menu->OnClick(m_mousePosX, m_viewHeight - m_mousePosY));
			}
			if (m_bKeyPressed[GAME_ACCEPT_KEY] && m_menuKeysInputTimer <= 0.f) // Enter press
			{
				processMenuKeyAction(m__menu->OnEnter());
			}
			break;
		}
	}

	// Quitting the game
	/*if (m_bKeyPressed[GAME_EXIT_KEY])
	{
		// TODO: Open a pause menu and then quit by that instead. Do actual pausing or return to menus
		m_running = false;
	}*/
}

void MVC_Model_Spectre::processMenuKeyAction(MenuManager::E_RETURN_STATE returnState)
{
	switch (returnState)
	{
		case MenuManager::RS_MENU:
		{
			m_appState = AS_MENU;

			if (m__menu->GetCurrentMenu()->GetType() == Menu::MENU_MAIN)
			{
				// If there is bgm playing, stop it
				if (m__bgm != NULL)
				{
					m__bgm->Stop();
					m__bgm = NULL;
				}

				// Play menu music
				m__soundPlayer[SP_MAIN_MENU]->Play(true);
			}
		}
		break;
		case MenuManager::RS_NEW_GAME:
			{
				m_appState = AS_MAIN_GAME;
				m_currentLevelID = 0;
				loadLevel(m_levelFiles[m_currentLevelID]);
			}
			break;
		case MenuManager::RS_EXIT:
			{
				m_running = false;
			}
			break;
	case MenuManager::RS_CURRENT_LEVEL:
			{
				m_appState = AS_MAIN_GAME;
				loadLevel(m_levelFiles[m_currentLevelID]);
			}
			break;
		case MenuManager::RS_NEXT_LEVEL:
			{
				m_appState = AS_MAIN_GAME;
				nextLevel();
			}
			break;
		case MenuManager::RS_RESUME:
			{
				m_appState = AS_MAIN_GAME;
			}
			break;
		case MenuManager::RS_LEVEL:
			{
				m_appState = AS_MAIN_GAME;
				m_currentLevelID = m__menu->GetCurrentButton()->GetLevelID();
				loadLevel(m_levelFiles[m_currentLevelID]);
			}
			break;
	}
	m_menuKeysInputTimer = S_M_MENU_KEYS_INPUT_DELAY;
}

int MVC_Model_Spectre::findLevelFiles(string folderPath)
{
	m_levelFiles.clear();

	short numOfFiles = 0;
	ifstream files;

	//Gets all file names
	std::ostringstream command;
	command <<"dir \"" << folderPath << "\" /b /a-d > level_names";
	system(command.str().c_str());

	files.open("level_names");

	if (files.is_open())
	{
		string output;

		while (!files.eof())
		{
			getline(files, output);
			if (!(output == ""))
			{
				// Append the Folder
				output = folderPath + output;
				m_levelFiles.push_back(output);
				++numOfFiles;
			}
		}

		files.close();

		remove("level_names");
	}
	else
	{
		return -1;
	}

	return numOfFiles;
}

void MVC_Model_Spectre::loadLevel(string levelMapFile)
{
	m_alertLevel = 0.f;
	Vector2 numScreenTile;
	// Delete the previous level
	if (m__currentLevel != NULL)
	{
		numScreenTile = m__currentLevel->GetTileMap()->GetNumScreenTile();
		m__currentLevel->Clear();
		delete m__currentLevel;
		m__currentLevel = NULL;
	}
	clearCameraList();

	// Initialize the level
	m__currentLevel = new Level();
	//m__currentLevel->InitMap(Vector2(64, 50), m_viewWidth, m_viewHeight, 64, "TileMap//Level1.csv", meshList);
	m__currentLevel->Load(levelMapFile, m_viewWidth, m_viewHeight, meshList, numScreenTile);
	SecurityCamera::InitCamMeshList(meshList);
	clearCameraList();
	loadToList(m__currentLevel->GetTileMap());

	// Ensure that lighting is updated in the first frame of the level
	m_lightUpdateTimer = S_M_LIGHTING_UPDATE_FREQUENCY;
	
	// Setup camera view
	for (vector<SecurityCamera*>::iterator it = m_cameraList.begin(); it != m_cameraList.end(); ++it)
	{
		SecurityCamera* _camera = *it;
		if (_camera)
		{
			_camera->GenerateViewBox(m__currentLevel->GetTileMap());
		}
	}

	// Initialize the player
	int tileSize = m__currentLevel->GetTileMap()->GetTileSize();
	Vector2 playerSpawnPos = m__currentLevel->GetTileMap()->GetPlayerSpawnPos();
	Vector2 spawnScrollOffset = playerSpawnPos - m__currentLevel->GetTileMap()->GetScreenSize() * 0.5f;
	m__currentLevel->GetTileMap()->SetScrollOffset(spawnScrollOffset);
	m__player->SetHealth(1);
	m__player->SetMapPosition(playerSpawnPos, spawnScrollOffset, m__currentLevel->GetTileMap()->GetTileSize()); // Start at center with no scroll offset
	m__player->SetScale(Vector3(tileSize, tileSize));
	m__player->SetInShadow(false);
	m__player->ForceSetAnimation(Player::PS_IDLE_UP);
	m_shadowMode = false;
	

	// Initialize the enemies
	clearEnemyList();
	
	vector<NPC*> enemies = m__currentLevel->GetEnemyList();
	for (vector<NPC*>::iterator enemyIT = enemies.begin(); enemyIT != enemies.end(); ++enemyIT)
	{
		NPC* _enemy = NPC::CreateCopy(**enemyIT);

		Vector2 mapPos = m__player->GetMapTilePos();
		_enemy->SetPlayerPtr(m__player);
		_enemy->SetActive(true);
		m_enemyList.push_back(_enemy);
	}

	// Get a reference to the objective
	m_objective = m__currentLevel->GetObjective();

	// Init the HUD
	initHUD();

	// Initialize the messages
	// -- Clear any previous messages from any previous levels
	m_messenger.ClearMessages();
	// -- Load the messages for this map
	m_messenger.AddMessages(m__currentLevel->GetMessagesFile());

	// Initialize the BGM
	// -- If main menu bgm is playing, stop it
	m__soundPlayer[SP_MAIN_MENU]->Pause();
	// -- Find the BGM
	static int prevBGM = -1;
	int bgm = GetSoundResource(m__currentLevel->GetBGMName());
	
	// -- if the previous BGM is not the same, then init the new sound
	if (bgm != prevBGM || m__bgm == NULL)
	{
		// -- Stop previous BGM
		if (m__bgm != NULL)
		{
			m__bgm->Stop();
			m__bgm = NULL;
		}

		// If the sound was found
		if (bgm >= 0)
		{
			m__bgm = SoundEngine::CreateSound2D(bgm);
			m__bgm->Play(true);
			m__bgm->SetVolume(S_M_BGM_VOLUME);
			prevBGM = bgm;
		}
	}
}

void MVC_Model_Spectre::nextLevel(void)
{
	m_currentLevelID = Math::Wrap(++m_currentLevelID, 0, static_cast<int>(m_levelFiles.size() - 1));

	loadLevel(m_levelFiles[m_currentLevelID]);
}

void MVC_Model_Spectre::prevLevel(void)
{
	m_currentLevelID = Math::Wrap(--m_currentLevelID, 0, static_cast<int>(m_levelFiles.size() - 1));

	loadLevel(m_levelFiles[m_currentLevelID]);
}

void MVC_Model_Spectre::pushMessageToRender(void)
{
	// Store the message objects into this
	m_messageObjects = m_messenger.GetMessageObjects(m_viewWidth, m_viewHeight);

	for (vector<GameObject2D*>::iterator messageObjects = m_messageObjects.begin(); messageObjects != m_messageObjects.end(); ++messageObjects)
	{
		m_renderList2D.push(*messageObjects);
	}
}

void MVC_Model_Spectre::resetTileMarkers(void)
{
	for (vector<GameObject2D*>::iterator tileMarker = m__tileMarkers.begin(); tileMarker != m__tileMarkers.end(); ++tileMarker)
	{
		(*tileMarker)->SetActive(false);
	}
}

GameObject2D * MVC_Model_Spectre::fetchTileMarker(TILE_MARKER_TYPE type)
{
	// Retrieve a light
	for (vector<GameObject2D*>::iterator tileMarker = m__tileMarkers.begin(); tileMarker != m__tileMarkers.end(); ++tileMarker)
	{
		if (!(*tileMarker)->GetActive())
		{
			(*tileMarker)->SetActive(true);
			(*tileMarker)->SetMesh(m__tileMarkerMesh[type]);
			return *tileMarker;
		}
	}

	// Generate some if unavailable
	const int BATCH_MAKE = 10;
	for (size_t tileMarker = 0; tileMarker < BATCH_MAKE; ++tileMarker)
	{
		PhysicalObject* _tileMarker = new PhysicalObject();
		_tileMarker->SetActive(false);
		m__tileMarkers.push_back(_tileMarker);
	}

	m__tileMarkers.back()->SetActive(true);
	m__tileMarkers.back()->SetMesh(m__tileMarkerMesh[type]);

	return m__tileMarkers.back();
}

void MVC_Model_Spectre::startHackMode(void)
{
	m_appState = AS_HACK_GAME;
	m_hackingGame.Reset(m_viewWidth, m_viewHeight);
	m__soundPlayer[SP_SKILL_HACK_START]->Play(false);
}

void MVC_Model_Spectre::Init(void)
{
	MVC_Model::Init();

	// Init Sounds
	m__soundPlayer[SP_MAIN_MENU] = SoundEngine::CreateSound2D(GetSoundResource("MenuBGM"));
	m__soundPlayer[SP_SKILL_DIVE_ENTER] = SoundEngine::CreateSound2D(GetSoundResource("EnterDive"));
	m__soundPlayer[SP_SKILL_DIVE_EXIT] = SoundEngine::CreateSound2D(GetSoundResource("ExitDive"));
	m__soundPlayer[SP_SKILL_HACK_START] = SoundEngine::CreateSound2D(GetSoundResource("StartHack"));
	m__soundPlayer[SP_SKILL_HACK_STOP] = SoundEngine::CreateSound2D(GetSoundResource("StopHack"));
	m__soundPlayer[SP_SKILL_SPECTRAL_JUMP] = SoundEngine::CreateSound2D(GetSoundResource("SpectralJump"));
	m__soundPlayer[SP_SKILL_SPECTRAL_HOST_ENTER] = SoundEngine::CreateSound2D(GetSoundResource("PossessEnemy"));
	m__soundPlayer[SP_SKILL_SPECTRAL_HOST_EXIT] = SoundEngine::CreateSound2D(GetSoundResource("UnpossessEnemy"));
	m__soundPlayer[SP_OBJ_COMPLETE] = SoundEngine::CreateSound2D(GetSoundResource("Objective_Complete"));
	m__soundPlayer[SP_OBJ_BOMB_PLANTING] = SoundEngine::CreateSound2D(GetSoundResource("Objective_BombPlant"));
	m__soundPlayer[SP_OBJ_BOMB_DEFUSING] = SoundEngine::CreateSound2D(GetSoundResource("Objective_BombDefuse"));
	m__soundPlayer[SP_OBJ_ASSASSINATE] = SoundEngine::CreateSound2D(GetSoundResource("Objective_Assassinate"));
	m__soundPlayer[SP_DEATH] = SoundEngine::CreateSound2D(GetSoundResource("Death"));
	m__soundPlayer[SP_EXPLOSIVE_DEATH] = SoundEngine::CreateSound2D(GetSoundResource("Explosion"));

	// -- Load Shadow GameObject
	m__tileMarkerMesh[TM_SHADOW] = GetMeshResource("ShadowOverlay");
	m__tileMarkerMesh[TM_VIEWED] = GetMeshResource("LightOverlay");

	// Load the player
	initPlayer();

	// Init the hacking game
	m_hackingGame.Init
		(
			GetMeshResource("ShadowBall"), 
			GetMeshResource("PlayerBall"), 
			GetMeshResource("CircuitWall"), 
			GetMeshResource("DestroyedWall"), 
			GetMeshResource("RestrictedWall"), 
			GetMeshResource("LoseScreen"), 
			GetMeshResource("MinigameBG"),
			m_viewWidth, m_viewHeight);

	// Search for and store all the level files
	findLevelFiles("Levels//");

	// Load menu
	initMenu();
	// -- Start menu music
	m__soundPlayer[SP_MAIN_MENU]->Play(true);

	// Init the MessageManager
	m_messenger.Init(GetMeshResource("MessageBG"), m_defaultFont, m_defaultFont, Vector2(m_viewWidth - S_M_MESSAGE_OFFSET.x, 200.0f), S_M_MESSAGE_OFFSET);
	m_messenger.AddMessages("Messages//Level1_Message.son");
}

void MVC_Model_Spectre::initMenu(void)
{
	UIButton::InitMeshLists(meshList); // Generate static mesh list for buttons
	int buttonCount; // Use this for alignment of buttons
	Mesh* genericBG = GetMeshResource("GenericMenuBG");
	const float HEIGHT_OFFSET = m_viewHeight * 0.1f;
	const Vector2 BUTTON_SIZE(m_viewWidth * 0.25f, m_viewHeight * 0.08f);
	Vector2 startPos; // Start position
	
	if (m__menu == NULL)
	{
		m__menu = new MenuManager_Spectre();
	}
	/* Main menu */
	// Variables
	startPos = Vector2((m_viewWidth * 0.75f) - (BUTTON_SIZE.x * 0.5f), (m_viewHeight * 0.5f) - (BUTTON_SIZE.y * 0.5f));
	buttonCount = 0;
	// Button creation
	Menu* _newMenu = new Menu();
	_newMenu->Init(Menu::MENU_MAIN, GetMeshResource("MainMenuBG"), Vector2(0,0), Vector2(m_viewWidth, m_viewHeight));
	_newMenu->AddButton(new UIButton(UIButton::BUTTON_NEW_GAME, GetMeshResource("BUTTON_NEW_GAME_OFF"), startPos - Vector2(0.f, HEIGHT_OFFSET * buttonCount), BUTTON_SIZE));
	++buttonCount;
	_newMenu->AddButton(new UIButton(UIButton::BUTTON_LEVEL_SELECT, GetMeshResource("BUTTON_LEVEL_SELECT_OFF"), startPos - Vector2(0.f, HEIGHT_OFFSET * buttonCount), BUTTON_SIZE));
	++buttonCount;
	_newMenu->AddButton(new UIButton(UIButton::BUTTON_INSTRUCTIONS, GetMeshResource("BUTTON_INSTRUCTIONS_OFF"), startPos - Vector2(0.f, HEIGHT_OFFSET * buttonCount), BUTTON_SIZE));
	++buttonCount;
	_newMenu->AddButton(new UIButton(UIButton::BUTTON_CREDITS, GetMeshResource("BUTTON_CREDITS_OFF"), startPos - Vector2(0.f, HEIGHT_OFFSET * buttonCount), BUTTON_SIZE));
	++buttonCount;
	_newMenu->AddButton(new UIButton(UIButton::BUTTON_EXIT, GetMeshResource("BUTTON_EXIT_OFF"), startPos - Vector2(0.f, HEIGHT_OFFSET * buttonCount), BUTTON_SIZE));
	++buttonCount;
	m__menu->AddMenu(_newMenu);

	/* Instructions */
	// Variables
	startPos = Vector2((m_viewWidth * 0.14f) - (BUTTON_SIZE.x * 0.5f), (m_viewHeight * 0.1f) - (BUTTON_SIZE.y * 0.5f));
	// Button creation
	_newMenu = new Menu();
	_newMenu->Init(Menu::MENU_INSTRUCTIONS, genericBG, Vector2(0,0), Vector2(m_viewWidth, m_viewHeight));
	_newMenu->AddButton(new UIButton(UIButton::BUTTON_RETURN_TO_MAIN_MENU, GetMeshResource("BUTTON_RETURN_TO_MAIN_MENU_OFF"), startPos, BUTTON_SIZE));
	m__menu->AddMenu(_newMenu);

	/* Credits */
	// Variables
	startPos = Vector2((m_viewWidth * 0.14f) - (BUTTON_SIZE.x * 0.5f), (m_viewHeight * 0.1f) - (BUTTON_SIZE.y * 0.5f));
	// Button creation
	_newMenu = new Menu();
	_newMenu->Init(Menu::MENU_CREDITS, genericBG, Vector2(0,0), Vector2(m_viewWidth, m_viewHeight));
	_newMenu->AddButton(new UIButton(UIButton::BUTTON_RETURN_TO_MAIN_MENU, GetMeshResource("BUTTON_RETURN_TO_MAIN_MENU_OFF"), startPos, BUTTON_SIZE));
	m__menu->AddMenu(_newMenu);

	/* Win level */
	// Variables
	startPos = Vector2((m_viewWidth * 0.5f) - (BUTTON_SIZE.x * 0.5f), (m_viewHeight * 0.5f) - (BUTTON_SIZE.y * 0.5f));
	buttonCount = 0;
	// Button creation
	_newMenu = new Menu();
	_newMenu->Init(Menu::MENU_WIN_LEVEL, GetMeshResource("WinMenuBG"), Vector2(0,0), Vector2(m_viewWidth, m_viewHeight));
	_newMenu->AddButton(new UIButton(UIButton::BUTTON_NEXT_STAGE, GetMeshResource("BUTTON_NEXT_STAGE_ON"), startPos - Vector2(0.f, HEIGHT_OFFSET * buttonCount), BUTTON_SIZE));
	++buttonCount;
	_newMenu->AddButton(new UIButton(UIButton::BUTTON_RETRY, GetMeshResource("BUTTON_RETRY_OFF"), startPos - Vector2(0.f, HEIGHT_OFFSET * buttonCount), BUTTON_SIZE));
	++buttonCount;
	_newMenu->AddButton(new UIButton(UIButton::BUTTON_RETURN_TO_MAIN_MENU, GetMeshResource("BUTTON_RETURN_TO_MAIN_MENU_OFF"), startPos - Vector2(0.f, HEIGHT_OFFSET * buttonCount), BUTTON_SIZE));
	++buttonCount;
	m__menu->AddMenu(_newMenu);

	/* Lose level */
	// Variables
	startPos = Vector2((m_viewWidth * 0.5f) - (BUTTON_SIZE.x * 0.5f), (m_viewHeight * 0.5f) - (BUTTON_SIZE.y * 0.5f));
	buttonCount = 0;
	// Button creation
	_newMenu = new Menu();
	_newMenu->Init(Menu::MENU_LOSE_LEVEL, GetMeshResource("LoseMenuBG"), Vector2(0,0), Vector2(m_viewWidth, m_viewHeight));
	_newMenu->AddButton(new UIButton(UIButton::BUTTON_RETRY, GetMeshResource("BUTTON_RETRY_OFF"), startPos - Vector2(0.f, HEIGHT_OFFSET * buttonCount), BUTTON_SIZE));
	++buttonCount;
	_newMenu->AddButton(new UIButton(UIButton::BUTTON_RETURN_TO_MAIN_MENU, GetMeshResource("BUTTON_RETURN_TO_MAIN_MENU_OFF"), startPos - Vector2(0.f, HEIGHT_OFFSET * buttonCount), BUTTON_SIZE));
	++buttonCount;
	m__menu->AddMenu(_newMenu);

	/* Pause */
	// Variables
	startPos = Vector2((m_viewWidth * 0.5f) - (BUTTON_SIZE.x * 0.5f), (m_viewHeight * 0.5f) - (BUTTON_SIZE.y * 0.5f));
	buttonCount = 0;
	// Button creation
	_newMenu = new Menu();
	_newMenu->Init(Menu::MENU_PAUSE, GetMeshResource("PauseMenuBG"), Vector2(0,0), Vector2(m_viewWidth, m_viewHeight));
	_newMenu->AddButton(new UIButton(UIButton::BUTTON_RESUME, GetMeshResource("BUTTON_RESUME_OFF"), startPos - Vector2(0.f, HEIGHT_OFFSET * buttonCount), BUTTON_SIZE));
	++buttonCount;
	_newMenu->AddButton(new UIButton(UIButton::BUTTON_RETRY, GetMeshResource("BUTTON_RETRY_OFF"), startPos - Vector2(0.f, HEIGHT_OFFSET * buttonCount), BUTTON_SIZE));
	++buttonCount;
	_newMenu->AddButton(new UIButton(UIButton::BUTTON_RETURN_TO_MAIN_MENU, GetMeshResource("BUTTON_RETURN_TO_MAIN_MENU_OFF"), startPos - Vector2(0.f, HEIGHT_OFFSET * buttonCount), BUTTON_SIZE));
	++buttonCount;
	m__menu->AddMenu(_newMenu);

	/* Level select */
	// Variables
	const float WIDTH_OFFSET = m_viewWidth * 0.3f;
	const Vector2 LEVEL_BUTTON_SIZE(m_viewWidth * 0.03125,m_viewHeight * 0.05555);
	startPos = Vector2((m_viewWidth * 0.2f) - (BUTTON_SIZE.x * 0.5f), (m_viewHeight * 0.8f) - (BUTTON_SIZE.y * 0.5f));
	buttonCount = 0;
	// Button creation
	_newMenu = new Menu();
	_newMenu->Init(Menu::MENU_LEVEL_SELECT, genericBG, Vector2(0,0), Vector2(m_viewWidth, m_viewHeight));

	UIButton* _newButton = new UIButton(UIButton::BUTTON_LEVEL, GetMeshResource("BUTTON_LEVEL_OFF"), startPos - Vector2(WIDTH_OFFSET * (static_cast<int>(buttonCount / 3)), HEIGHT_OFFSET * buttonCount), BUTTON_SIZE);
	_newButton->InitLevel(0, m_defaultFont, (startPos - Vector2((WIDTH_OFFSET * (static_cast<int>(buttonCount / 3))) - (m_viewWidth * 0.05f), (HEIGHT_OFFSET * buttonCount) - (m_viewHeight * 0.015f))) * 0.1, LEVEL_BUTTON_SIZE * 0.1, "Level 00"); 
	_newMenu->AddButton(_newButton);
	++buttonCount;

	_newButton = new UIButton(UIButton::BUTTON_LEVEL, GetMeshResource("BUTTON_LEVEL_OFF"), startPos - Vector2(WIDTH_OFFSET * (static_cast<int>(buttonCount / 3)), HEIGHT_OFFSET * buttonCount), BUTTON_SIZE);
	_newButton->InitLevel(1, m_defaultFont, (startPos - Vector2((WIDTH_OFFSET * (static_cast<int>(buttonCount / 3))) - (m_viewWidth * 0.05f), (HEIGHT_OFFSET * buttonCount) - (m_viewHeight * 0.015f))) * 0.1, LEVEL_BUTTON_SIZE * 0.1, "Level 01"); 
	_newMenu->AddButton(_newButton);
	++buttonCount;

	_newButton = new UIButton(UIButton::BUTTON_LEVEL, GetMeshResource("BUTTON_LEVEL_OFF"), startPos - Vector2(WIDTH_OFFSET * (static_cast<int>(buttonCount / 3)), HEIGHT_OFFSET * buttonCount), BUTTON_SIZE);
	_newButton->InitLevel(2, m_defaultFont, (startPos - Vector2((WIDTH_OFFSET * (static_cast<int>(buttonCount / 3))) - (m_viewWidth * 0.05f), (HEIGHT_OFFSET * buttonCount) - (m_viewHeight * 0.015f))) * 0.1, LEVEL_BUTTON_SIZE * 0.1, "Level 02"); 
	_newMenu->AddButton(_newButton);
	++buttonCount;

	startPos = Vector2((m_viewWidth * 0.14f) - (BUTTON_SIZE.x * 0.5f), (m_viewHeight * 0.1f) - (BUTTON_SIZE.y * 0.5f));
	_newMenu->AddButton(new UIButton(UIButton::BUTTON_RETURN_TO_MAIN_MENU, GetMeshResource("BUTTON_RETURN_TO_MAIN_MENU_OFF"), startPos, BUTTON_SIZE));
	m__menu->AddMenu(_newMenu);

	// Must assign at least the starting menu if not menu will crash
	m__menu->AssignCurrent(Menu::MENU_MAIN);
}

void MVC_Model_Spectre::initHUD(void)
{
	int hudCount = 0;
	const float HUD_OFFSET = static_cast<float>(m_viewHeight) * 0.15f;
	if (m__alert == NULL)
	{
		m__alert = new HUD_Cooldown();
	}
	const Vector2 ALERT_SCALE(static_cast<float>(m_viewWidth) * 0.2f, static_cast<float>(m_viewHeight) * 0.1f);
	const Vector2 ALERT_POS(static_cast<float>(m_viewWidth) - ALERT_SCALE.x, static_cast<float>(m_viewHeight) - ALERT_SCALE.y - (hudCount * HUD_OFFSET));
	m__alert->Init(GetMeshResource("AlertBar"), ALERT_POS, ALERT_SCALE, GetMeshResource("AlertCover"), ALERT_POS, ALERT_SCALE, true, false);
	++hudCount;

	if (m__spectreDive == NULL)
	{
		m__spectreDive = new HUD_Cooldown();
	}
	const Vector2 DIVE_SCALE(static_cast<float>(m_viewHeight) * 0.1f, static_cast<float>(m_viewHeight) * 0.1f);
	const Vector2 DIVE_POS(static_cast<float>(m_viewWidth) - DIVE_SCALE.x, static_cast<float>(m_viewHeight) - DIVE_SCALE.y - (hudCount * HUD_OFFSET));
	m__spectreDive->Init(GetMeshResource("Skill_DiveIcon"), DIVE_POS, DIVE_SCALE, GetMeshResource("Skill_Cover"), DIVE_POS, DIVE_SCALE, false, true);
	++hudCount;

	if (m__spectreJump == NULL)
	{
		m__spectreJump = new HUD_Cooldown();
	}
	const Vector2 JUMP_SCALE(static_cast<float>(m_viewHeight) * 0.1f, static_cast<float>(m_viewHeight) * 0.1f);
	const Vector2 JUMP_POS(static_cast<float>(m_viewWidth) - JUMP_SCALE.x, static_cast<float>(m_viewHeight) - JUMP_SCALE.y - (hudCount * HUD_OFFSET));
	m__spectreJump->Init(GetMeshResource("Skill_JumpIcon"), JUMP_POS, JUMP_SCALE, GetMeshResource("Skill_Cover"), JUMP_POS, JUMP_SCALE, false, true);
	++hudCount;

	if (m__spectreHost == NULL)
	{
		m__spectreHost = new HUD_Cooldown();
	}
	const Vector2 HOST_SCALE(static_cast<float>(m_viewHeight) * 0.1f, static_cast<float>(m_viewHeight) * 0.1f);
	const Vector2 HOST_POS(static_cast<float>(m_viewWidth) - JUMP_SCALE.x, static_cast<float>(m_viewHeight) - JUMP_SCALE.y - (hudCount * HUD_OFFSET));
	m__spectreHost->Init(GetMeshResource("Skill_HostIcon"), HOST_POS, HOST_SCALE, GetMeshResource("Skill_Cover"), HOST_POS, HOST_SCALE, false, true);
	++hudCount;


	if (m__defuseBomb == NULL)
	{
		m__defuseBomb = new HUD_Cooldown();
	}
	const Vector2 DEFUSE_SCALE(static_cast<float>(m_viewHeight)* 0.6f, static_cast<float>(m_viewHeight)* 0.15f);
	const Vector2 DEFUSE_POS(static_cast<float>(m_viewWidth)*0.5f - (DEFUSE_SCALE.x *0.5f), static_cast<float>(m_viewHeight)*0.25f - (DEFUSE_SCALE.y * 0.5f));
	m__defuseBomb->Init(GetMeshResource("Defuse_Bomb"), DEFUSE_POS, DEFUSE_SCALE, GetMeshResource("Skill_Cover"), DEFUSE_POS, DEFUSE_SCALE, true, false);

	if (m__plantBomb == NULL)
	{
		m__plantBomb = new HUD_Cooldown();
	}
	const Vector2 PLANT_SCALE(static_cast<float>(m_viewHeight)* 0.6f, static_cast<float>(m_viewHeight)* 0.15f);
	const Vector2 PLANT_POS(static_cast<float>(m_viewWidth)*0.5f - (PLANT_SCALE.x *0.5f), static_cast<float>(m_viewHeight)*0.25f - (PLANT_SCALE.y*0.5f));
	m__plantBomb->Init(GetMeshResource("Plant_Bomb"), PLANT_POS, PLANT_SCALE, GetMeshResource("Skill_Cover"), PLANT_POS, PLANT_SCALE, true, false);

	// Action prompt HUD
	float actionKeySize = m__currentLevel->GetTileMap()->GetTileSize() * 0.5f;
	const Vector2 ACTION_KEY_SCALE(actionKeySize, actionKeySize);
	if (m__fKey == NULL)
	{
		m__fKey = new HUD();
	}
	m__fKey->Init(GetMeshResource("F_Key"), Vector2(0,0), ACTION_KEY_SCALE);
	m__fKey->SetActive(false);

	if (m__kKey == NULL)
	{
		m__kKey = new HUD();
	}
	m__kKey->Init(GetMeshResource("K_Key"), Vector2(0,0), ACTION_KEY_SCALE);
	m__kKey->SetActive(false);

	if (m__objectivesHUD == NULL)
	{
		m__objectivesHUD = new HUD_Objectives();
	}
	m__objectivesHUD->Init(NULL, Vector2(0, m_viewHeight * 0.8f), Vector2(1,1), m_defaultFont, "", Vector2(0, m_viewHeight * 0.8f) * 0.1, Vector2 (40,40) * 0.1);
	m__objectivesHUD->Update(0.0, m_objective);
}

void MVC_Model_Spectre::updateMainGame(double dt)
{
	if (m_pauseTimer > 0.f)
	{
		m_pauseTimer -= dt;
	}
	m__objectivesHUD->Update(dt, m_objective);
	// Updates player depending on actions queued.
	m__player->Update(dt, m__currentLevel->GetTileMap());

	// Update the messenger
	m_messenger.Update(dt);

	// Update camera list
	updateCamera(dt);

	// Player inside viewed tile
	static const double S_ALERT_SPEED = 4.0;
	Tile* _tile = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos());
	if (_tile->IsViewed() && !m__player->GetInShadow()) // Player is viewed by some
	{
		_tile->NotifyViewer(m__player->GetMapTilePos(), m_alertLevel, dt * S_ALERT_SPEED);
	}

	//updating bomb if BOOM : not defused in time
	ObjectiveDefuse* _tempDefuseObjective = dynamic_cast<ObjectiveDefuse* >(m__currentLevel->GetObjective());	

	//if not completed
	if(!m__currentLevel->GetObjectiveComplete());
	{
		if(_tempDefuseObjective)
		{
			if(_tempDefuseObjective->GetTimeTillBOOM() <= 0)
			{
				m_appState = AS_MENU;
				m__menu->AssignCurrent(Menu::MENU_LOSE_LEVEL);
				m__soundPlayer[SP_EXPLOSIVE_DEATH]->Play(false);
				_tempDefuseObjective->ResetBombTimer();
			}
		}
	}
	//if hostage inside tile being viewed
	/*_tile = m__currentLevel->GetTileMap()->GetTileAt(m__currentLevel->GetTarget()->GetMapPos());
	if(_tile->IsViewed() )
	{
		_tile->NotifyViewer(m__currentLevel->GetTarget()->GetMapTilePos() , m_alertLevel, dt * S_ALERT_SPEED);
	}*/

	//update enemies
	/*ObjectiveAssassinate* _objectiveTarget = dynamic_cast<ObjectiveAssassinate*>(m__currentLevel->GetObjective());*/
	for (vector<NPC*>::iterator enemyIter = m_enemyList.begin(); enemyIter != m_enemyList.end(); ++enemyIter)
	{
		NPC* _enemy = (*enemyIter);
		_enemy->SetAlertLevel(m_alertLevel);
		if (_enemy->Update(dt, m__currentLevel->GetTileMap())) // Killed player
		{
			m_appState = AS_MENU;
			m__menu->AssignCurrent(Menu::MENU_LOSE_LEVEL);
			m__soundPlayer[SP_DEATH]->Play(false);
			break;
		}
		
	}
	/*if (m__player->GetHealth() <= 0)
	{
		std::cout << "K.O." << std::endl;
	}*/
	// Update Lighting
	updateLighting(dt);

	// Update alert system
	updateHUD(dt);


	// Rendering
	tileMapToRender(m__currentLevel->GetTileMap());

	// -- Render Player
	m_renderList2D.push(m__player);

	// Render Enemies
	for (vector<NPC*>::iterator enemyIter = m_enemyList.begin(); enemyIter != m_enemyList.end(); ++enemyIter)
	{
		//check if its the same enemy killed
		NPC* _temp = *enemyIter;
		//if NOT the target and objective is NOT 
		//if (_objectiveTarget!= NULL && !(_objectiveTarget->GetTarget()->GetNPCType() == _temp->GetNPCType() && _objectiveTarget->IsCompleted()))
		if (_temp && _temp->GetActive())
		{
			m_renderList2D.push((*enemyIter));
		}
		
	}

	// Render HUD
	// Alert
	m_renderList2D.push(m__alert);
	m_renderList2D.push(m__alert->GetDisplayCover());
	// Dive skill
	m_renderList2D.push(m__spectreDive);
	m_renderList2D.push(m__spectreDive->GetDisplayCover());
	// Jump skill
	m_renderList2D.push(m__spectreJump);
	m_renderList2D.push(m__spectreJump->GetDisplayCover());
	// Host skill
	m_renderList2D.push(m__spectreHost);
	m_renderList2D.push(m__spectreHost->GetDisplayCover());
	if (m_shadowMode == false && (m__player->Interact(Player::INTERACT_DEFUSE, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_DEFUSE))
	{
		// Defuse Bomb
		m_renderList2D.push(m__defuseBomb);
		m_renderList2D.push(m__defuseBomb->GetDisplayCover());
	}
	if (m_shadowMode == false && (m__player->Interact(Player::INTERACT_SETBOMB, m__currentLevel->GetTileMap()) == Player::PS_SPECTRAL_SETBOMB))
	{
		// Plant Bomb
		m_renderList2D.push(m__plantBomb);
		m_renderList2D.push(m__plantBomb->GetDisplayCover());
	}
	// Action prompts
	float tileSize = m__currentLevel->GetTileMap()->GetTileSize();
	Tile* _tileOnPlayer = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos());
	Tile::E_TILE_TYPE tileTypeOnPlayer = _tileOnPlayer->GetType();
	Vector2 frontTilePos = m__player->GetMapPos() + (m__player->GetLookDir() * tileSize);
	frontTilePos.x = floor(frontTilePos.x / tileSize);
	frontTilePos.y = floor(frontTilePos.y / tileSize);
	Tile* _tileInFrontPlayer = NULL;
	Tile::E_TILE_TYPE tileTypeInFrontPlayer;
	if (frontTilePos.x >= 0 && frontTilePos.x < m__currentLevel->GetTileMap()->GetNumScreenTile().x && frontTilePos.y >= 0 && frontTilePos.y < m__currentLevel->GetTileMap()->GetNumScreenTile().y)
	{
		_tileInFrontPlayer = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos() + (m__player->GetLookDir() * tileSize));
		tileTypeInFrontPlayer = _tileInFrontPlayer->GetType();
	}
	Vector2 playerDir = m__player->GetLookDir();
	Vector2 spawnDir;
	const Vector2 S_ACTION_HUD_OFFSET(tileSize * 0.25f, tileSize * 0.25f);
	if (tileTypeOnPlayer == Tile::TILE_OBJ_CAMERA_ON_1_1
		||
		tileTypeOnPlayer == Tile::TILE_OBJ_CAMERA_ON_1_2
		||
		tileTypeOnPlayer == Tile::TILE_OBJ_CAMERA_ON_1_3
		||
		tileTypeOnPlayer == Tile::TILE_OBJ_CAMERA_ON_1_4
		||
		(_tileInFrontPlayer && (tileTypeInFrontPlayer == Tile::TILE_OBJ_CAMERA_ON_1_1
			||
			tileTypeInFrontPlayer == Tile::TILE_OBJ_CAMERA_ON_1_2
			||
			tileTypeInFrontPlayer == Tile::TILE_OBJ_CAMERA_ON_1_3
			||
			tileTypeInFrontPlayer == Tile::TILE_OBJ_CAMERA_ON_1_4))) // Spawn K key if camera
	{
		// Spawn hud key to the right of where player is
		if (playerDir == Direction::DIRECTIONS[Direction::DIR_UP])			// If facing up, set spawn direction to right
		{
			spawnDir = Direction::DIRECTIONS[Direction::DIR_RIGHT];
		}
		else if (playerDir == Direction::DIRECTIONS[Direction::DIR_RIGHT])	// If facing right, set spawn direction to down
		{
			spawnDir = Direction::DIRECTIONS[Direction::DIR_DOWN];
		}
		else if (playerDir == Direction::DIRECTIONS[Direction::DIR_DOWN])	// If facing down, set spawn direction to left
		{
			spawnDir = Direction::DIRECTIONS[Direction::DIR_LEFT];
		}
		else if (playerDir == Direction::DIRECTIONS[Direction::DIR_LEFT])	// If facing left, set spawn direction to up
		{
			spawnDir = Direction::DIRECTIONS[Direction::DIR_UP];
		}
		m__kKey->SetActive(true);
		m__kKey->SetPos((m__player->GetMapPos() - m__currentLevel->GetTileMap()->GetScrollOffset()) + (spawnDir * tileSize) + S_ACTION_HUD_OFFSET);
		m_renderList2D.push(m__kKey);
	}
	else if (tileTypeOnPlayer == Tile::TILE_BOMB
		||
		tileTypeOnPlayer == Tile::TILE_DOCUMENT
		||
		tileTypeOnPlayer == Tile::TILE_SETBOMBAREA
		||
		tileTypeOnPlayer == Tile::TILE_EXTRACTION
		||
		(_tileInFrontPlayer && (tileTypeInFrontPlayer == Tile::TILE_BOMB
			||
			tileTypeInFrontPlayer == Tile::TILE_DOCUMENT
			||
			tileTypeInFrontPlayer == Tile::TILE_SETBOMBAREA))) // Spawn F if interactions
	{
		// Spawn hud key to the right of where player is
		if (playerDir == Direction::DIRECTIONS[Direction::DIR_UP])			// If facing up, set spawn direction to right
		{
			spawnDir = Direction::DIRECTIONS[Direction::DIR_RIGHT];
		}
		else if (playerDir == Direction::DIRECTIONS[Direction::DIR_RIGHT])	// If facing right, set spawn direction to down
		{
			spawnDir = Direction::DIRECTIONS[Direction::DIR_DOWN];
		}
		else if (playerDir == Direction::DIRECTIONS[Direction::DIR_DOWN])	// If facing down, set spawn direction to left
		{
			spawnDir = Direction::DIRECTIONS[Direction::DIR_LEFT];
		}
		else if (playerDir == Direction::DIRECTIONS[Direction::DIR_LEFT])	// If facing left, set spawn direction to up
		{
			spawnDir = Direction::DIRECTIONS[Direction::DIR_UP];
		}
		m__fKey->SetActive(true);
		m__fKey->SetPos((m__player->GetMapPos() - m__currentLevel->GetTileMap()->GetScrollOffset()) + (spawnDir * tileSize) + S_ACTION_HUD_OFFSET);
		m_renderList2D.push(m__fKey);
	}
	else
	{
		m__fKey->SetActive(false);
		m__kKey->SetActive(false);
	}
	// Objectives
	m_renderList2D.push(m__objectivesHUD);
	if (m__objectivesHUD->GetObjectiveText())
	{
		m_renderList2D.push(m__objectivesHUD->GetObjectiveText());
	}

	// Render Messages
	pushMessageToRender();

	m_renderList2D.push(m_fpsCount);
	if (m_fps < 55)
	{
		cout << m_fpsCount->GetText() << endl;
	}
}

void MVC_Model_Spectre::updateHackGame(double dt)
{
	m_hackingGame.Update(dt);

	if (m_hackingGame.IsVictory())
	{
		// Deactivate camera from camera list and tilemap
		for (vector<SecurityCamera*>::iterator it = m_cameraList.begin(); it != m_cameraList.end(); ++it)
		{
			SecurityCamera* _camera = *it;
			Vector2 cameraTilePos = _camera->GetMapTilePos();
			Vector2 playerTilePos = m__player->GetMapTilePos();
			Vector2 playerFrontTilePos = playerTilePos + m__player->GetLookDir();
			if (cameraTilePos == playerTilePos || cameraTilePos == playerFrontTilePos) // Found camera
			{
				_camera->DestroyViewBox(m__currentLevel->GetTileMap());
				_camera->SetState(false);
				// Change mesh for camera and tile obj
				if (_camera->GetDir() == Direction::DIRECTIONS[Direction::DIR_UP])
				{
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_UP]);
					// Force set tilemap
					if (cameraTilePos == playerTilePos)
					{
						Tile* _tile = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos());
						_tile->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_UP]);
						_tile->SetType(Tile::TILE_OBJ_CAMERA_OFF_1_3);
					}
					else if (cameraTilePos == playerFrontTilePos)
					{
						Tile* _tile = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos() + (m__player->GetLookDir() * m__currentLevel->GetTileMap()->GetTileSize()));
						_tile->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_UP]);
						_tile->SetType(Tile::TILE_OBJ_CAMERA_OFF_1_3);
					}
				}
				else if (_camera->GetDir() == Direction::DIRECTIONS[Direction::DIR_DOWN])
				{
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_DOWN]);
					// Force set tilemap
					if (cameraTilePos == playerTilePos)
					{
						Tile* _tile = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos());
						_tile->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_DOWN]);
						_tile->SetType(Tile::TILE_OBJ_CAMERA_OFF_1_1);
					}
					else if (cameraTilePos == playerFrontTilePos)
					{
						Tile* _tile = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos() + (m__player->GetLookDir() * m__currentLevel->GetTileMap()->GetTileSize()));
						_tile->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_DOWN]);
						_tile->SetType(Tile::TILE_OBJ_CAMERA_OFF_1_1);
					}
				}
				else if (_camera->GetDir() == Direction::DIRECTIONS[Direction::DIR_LEFT])
				{
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_LEFT]);
					// Force set tilemap
					if (cameraTilePos == playerTilePos)
					{
						Tile* _tile = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos());
						_tile->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_LEFT]);
						_tile->SetType(Tile::TILE_OBJ_CAMERA_OFF_1_2);
					}
					else if (cameraTilePos == playerFrontTilePos)
					{
						Tile* _tile = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos() + (m__player->GetLookDir() * m__currentLevel->GetTileMap()->GetTileSize()));
						_tile->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_LEFT]);
						_tile->SetType(Tile::TILE_OBJ_CAMERA_OFF_1_2);
					}
				}
				else if (_camera->GetDir() == Direction::DIRECTIONS[Direction::DIR_RIGHT])
				{
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_RIGHT]);
					// Force set tilemap
					if (cameraTilePos == playerTilePos)
					{
						Tile* _tile = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos());
						_tile->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_RIGHT]);
						_tile->SetType(Tile::TILE_OBJ_CAMERA_OFF_1_4);
					}
					else if (cameraTilePos == playerFrontTilePos)
					{
						Tile* _tile = m__currentLevel->GetTileMap()->GetTileAt(m__player->GetMapPos() + (m__player->GetLookDir() * m__currentLevel->GetTileMap()->GetTileSize()));
						_tile->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_RIGHT]);
						_tile->SetType(Tile::TILE_OBJ_CAMERA_OFF_1_4);
					}
				}
				break;
			}
		}

		m_appState = AS_MAIN_GAME;
	}
	else if (m_hackingGame.IsLoss())
	{
		m_appState = AS_MAIN_GAME;
	}

	// If end the hacking game
	if (m_appState == AS_MAIN_GAME)
	{
		m__player->SetState(Player::PS_IDLE_DOWN);
		m__soundPlayer[SP_SKILL_HACK_STOP]->Play(false);
	}

	vector<GameObject2D*> minigameObjects = m_hackingGame.GetRenderObjects();
	for (vector<GameObject2D*>::iterator go = minigameObjects.begin(); go != minigameObjects.end(); ++go)
	{
		m_renderList2D.push(*go);
	}
}

void MVC_Model_Spectre::initPlayer(void)
{
	m__player = Player::GetInstance();
	m__player->Init(GetMeshResource("Player_ANIMATION"));
	//player mesh and states
	//SpriteAnimation* _sa = dynamic_cast<SpriteAnimation* >(GetMeshResource("Player_ANIMATION"));
	//m__player->SetMesh(_sa);
	Animation* _a;

	//walking south
	_a = new Animation();
	_a->Set(0, 2, 0, 0.4f);
	m__player->AddAnimation(_a, Player::PS_WALK_DOWN);
	//walking right
	_a = new Animation();
	_a->Set(3, 5, 0, 0.4f); 
	m__player->AddAnimation(_a, Player::PS_WALK_RIGHT);
	//walk left
	_a = new Animation();
	_a->Set(6, 8, 0, 0.4f);
	m__player->AddAnimation(_a, Player::PS_WALK_LEFT);
	//walk north
	_a = new Animation();
	_a->Set(9, 11, 0, 0.4f);
	m__player->AddAnimation(_a, Player::PS_WALK_UP);
	//idle south
	_a = new Animation();
	_a->Set(1,1,0,0.f);
	m__player->AddAnimation(_a , Player::PS_IDLE_DOWN);
	//idle right
	_a = new Animation();
	_a->Set(5,5,0,0.f);
	m__player->AddAnimation(_a, Player::PS_IDLE_RIGHT);
	//idle left
	_a = new Animation();
	_a->Set(7,7, 0, 0.f);
	m__player->AddAnimation(_a, Player::PS_IDLE_LEFT);
	//idle north
	_a = new Animation();
	_a->Set(10, 10, 0, 0.f);
	m__player->AddAnimation(_a , Player::PS_IDLE_UP);
	
	
	//diving South
	_a = new Animation();
	_a->Set(12, 17, 1, 0.3f);
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVING_DOWN);
	//diving right
	_a = new Animation();
	_a->Set(18, 23, 1, 0.3f);
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVING_RIGHT);
	//diving left
	_a = new Animation();
	_a->Set(24, 29, 1, 0.3f);
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVING_LEFT);
	//diving north
	_a = new Animation();
	_a->Set(30, 35, 1, 0.3f);
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVING_UP);

	//shadow form north
	_a = new Animation();
	_a->Set(36, 36, 0, 0.f);
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVE_UP);
	// must set a animation or else cannot use and dont need to change much code
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVE); 
	//shadow form south
	_a = new Animation();
	_a->Set(37, 37, 0, 0.f);
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVE_DOWN);
	//shadow form left
	_a = new Animation();
	_a->Set(38, 38, 0, 0.f);
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVE_LEFT);
	//shadow form right
	_a = new Animation();
	_a->Set(39, 39, 0, 0.f);
	m__player->AddAnimation(_a , Player::PS_SPECTRAL_DIVE_RIGHT);

	//set player health
	m__player->SetMaxHealth(S_M_PLAYER_MAX_HP);
	m__player->SetHealth(m__player->GetMaxHealth());
}

void MVC_Model_Spectre::Update(double dt)
{
	MVC_Model::Update(dt);

	switch (m_appState)
	{
		case AS_MAIN_GAME:
		{
			updateMainGame(dt);
			break;
		}
		case AS_HACK_GAME:
		{
			updateHackGame(dt);
			break;
		}
		case AS_MENU:
		{
			updateMenu(dt);
			break;
		}
	}
}

void MVC_Model_Spectre::Exit(void)
{
	clearCameraList();

	m_hackingGame.Exit();

	Player::Clear();

	// Delete the messenger
	m_messenger.Exit();

	//clearing list
	while(m__colliderList.size() > 0)
	{
		Collider2D *go = m__colliderList.back();
		delete go;
		m__colliderList.pop_back();
	}

	while (m_enemyList.size() > 0)
	{
		if (m_enemyList.back() != NULL)
		{
			delete m_enemyList.back();
		}

		m_enemyList.pop_back();
	}

	// Clear the level
	if (m__currentLevel != NULL)
	{
		m__currentLevel->Clear();
		delete m__currentLevel;
		m__currentLevel = NULL;
	}

	// Clear the tile marker list
	while (m__tileMarkers.size() > 0)
	{
		GameObject2D* go = m__tileMarkers.back();

		if (go != NULL)
		{
			delete go;
			m__tileMarkers.pop_back();
		}
	}

	// Clear alert hud
	if (m__alert)
	{
		m__alert->Clear();
		delete m__alert;
		m__alert = NULL;
	}

	if (m__spectreDive)
	{
		m__spectreDive->Clear();
		delete m__spectreDive;
		m__spectreDive = NULL;
	}

	if (m__spectreJump)
	{
		m__spectreJump->Clear();
		delete m__spectreJump;
		m__spectreJump = NULL;
	}

	if (m__spectreHost)
	{
		m__spectreHost->Clear();
		delete m__spectreHost;
		m__spectreHost = NULL;
	}
	
	MVC_Model::Exit();
}

void MVC_Model_Spectre::updateMenu(double dt)
{
	if (m_menuKeysInputTimer > 0.f)
	{
		m_menuKeysInputTimer -= dt;
	}
	m__menu->MouseUpdate(dt, m_mousePosX, m_viewHeight - m_mousePosY);

	// Add menu background for render
	if (m__menu->GetCurrentMenu()->GetBackground() != NULL) // Background exists
	{
		m_renderList2D.push(m__menu->GetCurrentMenu()->GetBackground());
	}
	vector<UIButton*> buttonList = m__menu->GetCurrentMenu()->GetButtonList();
	for (vector<UIButton*>::iterator buttonIter = buttonList.begin(); buttonIter != buttonList.end(); ++buttonIter)
	{
		UIButton* _button = *buttonIter;
		if (_button)
		{
			m_renderList2D.push(_button);
			if (_button->GetTextObj())
			{
				m_renderList2D.push(_button->GetTextObj());
			}
		}
	}
}

void MVC_Model_Spectre::updateLighting(double dt)
{
	m_lightUpdateTimer += dt;

	if (m_lightUpdateTimer >= S_M_LIGHTING_UPDATE_FREQUENCY)
	{
		vector<Vector2> shadowCasters;
		// Give enemies a shadow
		for (vector<NPC*>::iterator enemyIter = m_enemyList.begin(); enemyIter != m_enemyList.end(); ++enemyIter)
		{
			shadowCasters.push_back((*enemyIter)->GetMapTilePos());
		}
		//shadowCasters.push_back(m__player->GetMapTilePos());
		m__currentLevel->GetTileMap()->UpdateLighting(shadowCasters);

		m_lightUpdateTimer = 0.0;
	}
}

void MVC_Model_Spectre::tileMapToRender(TileMap* _ToRender)
{
	const Vector2 MAX_SCROLL_SIZE_TILE = _ToRender->GetNumMapTile(); // Max tile difference between map and screen
	vector<vector<Tile*>*> _map = _ToRender->GetMap();

	// Reset the lights from the previous frame rendered
	resetTileMarkers();

	// Calc the starting tile to render and round down any decimal as it is still seen
	Vector2 tileStart
		( 
			floor(_ToRender->GetScrollOffset().x / _ToRender->GetTileSize()),
			floor(_ToRender->GetScrollOffset().y / _ToRender->GetTileSize()) 
		);

	for (int row = 0; row < ceil(_ToRender->GetNumScreenTile().y) + 1; ++row)			// Loop for rows
	{
		for (int col = 0; col < ceil(_ToRender->GetNumScreenTile().x) + 1; ++col)	// Loop for columns (+1 for the offset)
		{
			if (row >= _ToRender->GetNumMapTile().y || col >= _ToRender->GetNumMapTile().x) // Stop the rendering if row and col goes out of map
			{
				break;
			}
			if (tileStart.y + row < 0 || tileStart.y + row >= MAX_SCROLL_SIZE_TILE.y || tileStart.x + col < 0 || tileStart.x + col >= MAX_SCROLL_SIZE_TILE.x)
			{
				continue;
			}
			Tile* _tile = (*_map[tileStart.y + row])[tileStart.x + col]; // Get the tile data based on loop
			_tile->SetMapPosition(_tile->GetMapPos(), _ToRender->GetScrollOffset(), m__currentLevel->GetTileMap()->GetTileSize()); // Calculate screen position based on map position for rendering
			if (m__player->GetInShadow() && _tile->GetLightLevel() > Player::S_SPECTRE_DIVE_LIGHT_LIMIT)
			{
				continue;
			}
			else
			{
				_tile->SetMapPosition(_tile->GetMapPos(), _ToRender->GetScrollOffset(), m__currentLevel->GetTileMap()->GetTileSize()); // Calculate screen position based on map position for rendering
				m_renderList2D.push(_tile); // Add to queue for rendering

				/*
				* Shadow Portion
				*/
				if (m_enableShadow && _tile->GetType() != Tile::TILE_INVISIBLE_WALL)
				{
					int numShadows = Tile::MAX_LIGHT_LEVEL - _tile->GetLightLevel();
					for (size_t lightLevel = 0; lightLevel < numShadows; lightLevel += TileMap::S_LIGHT_ATTENUATION)
					{
						Transform tileT = _tile->GetTransform();
						GameObject2D* _shadow = fetchTileMarker(TM_SHADOW);
						_shadow->SetPos(tileT.Translation);
						_shadow->SetScale(tileT.Scale);
						m_renderList2D.push(_shadow);
					}
				}

				if (_tile->IsViewed())
				{
					Transform tileT = _tile->GetTransform();
					GameObject2D* light = fetchTileMarker(TM_VIEWED);
					light->SetPos(tileT.Translation);
					light->SetScale(tileT.Scale);
					m_renderList2D.push(light);
				}
			}
		}	
	}
}

void MVC_Model_Spectre::onResolutionChanged(int oldViewWidth, int oldViewHeight)
{
	if (m__currentLevel != NULL)
	{
		TileMap* _tilemap = m__currentLevel->GetTileMap();
		float oldTileSize = _tilemap->GetTileSize();
		_tilemap->SetTileSize(m_viewWidth / _tilemap->GetNumScreenTile().x);
		float newTileSize = _tilemap->GetTileSize();
		// Update the tilemap
		resizeTileMap(oldTileSize, newTileSize);

		// Update camera list position
		for (vector<SecurityCamera*>::iterator cameraIter = m_cameraList.begin(); cameraIter != m_cameraList.end(); ++cameraIter)
		{
			SecurityCamera* _camera = *cameraIter;
			Vector2 newPosition(floor(_camera->GetMapPos().x / oldTileSize) * newTileSize, floor(_camera->GetMapPos().y / oldTileSize) * newTileSize);
			_camera->SetMapPosition(newPosition, _tilemap->GetScrollOffset(), newTileSize);
		}

		// Update HUD position
		initHUD();

		// Update enemy position
		for (vector<NPC*>::iterator enemyIter = m_enemyList.begin(); enemyIter != m_enemyList.end(); ++enemyIter)
		{
			NPC* _enemy = *enemyIter;
			if (_enemy)
			{
				Vector2 enemyTilePos(floor(_enemy->GetMapPos().x / oldTileSize), floor(_enemy->GetMapPos().y / oldTileSize));
				_enemy->SetMapPosition(enemyTilePos * newTileSize, _tilemap->GetScrollOffset(), newTileSize);
				_enemy->SetScale(Vector2(newTileSize, newTileSize));
			}
		}

		// Update the Messenger
		m_messenger.SetMessageBGScale(Vector2(m_viewWidth - S_M_MESSAGE_OFFSET.x * 2, m_messenger.GetMessageBGScale().y));
	}
}

void MVC_Model_Spectre::resizeTileMap(float oldTileSize, float newTileSize)
{
	TileMap* _tilemap = m__currentLevel->GetTileMap();
	vector<vector<Tile*>*> _map = _tilemap->GetMap();
	Vector2 playerTilePos(floor(m__player->GetMapPos().x / oldTileSize), floor(m__player->GetMapPos().y / oldTileSize));
	Vector2 mapScrollOffset(ceil(_tilemap->GetScrollOffset().x / oldTileSize), ceil(_tilemap->GetScrollOffset().y / oldTileSize));
	/*if (oldViewWidth < m_viewWidth) // Scale up screen
	{
		++mapScrollOffset.y;
		_tilemap->SetTileSize(m_viewWidth / _tilemap->GetNumScreenTile().x);
	}
	else if (oldViewWidth > m_viewWidth) // Scale down screen
	{
		--mapScrollOffset.y;
		_tilemap->SetTileSize(m_viewWidth / _tilemap->GetNumScreenTile().x);
	}*/
	_tilemap->SetScrollOffset(mapScrollOffset * newTileSize);
	for (int row = 0; row < _tilemap->GetNumMapTile().y; ++row)
	{
		for (int col = 0; col < _tilemap->GetNumMapTile().x; ++col)
		{
			_tilemap->SetMapSize(_tilemap->GetNumMapTile() * newTileSize);
			(*_map[row])[col]->SetMapPosition(Vector2(col * newTileSize, row * newTileSize), _tilemap->GetScrollOffset(), newTileSize);
			(*_map[row])[col]->SetScale(Vector2(newTileSize, newTileSize));
		}
	}
	m__player->SetMapPosition(playerTilePos * newTileSize, _tilemap->GetScrollOffset(), newTileSize);
	m__player->SetScale(Vector2(newTileSize, newTileSize));
}

void MVC_Model_Spectre::updateHUD(double dt)
{
	if (m_alertLevel > S_M_MAX_ALERT)
	{
		m_alertLevel = S_M_MAX_ALERT;
	}
	if (m_alertLevel > 0.f)
	{
		m_alertLevel -= dt * 0.2f;
		if (m_alertLevel < 0.f)
		{
			m_alertLevel = 0.f;
		}
	}
	m__alert->Update(dt, m_alertLevel, S_M_MAX_ALERT);

	ObjectiveDefuse* temp_defuse = dynamic_cast<ObjectiveDefuse*>(m_objective);
	if (temp_defuse)
	{
		//defusing bomb time;
		static const double S_M_MAX_DEFUSE_TIME = temp_defuse->GetMexTime();
		m__defuseBomb->Update(dt, S_M_MAX_DEFUSE_TIME - temp_defuse->GetTimer(), S_M_MAX_DEFUSE_TIME);
	}

	ObjectiveSetBomb* temp_plant = dynamic_cast<ObjectiveSetBomb*>(m_objective);
	if (temp_plant)
	{
		m__plantBomb->Update(dt, ObjectiveSetBomb::S_M_MAX_PLANT_TIME - temp_plant->GetTimer(), ObjectiveSetBomb::S_M_MAX_PLANT_TIME);
	}

	m__spectreDive->Update(dt, Player::S_SPECTRE_DIVE_COOLDOWN - m__player->GetDiveTimer(), Player::S_SPECTRE_DIVE_COOLDOWN);
	m__spectreJump->Update(dt, Player::S_SPECTRE_JUMP_COOLDOWN - m__player->GetJumpTimer(), Player::S_SPECTRE_JUMP_COOLDOWN);
	if (!m__player->GetInShadow())
	{
		m__spectreHost->Update(dt, Player::S_SPECTRE_DIVE_COOLDOWN - m__player->GetDiveTimer(), Player::S_SPECTRE_DIVE_COOLDOWN);
	}
	else
	{
		m__spectreHost->Update(dt, 0.f, Player::S_SPECTRE_DIVE_COOLDOWN);
	}
}

void MVC_Model_Spectre::updateCamera(double dt)
{
	for (vector<SecurityCamera*>::iterator it = m_cameraList.begin(); it != m_cameraList.end(); ++it)
	{
		SecurityCamera* _camera = (SecurityCamera*)*it;
		if (_camera) // If camera exists, update it
		{
			_camera->Update(dt, m__currentLevel->GetTileMap());
			_camera->SetMapPosition(_camera->GetMapPos(), m__currentLevel->GetTileMap()->GetScrollOffset(), m__currentLevel->GetTileMap()->GetTileSize());
		}
	}
}

void MVC_Model_Spectre::clearCameraList()
{
	while (m_cameraList.size() > 0)
	{
		SecurityCamera* _camera = m_cameraList.back();
		if (_camera)
		{
			delete _camera;
		}
		m_cameraList.pop_back();
	}
}

void MVC_Model_Spectre::clearEnemyList()
{
	while (m_enemyList.size() > 0)
	{
		NPC* _enemy = m_enemyList.back();
		if (_enemy)
		{
			delete _enemy;
		}
		m_enemyList.pop_back();
	}
}

void MVC_Model_Spectre::loadToList(TileMap* _map)
{
	float tileSize = _map->GetTileSize();
	for (int row = 0; row < _map->GetNumMapTile().y; ++row)
	{
		for (int col = 0; col < _map->GetNumMapTile().x; ++col)
		{
			Tile::E_TILE_TYPE tileType = (*_map->GetMap()[row])[col]->GetType();
			SecurityCamera* _camera = NULL;
			switch (tileType)
			{
			case Tile::TILE_OBJ_CAMERA_ON_1_1:
				{
					_camera = new SecurityCamera(Vector2(col * tileSize, row * tileSize), Direction::DIRECTIONS[Direction::DIR_DOWN], true, _map->GetTileSize());
					_camera->SetMapPosition(_camera->GetMapPos(), _map->GetScrollOffset(), _map->GetTileSize());
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_ON_DOWN]);
					m_cameraList.push_back(_camera);
				}
				break;
			case Tile::TILE_OBJ_CAMERA_ON_1_2:
				{
					_camera = new SecurityCamera(Vector2(col * tileSize, row * tileSize), Direction::DIRECTIONS[Direction::DIR_LEFT], true, _map->GetTileSize());
					_camera->SetMapPosition(_camera->GetMapPos(), _map->GetScrollOffset(), _map->GetTileSize());
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_ON_LEFT]);
					m_cameraList.push_back(_camera);
				}
				break;
			case Tile::TILE_OBJ_CAMERA_ON_1_3:
				{
					_camera = new SecurityCamera(Vector2(col * tileSize, row * tileSize), Direction::DIRECTIONS[Direction::DIR_UP], true, _map->GetTileSize());
					_camera->SetMapPosition(_camera->GetMapPos(), _map->GetScrollOffset(), _map->GetTileSize());
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_ON_UP]);
					m_cameraList.push_back(_camera);
				}
				break;
			case Tile::TILE_OBJ_CAMERA_ON_1_4:
				{
					_camera = new SecurityCamera(Vector2(col * tileSize, row * tileSize), Direction::DIRECTIONS[Direction::DIR_RIGHT], true, _map->GetTileSize());
					_camera->SetMapPosition(_camera->GetMapPos(), _map->GetScrollOffset(), _map->GetTileSize());
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_ON_RIGHT]);
					m_cameraList.push_back(_camera);
				}
				break;
			case Tile::TILE_OBJ_CAMERA_OFF_1_1:
				{
					_camera = new SecurityCamera(Vector2(col * tileSize, row * tileSize), Direction::DIRECTIONS[Direction::DIR_DOWN], false, _map->GetTileSize());
					_camera->SetMapPosition(_camera->GetMapPos(), _map->GetScrollOffset(), _map->GetTileSize());
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_DOWN]);
					m_cameraList.push_back(_camera);
				}
				break;
			case Tile::TILE_OBJ_CAMERA_OFF_1_2:
				{
					_camera = new SecurityCamera(Vector2(col * tileSize, row * tileSize), Direction::DIRECTIONS[Direction::DIR_LEFT], false, _map->GetTileSize());
					_camera->SetMapPosition(_camera->GetMapPos(), _map->GetScrollOffset(), _map->GetTileSize());
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_LEFT]);
					m_cameraList.push_back(_camera);
				}
				break;
			case Tile::TILE_OBJ_CAMERA_OFF_1_3:
				{
					_camera = new SecurityCamera(Vector2(col * tileSize, row * tileSize), Direction::DIRECTIONS[Direction::DIR_UP], false, _map->GetTileSize());
					_camera->SetMapPosition(_camera->GetMapPos(), _map->GetScrollOffset(), _map->GetTileSize());
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_UP]);
					m_cameraList.push_back(_camera);
				}
				break;
			case Tile::TILE_OBJ_CAMERA_OFF_1_4:
				{
					_camera = new SecurityCamera(Vector2(col * tileSize, row * tileSize), Direction::DIRECTIONS[Direction::DIR_RIGHT], false, _map->GetTileSize());
					_camera->SetMapPosition(_camera->GetMapPos(), _map->GetScrollOffset(), _map->GetTileSize());
					_camera->SetMesh(SecurityCamera::s_camMeshList[SecurityCamera::CAM_OFF_RIGHT]);
					m_cameraList.push_back(_camera);
				}
				break;
			}
		}
	}
}
