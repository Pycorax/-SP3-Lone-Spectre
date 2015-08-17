/************************************************************************/
/* Test code for LoadSON, change the project type to                    */
/* Application(.exe) to test                                            */
/************************************************************************/

#include <iostream>
#include "SONIO.h"

using std::cout;
using std::endl;

void PrintBranch(Branch b);
void PrintIndents(int indents);

int main(void)
{
	Branch myBranch = SONIO::LoadSON("Test.son");

	PrintBranch(myBranch);

	SONIO::WriteSON("result.son", myBranch);

	system("pause");

	return 0;
}

void PrintBranch(Branch b)
{
	static int level = 0;
	
	// Opening Brace
	++level;
	
	PrintIndents(level);
	cout << "{" << endl;

	// Stuff Within--------------------
	PrintIndents(level + 1);
	cout << "[Name] " << b.name << endl;
	for (size_t i = 0; i < b.attributes.size(); ++i)
	{
		PrintIndents(level + 1);
		cout << "[" << b.attributes[i].name << "] " << b.attributes[i].value << endl;
	}

	cout << endl;
	for (size_t i = 0; i < b.childBranches.size(); ++i)
	{
		PrintBranch(b.childBranches[i]);
	}
	
	// Stuff Within--------------------

	// Closing Brace
	PrintIndents(level);
	cout << "}" << endl;

	--level;
}

void PrintIndents(int indents)
{
	const unsigned INDENT_LENGTH = 4;

	for (size_t i = 0; i < (indents - 1) * INDENT_LENGTH; ++i)
	{
		cout << " ";
	}
}