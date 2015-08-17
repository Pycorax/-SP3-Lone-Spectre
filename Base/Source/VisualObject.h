/******************************************************************************/
/*!
\file	BasicObject.h
\author Tng Kah Wei
\brief
A type of BasicObject that supports repeated rendering
*/
/******************************************************************************/
#ifndef VISUAL_OBJECT_H
#define VISUAL_OBJECT_H

#include "BasicObject.h"

class VisualObject : public BasicObject
{
	Transform m_deltaTransform;
	int m_repeat;

	public:
		VisualObject(Mesh* mesh = NULL);
		virtual ~VisualObject(void);

		void SetDeltaTransform(Transform deltaTrans);
		void SetDeltaPos(Vector3 deltaPos);
		void SetDeltaRot(Vector3 deltaRot);
		void SetDeltaScale(Vector3 deltaScale);
		void SetRepeat(int repeat);

		Transform GetDeltaTransform(void);
		int GetRepeat(void);
};

#endif