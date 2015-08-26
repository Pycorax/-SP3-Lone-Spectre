#include "SONIO.h"

#include "SONException.h"

#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::getline;
using std::ostringstream;

Branch SONIO::GetBranches(vector<string> branchBlock)
{
	Branch result;
	Attribute tempAttrib;

	bool openedBranch = false;
	bool validFile = true;

	// Get a branch block
	for (size_t i = 0; i < branchBlock.size(); ++i)
	{
		// Check first char of each line
		if (branchBlock[i][0] == OPEN_BRANCH)		// Find the opening
		{
			// If there is a new block
			if (openedBranch)
			{
				// Get this block
				vector<string> blockToPass;
				// -- Find this OPEN_BRANCH's CLOSE_BRANCH
				short braceLevel = 0;
				for (size_t j = i; j < branchBlock.size(); ++j)
				{
					if (branchBlock[j][0] == OPEN_BRANCH)
					{
						++braceLevel;
					}
					else if (branchBlock[j][0] == CLOSE_BRANCH)
					{
						--braceLevel;
					}

					if (braceLevel < 1)
					{
						// Load blockToPass with the lines for this block
						for (size_t line = i; line < j; ++line)
						{
							blockToPass.push_back(branchBlock[line]);
						}
						break;
					}
				}

				// Skip ahead the lines that the recursive call will handle
				i += blockToPass.size();

				// Pass this block in recursively and add to the top level branch
				result.childBranches.push_back(GetBranches(blockToPass));
			}
			else	// First block AKA current
			{
				openedBranch = true;
			}
		}
		else if (branchBlock[i][0] == OPEN_ATTRIB)		// Line is an attribute
		{
			// Find attribute name
			string attribName = "";
			size_t j = 1;
			while (branchBlock[i][j] != CLOSE_ATTRIB)
			{
				attribName += branchBlock[i][j];
				++j;
				if (j >= branchBlock[i].length())
				{
					validFile = false;
					break;
				}
			}

			// Skip the CLOSE_ATTRIB that was skipped
			++j;

			// Find property
			string attirbProp = "";
			for (; j < branchBlock[i].length(); ++j)
			{
				attirbProp += branchBlock[i][j];
			}

			// Special case attribute for "Name"
			if (attribName == "Name")
			{
				result.name = attirbProp;
			}
			else
			{
				// Add attribute into branch
				tempAttrib.Set(attribName, attirbProp);
				result.attributes.push_back(tempAttrib);
				tempAttrib.Set();
			}
		}
	}

	return result;
}

void SONIO::WriteBranch(Branch branchBlock, ofstream& file)
{
	// Indentation
	static int depth = 0;
	string braceIndent = createIndent(depth);
	string indent = createIndent(depth + 1);
	++depth;

	//-------------------- Start of the Branch --------------------//
	file << braceIndent << OPEN_BRANCH << "\n";

	// Write Branch Name
	file << indent << OPEN_ATTRIB << "Name" << CLOSE_ATTRIB << " " << branchBlock.name << "\n";

	// Loop through every attribute
	for (vector<Attribute>::iterator it = branchBlock.attributes.begin(); it != branchBlock.attributes.end(); ++it)
	{
		file << indent << OPEN_ATTRIB << it->name << CLOSE_ATTRIB << " " << it->value << "\n";
	}

	file << "\n";

	// Loop through every child
	for (vector<Branch>::iterator it = branchBlock.childBranches.begin(); it != branchBlock.childBranches.end(); ++it)
	{
		WriteBranch(*it, file);
	}

	file << braceIndent << CLOSE_BRANCH << "\n";
	//-------------------- End of the Branch --------------------//

	--depth;
}

vector<string> SONIO::fileToVector(string fileName)
{
	vector<string> result;
	ifstream file;

	file.open(fileName);

	if (file.is_open())
	{
		while (!file.eof())
		{
			string line;
			getline(file, line);
			line = trim(line);

			// Only add if the line is empty or not a comment
			if (line.length() > 0 && line[0] != '#')
			{
				result.push_back(line);
			}
		}
	}
	
	return result;
}

string SONIO::trim(string str)
{
	string result = "";
	bool stopTrim = false;

	for (size_t i = 0; i < str.length(); ++i)
	{
		if (str[i] == IGNORE_TRIM)
		{
			stopTrim = !stopTrim;
		}
		else if (stopTrim || (!stopTrim && !isWhiteSpace(str[i])))
		{
			result += str[i];
		}
	}

	return result;
}

Branch SONIO::LoadSON(string fileName)
{
	vector<string> codeToProcess = fileToVector(fileName);

	return GetBranches(codeToProcess);
}

void SONIO::WriteSON(string fileName, Branch branch)
{
	if (fileExists(fileName.c_str()))
	{
		throw SONException(SONException::SON_FILE_ALREADY_EXISTS);
	}

	// Create the file
	ofstream sonFile(fileName);

	if (sonFile.is_open())
	{
		WriteBranch(branch, sonFile);
	}
	else
	{
		throw SONException(SONException::SON_FILE_FAILED_CREATION);
	}
}

bool SONIO::fileExists(const char *fileName)
{
	ifstream infile(fileName);
	return infile.good();
}

string SONIO::createIndent(int depth)
{
	ostringstream result;
	
	for (int i = 0; i < depth; ++i)
	{
		result << "\t";
	}

	return result.str();
}

bool SONIO::isWhiteSpace(char c)
{
	if (c == ' ' || c == '\t')
	{
		return true;
	}
	else
	{
		return false;
	}
}
