#include "UIButton.h"

UIButton::UIButton()
	: m_mesh(NULL)
	, m_state(UP_STATE)
{
}


UIButton::~UIButton()
{
}

void UIButton::Init(Mesh * mesh, Vector3 pos, Vector3 scale)
{
	m_mesh = mesh;
	m_pos = pos;
	m_state = UP_STATE;
	m_scale = scale;
}

void UIButton::UpdateState(bool pressed, float mouseX, float mouseY)
{
	// Checking if cursor is on the button
	if (
		mouseX >= m_pos.x && mouseX <= m_pos.x + m_scale.x
		&&
		mouseY >= m_pos.y && mouseY <= m_pos.y + m_scale.y
		)
	{
		if (pressed)	// If use clicked
		{
			m_state = DOWN_STATE;
		}
		else
		{
			m_state = HOVER_STATE;
		}
	}
	else	// Cursor not on mouse
	{
		m_state = UP_STATE;
	}
}

void UIButton::SetMesh(Mesh * mesh)
{
	m_mesh = mesh;
}

Mesh * UIButton::GetMesh(void)
{
	return m_mesh;
}

Vector3 UIButton::GetPosition(void)
{
	return m_pos;
}

UIButton::BUTTON_STATE_TYPE UIButton::GetState(void)
{
	return m_state;
}
