#include "MenuManager.h"


MenuManager::MenuManager(void)
{
}


MenuManager::~MenuManager(void)
{
}

void MenuManager::Clear()
{
	while (m_menuList.size() > 0)
	{
		Menu* _menu = m_menuList.back();
		if (_menu)
		{
			_menu->Clear();
			delete _menu;
			_menu = NULL;
		}
		m_menuList.pop_back();
	}
}

bool MenuManager::AddButton(Menu::E_MENU_TYPE menuType, UIButton* _button)
{
	for (vector<Menu*>::iterator menuIter = m_menuList.begin(); menuIter != m_menuList.end(); ++menuIter)
	{
		Menu* _menu = *menuIter;
		if (_menu->GetType() == menuType)
		{
			_menu->AddButton(_button);
			return true;
		}
	}
	return false;
}

bool MenuManager::AddMenu(Menu* _menu)
{
	m_menuList.push_back(_menu);
	return true;
}

vector<Menu*>& MenuManager::GetMenuList()
{
	return m_menuList;
}

Menu* MenuManager::GetCurrentMenu()
{
	return m__currentMenu;
}

UIButton* MenuManager::GetCurrentButton()
{
	return m__currentButton;
}