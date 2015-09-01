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
			setNoCurrent();
			return RS_NEW_GAME;
		}
		break;
	case UIButton::BUTTON_INSTRUCTIONS:
		{
			AssignCurrent(Menu::MENU_INSTRUCTIONS);
			return RS_MENU;
		}
		break;
	case UIButton::BUTTON_CREDITS:
		{
			AssignCurrent(Menu::MENU_CREDITS);
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
	case UIButton::BUTTON_RETRY:
		{
			setNoCurrent();
			return RS_CURRENT_LEVEL;
		}
		break;
	case UIButton::BUTTON_NEXT_STAGE:
		{
			setNoCurrent();
			return RS_NEXT_LEVEL;
		}
		break;
	case UIButton::BUTTON_RESUME:
		{
			setNoCurrent();
			return RS_RESUME;
		}
		break;
	case UIButton::BUTTON_LEVEL_SELECT:
		{
			AssignCurrent(Menu::MENU_LEVEL_SELECT);
			return RS_MENU;
		}
		break;
	case UIButton::BUTTON_LEVEL:
		{
			//setNoCurrent();
			return RS_LEVEL;
		}
		break;
	}
}