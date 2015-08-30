#include "MenuManager.h"

MenuManager::MenuManager(void) : m_menuList(NULL), m__currentButton(NULL), m__currentMenu(NULL)
{
}


MenuManager::~MenuManager(void)
{
	Clear();
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

void MenuManager::AssignCurrent(Menu::E_MENU_TYPE menuType, UIButton::E_BUTTON_TYPE buttonType)
{
	for (vector<Menu*>::iterator menuIter = m_menuList.begin(); menuIter != m_menuList.end(); ++menuIter)
	{
		Menu* _menu = *menuIter;
		if (_menu && _menu->GetType() == menuType) // Found menu, find button
		{
			m__currentMenu = _menu;
			vector<UIButton*> buttonList= _menu->GetButtonList();
			for (vector<UIButton*>::iterator buttonIter = buttonList.begin(); buttonIter != buttonList.end(); ++buttonIter)
			{
				UIButton* _button = *buttonIter;
				if (_button && _button->GetType() == buttonType) // Found button
				{
					m__currentButton = _button;
					return;
				}
			}
		}
	}
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

bool MenuManager::OnMouseCollision(int mouseX, int mouseY, UIButton* _button)
{
	const Transform transform = _button->GetTransform();
	const Vector2 buttonMaxBound(transform.Translation + (transform.Scale * 0.5f));
	const Vector2 buttonMinBound(transform.Translation - (transform.Scale * 0.5f));
	
	if (mouseX < buttonMinBound.x || mouseX > buttonMaxBound.x || mouseY < buttonMinBound.y || mouseY > buttonMinBound.y)
	{
		return false;
	}
	return true;
}