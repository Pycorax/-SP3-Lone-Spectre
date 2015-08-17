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
	Vector3 Translation;			// Translate Vector
	Vector3 Rotation;				// Rotate Vector
	Vector3 Scale;					// Scale Vector

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
	Transform m_transforms;			// Stores this object's Transforms info
	Mesh* m__mesh;					// Stores this object's mesh
	bool m_active;					// Controls if this object can be collided with

public:		// Functions
	GObject(void);
	virtual ~GObject(void);
	
	/*
	* Setters
	*/
	void SetPos(const Vector3 pos);
	void SetRot(const Vector3 rot);
	void SetScale(const Vector3 scale);
	void SetMesh(Mesh* _mesh);

	/*
	* Getters
	*/
	Transform GetTransform(void) const;
	Mesh* GetMesh(void) const;
};

#endif