/******************************************************************************/
/*!
\file	BasicObject.h
\author Tng Kah Wei
\brief
Structs and class to store data about an object that is to exist in the game
*/
/******************************************************************************/
#ifndef BASIC_OBJECT_H
#define BASIC_OBJECT_H

#include "Vector3.h"
#include "Mesh.h"
#include "GObject.h"

class BasicObject
{
	protected:
		Transform m_transforms;
		Mesh* m_mesh;
		
		bool m_bLighted;
		bool m_bFogged;

		// Billboard: Follow Camera Rotation
		bool m_bBillboard;

	public:
		BasicObject(Mesh * mesh = NULL);
		virtual ~BasicObject();

		void SetPos(Vector3 pos);
		void SetRot(Vector3 rot);
		void SetScale(Vector3 scale);

		void SetMesh(Mesh* mesh);
		void SetLighted(bool light);
		void SetFogged(bool fog);
		void SetBillboard(bool billboard);

		Transform GetTransform(void) const;
		Mesh* GetMesh(void);
		bool IsLighted(void);
		bool IsFogged(void);
		bool IsBillboard(void);
};

#endif