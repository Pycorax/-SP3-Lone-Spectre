#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H

// STL Includes
#include <queue>
#include <string>

// Other Includes
#include "GameObject2D.h"

// Using Directives
using std::queue;
using std::string;

class MessageManager
{
	queue<string> m_messages;

public:
	MessageManager();
	~MessageManager();

	void AddMessages(string filePath);
	void AddMessage(string str);
	GameObject2D* GetMessageToDisplay(void);
};

#endif