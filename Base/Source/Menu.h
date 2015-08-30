#ifndef MENU_H
#define MENU_H

#include <vector>
#include "UIButton.h"

using std::vector;

class Menu 
{
public:
	/*enum E_MENU_STATE
	{
		//main menu states
		MENU_NULL = 0,
		MENU_MAIN_NONE,
		MENU_MAIN_START,
		MENU_MAIN_SELECTLEVEL,
		MENU_MAIN_INSTRUCTIONS,
		MENU_MAIN_OPTIONS,
		MENU_MAIN_EXIT,

		//pause menu states
		MENU_PAUSE,
		MENU_PAUSE_RESUME,
		MENU_PAUSE_OPTIONS,
		MENU_PAUSE_RESTART,
		MENU_PAUSE_BACK,
		
		//options states
		MENU_OPTIONS,
		MENU_OPTIONS_SOUND,
		MENU_OPTIONS_BGM,
		MENU_OPTIONS_LOAD_PROFILE_HIGHLIGHT,
		MENU_OPTIONS_BACK,

		IN_GAME,

		//loads previous available level
		MENU_SELECTLEVEL_TUTORIAL,
		MENU_SELECTLEVEL_LV1,
		MENU_SELECTLEVEL_LV2,

		//complete level
		MENU_WIN_SCREEN,
		MENU_WIN_SCREEN_BACK,

		//lose level
		MENU_LOSE_SCREEN,
		MENU_LOSE_SCREEN_BACK,

		NUM_MENU,
	};*/
	enum E_MENU_TYPE
	{
		MENU_NONE = 0,
		MENU_MAIN,
		MENU_LEVEL_SELECT,
		MENU_OPTIONS,
		MENU_HIGHSCORE,
		MENU_EXIT,
		NUM_MENU,
	};

protected:	// Variables
	vector<UIButton*> m_buttonList;
	E_MENU_TYPE m_type;
	GameObject2D* m__background;

public:		// Functions
	Menu();
	virtual ~Menu();

	virtual void Init(E_MENU_TYPE type, Mesh* _bgMesh = NULL, Vector2 bgPos = Vector2::ZERO_VECTOR, Vector2 bgScale = Vector2(1,1));
	virtual void Update(double dt);
	void Clear();

	bool AddButton(UIButton* _button);

	/*
	 * Getters
	 */
	vector<UIButton*>& GetButtonList();
	E_MENU_TYPE GetType();
	GameObject2D* GetBackground();
};

#endif