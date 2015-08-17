#ifndef GOBJECT_H
#define GOBJECT_H

#include "Vector3.h"

/******************************************************************************/
/*!
		Struct Transform:
\brief	Consists of 3 Vector3 that control Transformation (Translate, Rotate, Scale)
*/
/******************************************************************************/
struct Transform
{
	Vector3 Translation;
	Vector3 Rotation;
	Vector3 Scale;

	Transform(void)
	{
		Scale = Vector3(1.0f, 1.0f, 1.0f);
	}
};

class GObject
{
	Transform m_transforms;

	public:
		GObject(void);
		virtual ~GObject(void);

		void SetPos(Vector3 pos);
		void SetRot(Vector3 rot);
		void SetScale(Vector3 scale);

		Transform GetTransform(void) const;
};

#endif