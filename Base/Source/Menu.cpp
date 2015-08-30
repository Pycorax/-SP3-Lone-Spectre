#include "Menu.h"

Menu::Menu(void) : m_buttonList(NULL)
{
}

Menu::~Menu(void)
{
	Clear();
}

void Menu::Clear()
{
	while (m_buttonList.size() > 0)
	{
		UIButton* _button = m_buttonList.back();
		if (_button)
		{
			delete _button;
			_button = NULL;
		}
		m_buttonList.pop_back();
	}
}

bool Menu::AddButton(UIButton* _button)
{
	m_buttonList.push_back(_button);
	return true;
}

vector<UIButton*>& Menu::GetButtonList()
{
	return m_buttonList;
}

Menu::E_MENU_TYPE Menu::GetType()
{
	return m_type;
}