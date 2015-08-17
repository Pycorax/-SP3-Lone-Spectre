/******************************************************************************/
/*!
\file	SONIO.h
\author Tng Kah Wei
\brief
Code to load a SON (Simple Object Notation) file, inspired by JSON.

Format of a SON file:
Made up of "Branches" {} which contains Attributes [<attribute name>] <attribute value> and has child branches
E.g.
{
	[Name] Level 1 Parent Branch
	[TestProperty] TestValue

	{
		[Name] Level 2 Child Branch of Level 1 Branch
	}
}
*/
/******************************************************************************/
#ifndef SONIO_H
#define SONIO_H

#include <string>
#include <vector>
#include <fstream>
#include "SONBranch.h"

using std::string;
using std::vector;
using std::ofstream;

/******************************************************************************/
/*!
		class SONIO:
\brief	Simple Object Notation Input Output Class for processing SON files.		
*/
/******************************************************************************/
class SONIO
{
	private:
		static const char OPEN_BRANCH = '{';
		static const char CLOSE_BRANCH = '}';
		static const char OPEN_ATTRIB = '[';
		static const char CLOSE_ATTRIB = ']';
		static const char COMMENT = '#';

		static Branch GetBranches(vector<string> branchBlock);
		static void WriteBranch(Branch branchBlock, ofstream& file);

		static vector<string> FileToVector(string fileName);
		static string Trim(string str);
		static bool fileExists(const char *fileName);
		static string CreateIndent(int depth);

	public:
		static Branch LoadSON(string fileName);
		static void WriteSON(string fileName, Branch branch);
};



#endif