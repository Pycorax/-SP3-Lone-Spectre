#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "Menu.h"

class MenuManager
{
public:
	enum E_RETURN_STATE
	{
		RS_MENU = 0,
		RS_NEW_GAME,
		RS_EXIT,
		RS_CURRENT_LEVEL,
		RS_NEXT_LEVEL,
		RS_RESUME,
		RS_LEVEL,
		NUM_RS,
	};

protected:
	vector<Menu*> m_menuList;
	Menu* m__currentMenu;
	UIButton* m__currentButton;
	int m_currentButton; // For controller

public:
	MenuManager(void);
	~MenuManager(void);

	void MouseUpdate(double dt, int mouseX, int mouseY);	// Update every frame
	void KeysUpdate(double dt, bool dir);					// Update on input (Dir false = Up | Dir true = Down
	void Clear();

	// Addition of button or screen
	bool AddButton(Menu::E_MENU_TYPE menuType, UIButton* _button); // Creation on the outside
	bool AddMenu(Menu* _menu); // Creation on the outside

	// Assign current menu and button for init only
	void AssignCurrent(Menu::E_MENU_TYPE menuType, UIButton::E_BUTTON_TYPE buttonType = static_cast<UIButton::E_BUTTON_TYPE>(-1));

	// Response on click or enter
	E_RETURN_STATE OnClick(int mouseX, int mouseY); // Mouse
	E_RETURN_STATE OnEnter(); // Controller
	virtual E_RETURN_STATE Response(UIButton::E_BUTTON_TYPE type) = 0;

	/*
	 * Getters
	 */
	vector<Menu*>& GetMenuList();
	Menu* GetCurrentMenu();
	UIButton* GetCurrentButton();
protected:
	bool onMouseCollision(int mouseX, int mouseY, UIButton* _button);
	void setNoCurrent();
};

#endif