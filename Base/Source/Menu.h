#ifndef MENU_H
#define MENU_H

#include "UIButton.h"
#include "MVC_Model.h"

class Menu 
{
private:	//Game States
	enum E_MENU_STATE
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
	};

private:	// Variables

public:		// Functions
	Menu();
	~Menu();

	void Init();
	void Update();

	UIButton* m_UIButton;

protected:
	E_MENU_STATE m_menuState;
};

#endif