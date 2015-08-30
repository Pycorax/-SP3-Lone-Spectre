#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "Menu.h"

class MenuManager
{
protected:
	vector<Menu*> m_menuList;
	Menu* m__currentMenu;
	UIButton* m__currentButton;

public:
	MenuManager(void);
	~MenuManager(void);

	virtual void Update(double dt) = 0;
	void Clear();

	bool AddButton(Menu::E_MENU_TYPE menuType, UIButton* _button); // Creation on the outside
	bool AddMenu(Menu* _menu); // Creation on the outside

	/*
	 * Getters
	 */
	vector<Menu*>& GetMenuList();
	Menu* GetCurrentMenu();
	UIButton* GetCurrentButton();
};

#endif