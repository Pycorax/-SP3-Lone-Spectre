#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H

// STL Includes
#include <queue>
#include <vector>
#include <string>

// Other Includes
#include "GameObject2D.h"
#include "TextObject.h"

// Using Directives
using std::queue;
using std::vector;
using std::string;

enum MESSAGE_POSITION_TYPE
{
	MP_TOP_LEFT,
	MP_TOP_RIGHT,
	MP_BOT_LEFT,
	MP_BOT_RIGHT,
	NUM_MESSAGE_POSITION
};

struct Message
{
	string m_messageTitle;						// The title of the message. Will be displayed above the message body
	string m_message;							// The message body
	MESSAGE_POSITION_TYPE m_posType;			// Determine the position of the message. The edge that the message will be aligned to.

	Message(string messageTitle = "", string message = "", MESSAGE_POSITION_TYPE posType = MP_BOT_LEFT)
	{
		Set(messageTitle, message, posType);
	}

	void Set(string messageTitle = "", string message = "", MESSAGE_POSITION_TYPE posType = MP_BOT_LEFT)
	{
		m_messageTitle = messageTitle;
		m_message = message;
		m_posType = posType;
	}
};

class MessageManager
{
private:	// Variables
	// Message Renderers
	GameObject2D* m_messageBG;			// GameObject of the message's background image
	TextObject* m_messageTitle;			// TextObject of the message's title
	TextObject* m_messageText;			// TextObject of the message

	// Message Properties
	Vector2 m_margin;					// The distance away from the corner of the screen stated by MESSAGE_POSITION_TYPE that the message should be rendered

	// Message List
	queue<Message> m_messages;			// The queue of messages to send

public:		// Functions
	MessageManager();
	~MessageManager();

	void Init(Mesh * _messageBackground, Mesh* _textMesh, Mesh* _titleMesh, int messageWidth, int messageHeight);

	void AddMessages(string filePath);
	void AddMessage(Message msg);
	vector<GameObject2D*> GetMessageObject(int viewWidth, int viewHeight);

private:	// Functions
};

#endif