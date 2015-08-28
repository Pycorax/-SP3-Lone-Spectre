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
	double m_timeToShow;						// Determines the time that the message should be shown

	Message(string messageTitle = "", string message = "", MESSAGE_POSITION_TYPE posType = MP_BOT_LEFT, double timer = 5.0)
	{
		Set(messageTitle, message, posType);
	}

	void Set(string messageTitle = "", string message = "", MESSAGE_POSITION_TYPE posType = MP_BOT_LEFT, double timer = 5.0)
	{
		m_messageTitle = messageTitle;
		m_message = message;
		m_posType = posType;
		m_timeToShow = timer;
	}
};

class MessageManager
{
private:	// Variables
	// Static Constants
	static const Vector2 TITLE_POS_OFFSET;			// Offset from the top left of the message box
	static const Vector2 TEXT_POS_OFFSET;			// Offset from the top left of the message box
	static const string NO_MESSAGE_TIMER_TITLE;		// A message with this title will be treated as a timer, no message will be displayed

	// Message Renderers
	GameObject2D* m__messageBG;					// GameObject of the message's background image
	TextObject* m__messageTitle;					// TextObject of the message's title
	TextObject* m__messageTextTemplate;			// TextObject Template for the message bodies
	vector<TextObject*> m_messageTextList;		// TextObject of the message

	// Message Properties
	Vector2 m_margin;					// The distance away from the corner of the screen stated by MESSAGE_POSITION_TYPE that the message should be rendered

	// Message List
	queue<Message> m_messages;			// The queue of messages to send

	// Message Timer
	double m_timer;						// Timer for the messages

	// Optimization
	bool m_newMessage;					// Indicates whether the current messge is a new message

public:		// Functions
	MessageManager();
	~MessageManager();

	void Init(Mesh * _messageBackground, Mesh* _textMesh, Mesh* _titleMesh, Vector2 messageScale, Vector2 margin = Vector2::ZERO_VECTOR);
	void Update(double dt);
	void Exit(void);

	/*
	 * Setters
	 */
	void SetMessageBGScale(Vector2 scale);

	void AddMessages(string filePath);
	void AddMessage(Message msg);
	void ClearMessages(void);

	/*
	 * Getters
	 */
	Vector2 GetMessageBGScale(void);
	vector<GameObject2D*> GetMessageObjects(int viewWidth, int viewHeight);

private:	// Functions
	TextObject* fetchTextObject(void);
	void deactivateTextObjects(void);
};

#endif