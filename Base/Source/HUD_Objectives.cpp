#include "HUD_Objectives.h"

const string HUD_Objectives::S_M_OBJECTIVE_TEXTS[NUM_OBJECTIVE] = 
{
	"Escape!",
	"Find and assassinate the target!",
	"Find and collect the documents!",
	"Find and defuse the bomb!",
	"Find and resuce the hostage!",
	"Find the bomb site and plant the bomb"
};

HUD_Objectives::HUD_Objectives(void) : m__objectiveText(NULL)
{
}


HUD_Objectives::~HUD_Objectives(void)
{
	Clear();
}

void HUD_Objectives::Init(Mesh* _displayMesh, Vector2 displayPos, Vector2 displayScale, Mesh* _objMesh, string text, Vector2 objPos, Vector2 objScale)
{
	HUD::Init(_displayMesh, displayPos, displayScale);
	if (m__objectiveText == NULL)
	{
		m__objectiveText = new TextObject(_objMesh, text, Color(1,0,0));
	}
	m__objectiveText->SetPos(objPos);
	m__objectiveText->SetScale(objScale);
}

void HUD_Objectives::Update(double dt, Objective* _obj)
{
	if (_obj)
	{
		ObjectiveAssassinate* _objAss = dynamic_cast<ObjectiveAssassinate*>(_obj);
		ObjectiveCollect* _objCol = dynamic_cast<ObjectiveCollect*>(_obj);
		ObjectiveDefuse* _objDef = dynamic_cast<ObjectiveDefuse*>(_obj);
		ObjectiveHostage* _objHos = dynamic_cast<ObjectiveHostage*>(_obj);
		ObjectiveSetBomb* _objSet = dynamic_cast<ObjectiveSetBomb*>(_obj);
		ObjectiveEscape* _objEsc = dynamic_cast<ObjectiveEscape*>(_obj);
		if (_objAss)
		{
			if (_objAss->IsCompleted())
			{
				m__objectiveText->SetText(S_M_OBJECTIVE_TEXTS[OBJECTIVE_ESCAPE]);
			}
			else
			{
				m__objectiveText->SetText(S_M_OBJECTIVE_TEXTS[OBJECTIVE_ASSASSINATE]);
			}
		}
		else if (_objCol)
		{
			if (_objCol->IsCompleted())
			{
				m__objectiveText->SetText(S_M_OBJECTIVE_TEXTS[OBJECTIVE_ESCAPE]);
			}
			else
			{
				m__objectiveText->SetText(S_M_OBJECTIVE_TEXTS[OBJECTIVE_COLLECT]);
			}
		}
		else if (_objDef)
		{
			if (_objDef->IsCompleted())
			{
				m__objectiveText->SetText(S_M_OBJECTIVE_TEXTS[OBJECTIVE_ESCAPE]);
			}
			else
			{
				m__objectiveText->SetText(S_M_OBJECTIVE_TEXTS[OBJECTIVE_DEFUSE]);
			}
		}
		else if (_objHos)
		{
			if (_objHos->IsCompleted())
			{
				m__objectiveText->SetText(S_M_OBJECTIVE_TEXTS[OBJECTIVE_ESCAPE]);
			}
			else
			{
				m__objectiveText->SetText(S_M_OBJECTIVE_TEXTS[OBJECTIVE_HOSTAGE]);
			}
		}
		else if (_objSet)
		{
			if (_objSet->IsCompleted())
			{
				m__objectiveText->SetText(S_M_OBJECTIVE_TEXTS[OBJECTIVE_ESCAPE]);
			}
			else
			{
				m__objectiveText->SetText(S_M_OBJECTIVE_TEXTS[OBJECTIVE_SET_BOMB]);
			}
		}
		else if (_objEsc)
		{
			m__objectiveText->SetText(S_M_OBJECTIVE_TEXTS[OBJECTIVE_ESCAPE]);
		}
	}
}

void HUD_Objectives::Clear()
{
	if (m__objectiveText)
	{
		delete m__objectiveText;
		m__objectiveText = NULL;
	}
	HUD::Clear();
}

void HUD_Objectives::SetObjectiveText(TextObject* _objectiveText)
{
	m__objectiveText = _objectiveText;
}

GameObject2D* HUD_Objectives::GetObjectiveText()
{
	return m__objectiveText;
}

void HUD_Objectives::NumberToText(float num)
{
	m__objectiveText->SetText(std::to_string(static_cast<long long>(ceil(num))));
}