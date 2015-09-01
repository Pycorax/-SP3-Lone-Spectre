#include "HUD_Objectives.h"


HUD_Objectives::HUD_Objectives(void) : m__displayCover(NULL)
{
}


HUD_Objectives::~HUD_Objectives(void)
{
	Clear();
}

void HUD_Objectives::Init(Mesh* _displayMesh, Vector2 displayPos, Vector2 displayScale, Mesh* _coverMesh, Vector2 coverPos, Vector2 coverScale, bool scrollX, bool scrollY)
{
	HUD::Init(_displayMesh, displayPos, displayScale);
	if (!m__displayCover)
	{
		m__displayCover = new GameObject2D();
	}
	m__displayCover->SetMesh(_coverMesh);
	m__displayCover->SetScale(coverScale);
	m__displayCover->SetPos(coverPos);
	m_initPos = coverPos;
	m_initScale = coverScale;
	m_scrollX = scrollX;
	m_scrollY = scrollY;
}

void HUD_Objectives::Update(double dt, float coverLevel, float coverLevelLimit)
{
	const Vector2 S_VALUE_PER_UNIT = m_initScale * (1.f / coverLevelLimit);
	// Setting scale based on level
	if (m_scrollX)
	{
		m__displayCover->SetScale(Vector2(m_initScale.x - (S_VALUE_PER_UNIT.x * coverLevel), m__displayCover->GetTransform().Scale.y));
	}
	if (m_scrollY)
	{
		m__displayCover->SetScale(Vector2(m__displayCover->GetTransform().Scale.x, m_initScale.y - (S_VALUE_PER_UNIT.y * coverLevel)));
	}
	// Arranging cover position (Up or Right)
	m__displayCover->SetPos((m_initPos + m_initScale) - m__displayCover->GetTransform().Scale); // Using screen size and scale to find new position
}

void HUD_Objectives::Clear()
{
	if (m__displayCover)
	{
		delete m__displayCover;
		m__displayCover = NULL;
	}
	HUD::Clear();
}

void HUD_Objectives::SetDisplayCover(GameObject2D* _displayCover)
{
	m__displayCover = _displayCover;
}

GameObject2D* HUD_Objectives::GetDisplayCover()
{
	return m__displayCover;
}