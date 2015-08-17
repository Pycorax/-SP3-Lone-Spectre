#ifndef MVC_MODEL_2D_MARIO_H
#define MVC_MODEL_2D_MARIO_H

#include "MVC_Model.h"
#include "MVC_MAddon_TileSys.h"

#include "GhostEnemy.h"
#include "TargetEnemy.h"
#include "ElementalBullet.h"

// Buttons
#include "UIButton.h"

// File IO
#include "SONIO.h"

struct Ghost
{
	GhostEnemy* ghost;
	GameObject* ghostRep;
	GameObject* hpBar;

	Ghost(GhostEnemy* _ghost = NULL, GameObject* _ghostRep = NULL, GameObject* _hpBar = NULL)
	{
		ghost = _ghost;
		ghostRep = _ghostRep;
		hpBar = _hpBar;
	}

	~Ghost()
	{
		if (ghost != NULL)
		{
			delete ghost;
			ghost = NULL;
		}
		if (ghostRep != NULL)
		{
			delete ghostRep;
			ghostRep = NULL;
		}
		if (hpBar != NULL)
		{
			delete hpBar;
			hpBar = NULL;
		}
	}
};

struct Target
{
	TargetEnemy* target;
	GameObject* targetRep;
	GameObject* hpBar;

	Target(TargetEnemy* _target = NULL, GameObject* _targetRep = NULL, GameObject* _hpBar = NULL)
	{
		target = _target;
		targetRep = _targetRep;
		hpBar = _hpBar;
	}

	~Target()
	{
		if (target != NULL)
		{
			delete target;
			target = NULL;
		}
		if (targetRep != NULL)
		{
			delete targetRep;
			targetRep = NULL;
		}
		if (hpBar != NULL)
		{
			delete hpBar;
			hpBar = NULL;
		}
	}
};

struct Button
{
	UIButton button;
	GameObject* buttonRep;

	Button(GameObject* _buttonRep = NULL)
	{
		buttonRep = _buttonRep;
	}

	~Button()
	{
		if (buttonRep != NULL)
		{
			delete buttonRep;
			buttonRep = NULL;
		}
	}
};

class MVC_Model_Mario : public MVC_Model, public MVC_MAddon_TileSys
{
	enum GAME_STATE
	{
		LEVEL_STATE,
		WIN_STATE,
		LOSE_STATE,
		NUM_STATES
	};

	enum BUTTON_TYPE
	{
		CONTINUE_BUTTON,
		NUM_BUTTON
	};

	#pragma region Static Constants
		
		// Ghosts
		static const int GHOST_HEALTH = 100;
		static const float GHOST_MOVE_SPEED;

		// Target
		static const int TARGET_HEALTH = 500;
		static const int TARGET_KILL_SCORE = 200;

		// Bullets
		static const float BULLET_LIFETIME;
		static const float CONTACT_DMG_MULTIPLIER;
		static const float VULNERABLE_DMG_MULTIPLIER;
		
		// Scale
		static const Vector3 PLAYER_BULLET_SCALE;
		static const Vector3 ENEMY_BULLET_SCALE;

	#pragma endregion

	// Application Lifecycle
	GAME_STATE m_gameState;

	// Level
	CharObject::CHAR_TYPE m_levelType;
	int m_levelMaxEnemies;
	int m_currentLevel;
	vector<string> m_levelPaths;

	// Enemy
	vector<Ghost*> m_ghosts;
	Mesh* m_enemyMesh[Elemental::NUM_ELEMENTS];

	// Hero
	GameObject* m_heroHPBar;

	// Hero's Weapons
	ElementalWeapon* m_heroWeapon[Elemental::NUM_ELEMENTS];

	// Targets
	vector<Target*> m_targets;

	// Reusable Meshes
	Mesh* m_bulletMesh[Elemental::NUM_ELEMENTS];
	Mesh* m_hpBarMesh;
	Mesh* m_targetMesh[Elemental::NUM_ELEMENTS];
	Mesh* m_heroMesh[CharObject::NUM_PLAYER][Elemental::NUM_ELEMENTS];

	// Button UI
	Button* m_buttons[NUM_BUTTON];

	public:
		MVC_Model_Mario(string configSONFile);
		virtual ~MVC_Model_Mario();

		virtual void Init(void);
		virtual void Update(double dt);
		virtual void Exit(void);

	private:
		// Inits
		// -- Inits for Different Game States
		void levelInit(void);
		// -- Buttons
		void initButtons(void);
		// -- Level System
		void clearLevel(void);
		void initLevel(string levelFile);
		void loadLevel(string levelFile);
		MapLayer* loadMapLayer(Branch mapLayer);
		// -- Others
		void initEnvironment(void);

		// Updates
		// -- Updates for Different Game States
		void levelUpdate(double dt);
		void winUpdate(double dt);
		void loseUpdate(double dt);
		// -- ProcessKeyAction Updates
		void levelKeyUpdate(double dt);
		void winKeyUpdate(void);
		void loseKeyUpdate(void);

		// -- Others
		void levelController(void);
		void enemyUpdate(double dt);
		void targetUpdate(double dt);
		void bulletUpdate(double dt);
		void updateHeroHealthBar(void);

		ElementalBullet* FetchElementalBullet();
		
		// Hero
		void heroAttack(void);
		void changeHeroWeaponType(Elemental::ELEMENT_TYPE element);

		// Enemies
		void spawnEnemy(void);
		Ghost* fetchEnemy(void);
		int countEnemies(void);

		// Targets
		void spawnTarget(Vector2 pos, float scale, int health, int rewardScore, Elemental::ELEMENT_TYPE element = Elemental::NORMAL_TYPE);
		Target* fetchTarget(void);
		int countTargets(void);

		// GameState System
		void switchState(GAME_STATE state);

		// Push to Renders
		void pushUIButtonToRender(Button* btn);
		void pushEnemiesToRender(void);
		void pushTargetsToRender(void);

		void processKeyAction(double dt);
};

#endif