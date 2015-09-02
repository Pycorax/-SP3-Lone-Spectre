#ifndef HUD_OBJECTIVE_H
#define HUD_OBJECTIVE_H

#include "HUD.h"
#include "TextObject.h"

// Objective
#include "ObjectiveAssassinate.h"
#include "ObjectiveCollect.h"
#include "ObjectiveDefuse.h"
#include "ObjectiveHostage.h"
#include "ObjectiveSetBomb.h"
#include "ObjectiveEscape.h"

class HUD_Objectives : public HUD
{
public:
	enum OBJECTIVE_TYPE
	{
		OBJECTIVE_ESCAPE = 0,
		OBJECTIVE_ASSASSINATE,
		OBJECTIVE_COLLECT,
		OBJECTIVE_DEFUSE,
		OBJECTIVE_HOSTAGE,
		OBJECTIVE_SET_BOMB,
		NUM_OBJECTIVE,
	};
	static const string S_M_OBJECTIVE_TEXTS[NUM_OBJECTIVE];
protected:
	TextObject* m__objectiveText;
public:
	HUD_Objectives(void);
	virtual ~HUD_Objectives(void);

	virtual void Init(Mesh* _displayMesh, Vector2 displayPos, Vector2 displayScale,
		Mesh* _objMesh, string text, Vector2 objPos, Vector2 objScale);
	virtual void Update(double dt, Objective* _obj);
	virtual void Clear();

	void NumberToText(float num);

	/*
	* Setters
	*/
	void SetObjectiveText(TextObject* _objectiveText);

	/*
	* Getters
	*/
	GameObject2D* GetObjectiveText();
};

#endif