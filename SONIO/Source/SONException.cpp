#include "SONException.h"

const string SONException::message[NUM_EXCEPTIONS] =
{
	"SON File already exists. Writing to this file will overwrite it.",
	"SON File does not exist. Are you sure the correct directory was specified?",
	"Failed to create SON File."
};

SONException::SONException(SON_EXCEPTION_TYPE _type/* = UNKNOWN_EXCEPTION*/) : type(_type)
{
}


SONException::~SONException()
{
}

string SONException::GetMessage()
{
	return message[type];
}