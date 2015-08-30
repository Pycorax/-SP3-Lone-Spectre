#include "UIButton.h"

Mesh* UIButton::s_m_offMeshList[NUM_BUTTONS];
Mesh* UIButton::s_m_onMeshList[NUM_BUTTONS];

void UIButton::InitMeshLists(vector<Mesh*> meshList)
{
	for (int mesh = 0; mesh < NUM_BUTTONS; ++mesh) // Set everything to NULL
	{
		s_m_offMeshList[mesh] = s_m_onMeshList[mesh] = NULL;
	}
	for (vector<Mesh*>::iterator meshIter = meshList.begin(); meshIter != meshList.end(); ++meshIter)
	{
		Mesh* _mesh = *meshIter;
		if (_mesh->name == "BUTTON_START_ON")
		{
			s_m_onMeshList[BUTTON_START] = _mesh;
		}
		else if (_mesh->name == "BUTTON_START_OFF")
		{
			s_m_offMeshList[BUTTON_START] = _mesh;
		}
		else if (_mesh->name == "BUTTON_INSTRUCTIONS_ON")
		{
			s_m_onMeshList[BUTTON_INSTRUCTIONS] = _mesh;
		}
		else if (_mesh->name == "BUTTON_INSTRUCTIONS_OFF")
		{
			s_m_offMeshList[BUTTON_INSTRUCTIONS] = _mesh;
		}
		else if (_mesh->name == "BUTTON_EXIT_ON")
		{
			s_m_onMeshList[BUTTON_EXIT] = _mesh;
		}
		else if (_mesh->name == "BUTTON_EXIT_OFF")
		{
			s_m_offMeshList[BUTTON_EXIT] = _mesh;
		}
	}
}

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
	if (m_state != type) // If not similar state
	{
		this->m_state = type;
		if (type == HOVER_STATE) // Set new mesh
		{
			m__mesh = s_m_onMeshList[m_type];
		}
		else
		{
			m__mesh = s_m_offMeshList[m_type];
		}
	}
}

UIButton::E_BUTTON_STATE_TYPE UIButton::GetState(void)
{
	return m_state;
}


UIButton::E_BUTTON_TYPE UIButton::GetType(void)
{
	return m_type;
}