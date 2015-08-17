/******************************************************************************/
/*!
\file	GameObject.h
\author Tng Kah Wei
\brief
Class to store data about an interactive object that is to exist in the game
*/
/******************************************************************************/

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "BasicObject.h"

#include "Mesh.h"
#include "Vector3.h"

/******************************************************************************/
/*!
		Class GameObject:
\brief	Class to store data about an object that is to exist in the game
*/
/******************************************************************************/
class GameObject : public BasicObject
{
	public:
		GameObject(Mesh * mesh = NULL, bool collidable = true);
		virtual ~GameObject();
		
		void SetBoundExt(Vector3 ext);
		void SetBoundOffset(Vector3 offset);
		void SetCollidable(bool collide);
		
		Vector3 GetBoundExt(void);
		Vector3 GetBoundOffset(void);
		bool IsCollidable(void);
		
		bool CollisionTest(GameObject* go);

	protected:
		bool m_collidable;

		// Bound extension
		Vector3 m_boundExt;	
		Vector3 m_boundOffset;
};

#endif
