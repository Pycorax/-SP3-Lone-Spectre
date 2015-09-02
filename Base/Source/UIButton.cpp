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
		// Return to main menu
		if (_mesh->name == "BUTTON_RETURN_TO_MAIN_MENU_ON")
		{
			s_m_onMeshList[BUTTON_RETURN_TO_MAIN_MENU] = _mesh;
		}
		else if (_mesh->name == "BUTTON_RETURN_TO_MAIN_MENU_OFF")
		{
			s_m_offMeshList[BUTTON_RETURN_TO_MAIN_MENU] = _mesh;
		}
		// New game
		else if (_mesh->name == "BUTTON_NEW_GAME_ON")
		{
			s_m_onMeshList[BUTTON_NEW_GAME] = _mesh;
		}
		else if (_mesh->name == "BUTTON_NEW_GAME_OFF")
		{
			s_m_offMeshList[BUTTON_NEW_GAME] = _mesh;
		}
		// Continue
		else if (_mesh->name == "BUTTON_CONTINUE_ON")
		{
			s_m_onMeshList[BUTTON_CONTINUE] = _mesh;
		}
		else if (_mesh->name == "BUTTON_CONTINUE_OFF")
		{
			s_m_offMeshList[BUTTON_CONTINUE] = _mesh;
		}
		// Instructions
		else if (_mesh->name == "BUTTON_INSTRUCTIONS_ON")
		{
			s_m_onMeshList[BUTTON_INSTRUCTIONS] = _mesh;
		}
		else if (_mesh->name == "BUTTON_INSTRUCTIONS_OFF")
		{
			s_m_offMeshList[BUTTON_INSTRUCTIONS] = _mesh;
		}
		// Credits
		else if (_mesh->name == "BUTTON_CREDITS_ON")
		{
			s_m_onMeshList[BUTTON_CREDITS] = _mesh;
		}
		else if (_mesh->name == "BUTTON_CREDITS_OFF")
		{
			s_m_offMeshList[BUTTON_CREDITS] = _mesh;
		}
		// Exit
		else if (_mesh->name == "BUTTON_EXIT_ON")
		{
			s_m_onMeshList[BUTTON_EXIT] = _mesh;
		}
		else if (_mesh->name == "BUTTON_EXIT_OFF")
		{
			s_m_offMeshList[BUTTON_EXIT] = _mesh;
		}
		// Retry
		else if (_mesh->name == "BUTTON_RETRY_ON")
		{
			s_m_onMeshList[BUTTON_RETRY] = _mesh;
		}
		else if (_mesh->name == "BUTTON_RETRY_OFF")
		{
			s_m_offMeshList[BUTTON_RETRY] = _mesh;
		}
		// Next stage
		else if (_mesh->name == "BUTTON_NEXT_STAGE_ON")
		{
			s_m_onMeshList[BUTTON_NEXT_STAGE] = _mesh;
		}
		else if (_mesh->name == "BUTTON_NEXT_STAGE_OFF")
		{
			s_m_offMeshList[BUTTON_NEXT_STAGE] = _mesh;
		}
		// Start level
		else if (_mesh->name == "BUTTON_START_ON")
		{
			s_m_onMeshList[BUTTON_START] = _mesh;
		}
		else if (_mesh->name == "BUTTON_START_OFF")
		{
			s_m_offMeshList[BUTTON_START] = _mesh;
		}
		// Resume level
		else if (_mesh->name == "BUTTON_RESUME_ON")
		{
			s_m_onMeshList[BUTTON_RESUME] = _mesh;
		}
		else if (_mesh->name == "BUTTON_RESUME_OFF")
		{
			s_m_offMeshList[BUTTON_RESUME] = _mesh;
		}
		// Level select
		else if (_mesh->name == "BUTTON_LEVEL_SELECT_ON")
		{
			s_m_onMeshList[BUTTON_LEVEL_SELECT] = _mesh;
		}
		else if (_mesh->name == "BUTTON_LEVEL_SELECT_OFF")
		{
			s_m_offMeshList[BUTTON_LEVEL_SELECT] = _mesh;
		}
		// Level
		else if (_mesh->name == "BUTTON_LEVEL_ON")
		{
			s_m_onMeshList[BUTTON_LEVEL] = _mesh;
		}
		else if (_mesh->name == "BUTTON_LEVEL_OFF")
		{
			s_m_offMeshList[BUTTON_LEVEL] = _mesh;
		}
	}
}

UIButton::UIButton(E_BUTTON_TYPE type, Mesh* _mesh, Vector2 pos, Vector2 scale) : m_type(type), m_state(UP_STATE), m__text(NULL), m_levelID(-1)
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

void UIButton::InitLevel(int levelID, Mesh* _mesh, Vector2 pos, Vector2 scale, string text)
{
	if (m__text == NULL)
	{
		m__text = new TextObject(_mesh, text, Color(1,1,1));
	}
	m_levelID = levelID;
	m__text->SetPos(pos);
	m__text->SetScale(scale);
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

TextObject* UIButton::GetTextObj()
{
	return m__text;
}

int UIButton::GetLevelID()
{
	return m_levelID;
}

void UIButton::Resize(Vector2 pos, Vector2 scale)
{
	m_transforms.Translation = pos;
	m_transforms.Scale = scale;
}

void UIButton::ResizeText(Vector2 textPos, Vector2 textScale)
{
	if (m__text)
	{
		m__text->SetPos(textPos);
		m__text->SetScale(textScale);
	}
}