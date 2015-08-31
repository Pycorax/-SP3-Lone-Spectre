#include "MenuManager_Spectre.h"


MenuManager_Spectre::MenuManager_Spectre(void)
{
}


MenuManager_Spectre::~MenuManager_Spectre(void)
{
}

MenuManager::E_RETURN_STATE MenuManager_Spectre::Response(UIButton::E_BUTTON_TYPE type)
{
	switch (type)
	{
	case UIButton::BUTTON_NEW_GAME: // Set current to nothing as game is not menu
		{
			m__currentMenu = NULL;
			m__currentButton = NULL;
			m_currentButton = -1;
			return RS_GAME;
		}
		break;
	case UIButton::BUTTON_INSTRUCTIONS:
		{
			AssignCurrent(Menu::MENU_INSTRUCTIONS);
			return RS_MENU;
		}
		break;
	case UIButton::BUTTON_EXIT:
		{
			return RS_EXIT;
		}
		break;
	case UIButton::BUTTON_RETURN_TO_MAIN_MENU:
		{
			AssignCurrent(Menu::MENU_MAIN);
			return RS_MENU;
		}
		break;
	}
}