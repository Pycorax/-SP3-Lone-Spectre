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

	virtual void Update(double dt, int mouseX, int mouseY, bool clickState) = 0;
	void Clear();

	// Addition of button or screen
	bool AddButton(Menu::E_MENU_TYPE menuType, UIButton* _button); // Creation on the outside
	bool AddMenu(Menu* _menu); // Creation on the outside

	// Assign current menu and button for init only
	void AssignCurrent(Menu::E_MENU_TYPE menuType, UIButton::E_BUTTON_TYPE buttonType);

	/*
	 * Getters
	 */
	vector<Menu*>& GetMenuList();
	Menu* GetCurrentMenu();
	UIButton* GetCurrentButton();
protected:
	bool OnMouseCollision(int mouseX, int mouseY, UIButton* _button);
};

#endif