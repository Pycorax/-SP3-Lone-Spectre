/******************************************************************************/
/*!
\file	Pathway.cpp
\author 
\brief

*/
/******************************************************************************/
#include "Pathway.h"

#include <fstream>
#include <string>

using std::ifstream;
using std::getline;
using std::string;
using std::stof;
/******************************************************************************/
/*!
\brief
default constructor
*/
/******************************************************************************/
Pathway::Pathway()
{

}
/******************************************************************************/
/*!
\brief
denstructor
*/
/******************************************************************************/
Pathway::~Pathway()
{
}
/******************************************************************************/
/*!
\brief
Adds a point into the path
*/
/******************************************************************************/
void Pathway::AddPoint(PathPoint _point)
{
	pathVector.push_back(_point);
}
/******************************************************************************/
/*!
\brief
Adds point into the path through reading from text file
*/
/******************************************************************************/
bool Pathway::AddPoints(const char* fileName)
{
	ifstream file;
	file.open(fileName);

	// Check if file is open before loading
	if (file.is_open())
	{
		string line;

		while (!file.eof())
		{
			PathPoint keyPoint;

			getline(file, line, ',');
			keyPoint.x = stof(line.c_str());
			getline(file, line, ',');
			keyPoint.y = stof(line.c_str());
			getline(file, line, ',');
			keyPoint.z = stof(line.c_str());
			getline(file, line);
			keyPoint.speedMod = stof(line);

			AddPoint(keyPoint);
		}

		// Close the file
		file.close();

		return true;
	}
	else
	{
		return false;
	}
}
/******************************************************************************/
/*!
\brief
Return a point at that particular position in a vector
*/
/******************************************************************************/
PathPoint Pathway::GetPoint(unsigned pos)
{
	if (pos < GetSize())
	{
		return pathVector[pos];
	}
	else
	{
		// Return a zero point if not exists
		return PathPoint();
	}
}
/******************************************************************************/
/*!
\brief
Get size of the pathVector size
*/
/******************************************************************************/
unsigned Pathway::GetSize()
{
	return pathVector.size();
}
/******************************************************************************/
/*!
\brief
clears the pathway vector
*/
/******************************************************************************/
void Pathway::Erase()
{
	pathVector.erase(pathVector.begin(), pathVector.end());
}