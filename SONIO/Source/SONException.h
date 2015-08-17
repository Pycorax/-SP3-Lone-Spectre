#ifndef SON_EXCEPTION_H
#define SON_EXCEPTION_H

#include <string>

using std::string;

class SONException
{
	public:
		enum SON_EXCEPTION_TYPE
		{
			UNKNOWN_EXCEPTION,
			SON_FILE_ALREADY_EXISTS,
			SON_FILE_NOT_FOUND_FOR_READ,
			SON_FILE_FAILED_CREATION,
			NUM_EXCEPTIONS
		};
		
		SONException(SON_EXCEPTION_TYPE _type = UNKNOWN_EXCEPTION);
		~SONException();

		string GetMessage(void);

	private:
		SON_EXCEPTION_TYPE type;
		static const string message[NUM_EXCEPTIONS];
};

#endif