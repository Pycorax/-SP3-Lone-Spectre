#include "MenuManager_Spectre.h"


MenuManager_Spectre::MenuManager_Spectre(void)
{
}


MenuManager_Spectre::~MenuManager_Spectre(void)
{
}

void MenuManager_Spectre::Update(double dt, int mouseX, int mouseY, bool clickState)
{
	vector<UIButton*> buttonList = m__currentMenu->GetButtonList();
	// Update button states and current button
	for (vector<UIButton*>::iterator buttonIter = buttonList.begin(); buttonIter != buttonList.end(); ++buttonIter)
	{
		UIButton* _button = *buttonIter;
		if (OnMouseCollision(mouseX, mouseY, _button))
		{
			_button->SetState(UIButton::HOVER_STATE);
			m__currentButton = _button;
		}
		else
		{
			_button->SetState(UIButton::UP_STATE);
		}
	}

	if (clickState) // Mouse clicked
	{
		switch (m__currentButton->GetType())
		{
			// Insert cases for each button states
		}
	}
}