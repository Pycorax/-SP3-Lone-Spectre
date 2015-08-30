#include "UIButton.h"

UIButton::UIButton(E_BUTTON_TYPE type, Mesh* _mesh, Vector2 pos, Vector2 scale) : m_type(type), m_state(UP_STATE)
{
	m__mesh = _mesh;
	m_transforms.Translation = pos;
	m_transforms.Scale = scale;
}


UIButton::~UIButton()
{
}

void UIButton::Init(E_BUTTON_TYPE type, Mesh * _mesh, Vector2 pos, Vector2 scale)
{
	m__mesh = _mesh;
	m_transforms.Translation = pos;
	m_transforms.Scale = scale;
	m_state = UP_STATE;
	m_type = type;
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

void UIButton::SetState(E_BUTTON_STATE_TYPE type)
{
	this->m_state = type;
}

UIButton::E_BUTTON_STATE_TYPE UIButton::GetState(void)
{
	return m_state;
}


UIButton::E_BUTTON_TYPE UIButton::GetType(void)
{
	return m_type;
}