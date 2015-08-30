#ifndef UIBUTTON_H
#define UIBUTTON_H

#include "GameObject2D.h"

class UIButton : GameObject2D
{
	public:
		enum E_BUTTON_STATE_TYPE
		{
			UP_STATE,
			DOWN_STATE,
			HOVER_STATE,
			NUM_BUTTON_STATES
		};
		enum E_BUTTON_TYPE
		{
			BUTTON_NONE = 0,
			BUTTON_START,
			BUTTON_INSTRUCTIONS,
			NUM_BUTTONS,
		};

	private:
		E_BUTTON_STATE_TYPE m_state;
		E_BUTTON_TYPE m_type;

	public:
		UIButton();
		~UIButton();
	
		void Init(Mesh* _mesh, Vector2 pos, Vector2 scale);
		void UpdateState(bool pressed, float mouseX, float mouseY);

		/*
		 * Getters
		 */
		E_BUTTON_STATE_TYPE GetState(void);
		E_BUTTON_TYPE GetType(void);
};

#endif