#ifndef UIBUTTON_H
#define UIBUTTON_H

#include "GameObject2D.h"
#include <vector>

using std::vector;

class UIButton : public GameObject2D
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
			BUTTON_EXIT,
			NUM_BUTTONS,
		};
		static Mesh* s_m_offMeshList[NUM_BUTTONS];	// Not hover
		static Mesh* s_m_onMeshList[NUM_BUTTONS];		// Hover

	private:
		E_BUTTON_STATE_TYPE m_state;
		E_BUTTON_TYPE m_type;

	public:
		UIButton(E_BUTTON_TYPE type = BUTTON_NONE, Mesh* _mesh = NULL, Vector2 pos = Vector2::ZERO_VECTOR, Vector2 scale = Vector2(1,1));
		~UIButton();

		static void InitMeshLists(vector<Mesh*> meshList);
	
		void Init(E_BUTTON_TYPE type, Mesh* _mesh, Vector2 pos, Vector2 scale);
		void UpdateState(bool pressed, float mouseX, float mouseY);

		/*
		 * Setters
		 */
		void SetState(E_BUTTON_STATE_TYPE type);

		/*
		 * Getters
		 */
		E_BUTTON_STATE_TYPE GetState(void);
		E_BUTTON_TYPE GetType(void);
};

#endif