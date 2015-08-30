#include "UIButton.h"

UIButton::UIButton() : m_state(UP_STATE)
{
}


UIButton::~UIButton()
{
}

void UIButton::Init(Mesh * _mesh, Vector2 pos, Vector2 scale)
{
	m__mesh = _mesh;
	m_transforms.Translation = pos;
	m_transforms.Scale = scale;
	m_state = UP_STATE;
}

void UIButton::UpdateState(bool pressed, float mouseX, float mouseY)
{
	// Checking if cursor is on the button
	/*if (
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
	}*/
}

UIButton::E_BUTTON_STATE_TYPE UIButton::GetState(void)
{
	return m_state;
}


UIButton::E_BUTTON_TYPE UIButton::GetType(void)
{
	return m_type;
}