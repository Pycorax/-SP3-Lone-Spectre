/****************************************************************************/
/*!
\file Pathway.h
\author Tng Kah Wei
\brief
Class for storing a bunch of PathPoints
!*/
/****************************************************************************/
#ifndef PATHWAY_H
#define PATHWAY_H

#include <vector>
#include "PathPoint.h"

using std::vector;

/******************************************************************************/
/*!
		Class Pathway:
\brief	Class for storing a bunch of PathPoints and operations on it
*/
/******************************************************************************/
class Pathway
{
/******************************************************************************/
/*!
		vector<PathPoint> pathVector:
\brief	Stores all the PathPoints
*/
/******************************************************************************/
	vector<PathPoint> pathVector;

	public:
		Pathway();
		~Pathway();

		void AddPoint(PathPoint _point);
		bool AddPoints(const char* fileName);
		PathPoint GetPoint(unsigned pos);
		unsigned GetSize();
		void Erase();
};

#endif