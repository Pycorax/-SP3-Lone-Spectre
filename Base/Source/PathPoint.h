/****************************************************************************/
/*!
\file PathPoint.h
\author Tng Kah Wei
\brief
Struct of a point in the world except designed for a Pathway class
!*/
/****************************************************************************/
#ifndef PATHPOINT_H
#define PATHPOINT_H

#include "Vector3.h"

/****************************************************************************/
/*!
		struct PathPoint:
\brief	Struct of a point in the world except designed for a Pathway class
		The difference is that a speed modifier has been added to control the speed at
		which a CharacterObject will move towards a PathPoint
!*/
/****************************************************************************/
struct PathPoint
{
/****************************************************************************/
/*!
		float x:
\brief	The x-coordinate of the PathPoint
!*/
/****************************************************************************/
	float x;
/****************************************************************************/
/*!
		float y:
\brief	The y-coordinate of the PathPoint
!*/
/****************************************************************************/
	float y;
/****************************************************************************/
/*!
		float z:
\brief	The z-coordinate of the PathPoint
!*/
/****************************************************************************/
	float z;
/****************************************************************************/
/*!
		float speedMod:
\brief	The speedModifier of the PathPoint. 
		Controls how fast the NPC moves at this point.
!*/
/****************************************************************************/
	float speedMod;
/****************************************************************************/
/*!
\brief	
Default Constructor
!*/
/****************************************************************************/
	PathPoint(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, float _speedMod = 1.0f)
	{
		Set(_x, _y, _z, _speedMod);
	}
/****************************************************************************/
/*!
\brief	
Constructor to use a Vector3 to create a PathPoint
!*/
/****************************************************************************/
	PathPoint(Vector3 position)
	{
		Set(position);
	}
/****************************************************************************/
/*!
\brief	
Destructor
!*/
/****************************************************************************/
	~PathPoint()
	{

	}
/****************************************************************************/
/*!
\brief	
Setter function

\param _x - x-coordinate to set
\param _y - y-coordinate to set
\param _z - z-coordinate to set
\param speedMod - speed modifier to set
!*/
/****************************************************************************/
	void Set(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, float _speedMod = 1.0f)
	{
		x = _x;
		y = _y;
		z = _z;
		speedMod = _speedMod;
	}
/****************************************************************************/
/*!
\brief	
Setter function

\param position - Vector3 containing coordinates to set
!*/
/****************************************************************************/
	void Set(Vector3 position)
	{
		Set(position.x, position.y, position.z, 1.0f);
	}
/****************************************************************************/
/*!
\brief	
Converts a PathPoint to a Vector3, discarding speedMod

\return Returns a Vector3 with the x, y, z values of the PathPoint
!*/
/****************************************************************************/
	Vector3 ToVector3()
	{
		return Vector3(x, y ,z);
	}
};

#endif