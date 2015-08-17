#ifndef UIBUTTON_H
#define UIBUTTON_H

#include "Mesh.h"
#include "Vector3.h"

class UIButton
{
	public:
		enum BUTTON_STATE_TYPE
		{
			UP_STATE,
			DOWN_STATE,
			HOVER_STATE,
			NUM_BUTTON_STATES
		};

	private:
		Mesh* m_mesh;
		Vector3 m_pos;
		Vector3 m_scale;
		BUTTON_STATE_TYPE m_state;

	public:
		UIButton();
		~UIButton();
	
		void Init(Mesh* mesh, Vector3 pos, Vector3 scale);
		void UpdateState(bool pressed, float mouseX, float mouseY);

		void SetMesh(Mesh* mesh);

		Mesh* GetMesh(void);
		Vector3 GetPosition(void);
		BUTTON_STATE_TYPE GetState(void);
};

#endif