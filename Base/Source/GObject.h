#ifndef GOBJECT_H
#define GOBJECT_H

#include "Vector3.h"
#include "Mesh.h"

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

	bool operator==(Transform rhs)
	{
		return (Translation == rhs.Translation && Rotation == rhs.Rotation && Scale == rhs.Scale);
	}
};

class GObject
{
private:	// Variables
	Transform m_transforms;
	Mesh* m__mesh;
	bool m_active;

public:		// Functions
	GObject(void);
	virtual ~GObject(void);
	
	// Setters
	void SetPos(const Vector3 pos);
	void SetRot(const Vector3 rot);
	void SetScale(const Vector3 scale);
	void SetMesh(const Mesh* _mesh);

	// Getters
	Transform GetTransform(void) const;
	Mesh* GetMesh(void) const;
};

#endif