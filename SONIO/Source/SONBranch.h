#ifndef SON_BRANCH_H
#define SON_BRANCH_H

#include <string>
#include <vector>

using std::string;
using std::vector;

struct Attribute
{
	string name;
	string value;


	Attribute(string _name = "", string _value = "")
	{
		Set(_name, _value);
	}

	void Set(string _name = "", string _value = "")
	{
		name = _name;
		value = _value;
	}
};

struct Branch
{
	string name;
	vector<Attribute> attributes;
	vector<Branch> childBranches;

	Branch (string _name = "")
	{
		name = _name;
	}
};

#endif