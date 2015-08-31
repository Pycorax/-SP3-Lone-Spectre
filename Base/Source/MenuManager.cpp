#include "MenuManager.h"

MenuManager::MenuManager(void) : m_menuList(NULL), m__currentButton(NULL), m__currentMenu(NULL), m_currentButton(-1)
{
}


MenuManager::~MenuManager(void)
{
	Clear();
}

void MenuManager::MouseUpdate(double dt, int mouseX, int mouseY)
{
	vector<UIButton*> buttonList = m__currentMenu->GetButtonList();
	// Update button states and current button
	int buttonPlacement = 0;
	for (vector<UIButton*>::iterator buttonIter = buttonList.begin(); buttonIter != buttonList.end(); ++buttonIter)
	{
		UIButton* _button = *buttonIter;
		if (onMouseCollision(mouseX, mouseY, _button))
		{
			_button->SetState(UIButton::HOVER_STATE);
			m__currentButton = _button;
			m_currentButton = buttonPlacement;
		}
		else
		{
			if (_button != m__currentMenu->GetButtonList()[m_currentButton])
			{
				_button->SetState(UIButton::UP_STATE);
			}
		}
		++buttonPlacement;
	}
}

void MenuManager::KeysUpdate(double dt, bool dir)
{
	if (dir) // Down key pressed
	{
		--m_currentButton;
		if (m_currentButton < 0)
		{
			m_currentButton = m__currentMenu->GetButtonList().size() - 1;
		}
	}
	else // Up key pressed
	{
		++m_currentButton;
		if (m_currentButton >= m__currentMenu->GetButtonList().size())
		{
			m_currentButton = 0;
		}
	}
	m__currentButton = m__currentMenu->GetButtonList()[m_currentButton]; // Assign current button pointer to new current button
	m__currentButton->SetState(UIButton::HOVER_STATE);
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
			if (buttonType != -1)
			{
				int buttonPlacement = 0;
				for (vector<UIButton*>::iterator buttonIter = buttonList.begin(); buttonIter != buttonList.end(); ++buttonIter)
				{
					UIButton* _button = *buttonIter;
					if (_button && _button->GetType() == buttonType) // Found button
					{
						m__currentButton = _button;
						m_currentButton = buttonPlacement;
						m__currentButton->SetState(UIButton::HOVER_STATE);
						return;
					}
					++buttonPlacement;
				}
			}
			else
			{
				if (buttonList.size() > 0)
				{
					m__currentButton = buttonList.front();
					m_currentButton = 0;
					m__currentButton->SetState(UIButton::HOVER_STATE);
				}
				else
				{
					m__currentButton = NULL;
					m_currentButton = -1;
				}
			}
		}
	}
}

MenuManager::E_RETURN_STATE MenuManager::OnClick(int mouseX, int mouseY)
{
	if (onMouseCollision(mouseX, mouseY, m__currentButton))
	{
		return Response(m__currentButton->GetType());
	}
}

MenuManager::E_RETURN_STATE MenuManager::OnEnter()
{
	return Response(m__currentButton->GetType());
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

bool MenuManager::onMouseCollision(int mouseX, int mouseY, UIButton* _button)
{
	const Transform transform = _button->GetTransform();
	const Vector2 buttonMaxBound(transform.Translation + (transform.Scale * 1.f));
	const Vector2 buttonMinBound(transform.Translation - (transform.Scale * 0.f));
	
	if (mouseX < buttonMinBound.x || mouseX > buttonMaxBound.x || mouseY < buttonMinBound.y || mouseY > buttonMaxBound.y)
	{
		return false;
	}
	return true;
}

void MenuManager::setNoCurrent()
{
	m__currentMenu = NULL;
	m__currentButton = NULL;
	m_currentButton = -1;
}