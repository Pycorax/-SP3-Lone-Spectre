#ifndef MENU_MANAGER_SPECTRE_H
#define MENU_MANAGER_SPECTRE_H

#include "MenuManager.h"

class MenuManager_Spectre : public MenuManager
{
public:
	MenuManager_Spectre(void);
	virtual ~MenuManager_Spectre(void);

	virtual void Update(double dt);
};

#endif