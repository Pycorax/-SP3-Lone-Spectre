#ifndef HUD_H
#define HUD_H

#include "HUD.h"

class HUD_Cooldown : public HUD
{
protected:
	GameObject2D* m__displayCover;
	bool m_scrollX;
	bool m_scrollY;
	Vector2 m_initPos;
	Vector2 m_initScale;
public:
	HUD_Cooldown(void);
	virtual ~HUD_Cooldown(void);

	virtual void Init(Mesh* _displayMesh, Vector2 displayPos, Vector2 displayScale,
					  Mesh* _coverMesh, Vector2 coverPos, Vector2 coverScale, bool scrollX, bool scrollY);
	virtual void Update(double dt, float coverLevels, float coverLevelLimit);
	virtual void Clear();

	/*
	 * Setters
	 */
	void SetDisplayCover(GameObject2D* _displayCover);

	/*
	 * Getters
	 */
	GameObject2D* GetDisplayCover();
};

#endif