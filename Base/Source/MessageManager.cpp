#include "MessageManager.h"
#include "SONIO.h"


const Vector2 MessageManager::TITLE_POS_OFFSET(50.0f, 55.0f);
const Vector2 MessageManager::TEXT_POS_OFFSET(50.0f, 90.0f);
const string MessageManager::NO_MESSAGE_TIMER_TITLE("NO_MESSAGE");

MessageManager::MessageManager()
	: m_newMessage(true)
	, m_timer(0.0)
{
}


MessageManager::~MessageManager()
{
}

void MessageManager::Init(Mesh * _messageBackground, Mesh* _textMesh, Mesh* _titleMesh, Vector2 messageScale, Vector2 margin)
{
	// Init the MessageBG
	m__messageBG = new GameObject2D;
	m__messageBG->SetMesh(_messageBackground);
	m__messageBG->SetScale(messageScale);

	// Init the Text
	m__messageTextTemplate = new TextObject(_textMesh);
	m__messageTextTemplate->SetScale(Vector2(2.2f));

	// Init the Title
	m__messageTitle = new TextObject(_titleMesh);
	m__messageTitle->SetScale(Vector2(3.5f));

	// Set the Margin
	m_margin = margin;
}

void MessageManager::Update(double dt)
{
	if (m_messages.size() > 0)
	{
		Message message = m_messages.front();

		// Only update the timer if there is still a message
		m_timer += dt;

		// Remove this message if this message's time is up
		if (m_timer > message.m_timeToShow)
		{
			m_timer = 0.0;
			m_messages.pop();
			m_newMessage = true;
		}
	}
}

void MessageManager::Exit()
{
	// Delete the TextObject resources
	while (m_messageTextList.size() > 0)
	{
		TextObject* to = m_messageTextList.back();

		if (to != NULL)
		{
			delete to;
			m_messageTextList.pop_back();
		}
	}

	// Delete the template
	if (m__messageTextTemplate != NULL)
	{
		delete m__messageTextTemplate;
		m__messageTextTemplate = NULL;
	}

	// Delete the background
	if (m__messageBG != NULL)
	{
		delete m__messageBG;
		m__messageBG = NULL;
	}

	// Delete the title
	if (m__messageTitle != NULL)
	{
		delete m__messageTitle;
		m__messageTitle = NULL;
	}
}

void MessageManager::SetMessageBGScale(Vector2 scale)
{
	m__messageBG->SetScale(scale);
}

void MessageManager::AddMessages(string filePath)
{
	const string ROOT_NAME = "MessageContainer";
	const string MESSAGE_POS_NAME[NUM_MESSAGE_POSITION] =
	{
		"MP_TOP_LEFT",
		"MP_TOP_RIGHT",
		"MP_BOT_LEFT",
		"MP_BOT_RIGHT"
	};

	Branch messageBranch = SONIO::LoadSON(filePath);

	if (messageBranch.name == ROOT_NAME)
	{
		// For each branch/message
		for (vector<Branch>::iterator msgIter = messageBranch.childBranches.begin(); msgIter != messageBranch.childBranches.end(); ++msgIter)
		{
			Branch msg = *msgIter;

			// Create a message and set the name of that message to the name of this branch
			Message message(msg.name);

			// For each attribute/ message property
			for (vector<Attribute>::iterator attribIter = msg.attributes.begin(); attribIter != msg.attributes.end(); ++attribIter)
			{
				Attribute attrib = *attribIter;

				if (attrib.name == "Message")
				{
					message.m_message = attrib.value;
				}
				else if (attrib.name == "MessagePosition")
				{
					for (int pos = 0; pos < NUM_MESSAGE_POSITION; ++pos)
					{
						if (attrib.value == MESSAGE_POS_NAME[pos])
						{
							message.m_posType = static_cast<MESSAGE_POSITION_TYPE>(pos);
						}
					}
				}
				else if (attrib.name == "TimeToShow")
				{
					message.m_timeToShow = stod(attrib.value);
				}
			}

			// Add this message into the message queue
			m_messages.push(message);
		}
	}
}

void MessageManager::AddMessage(Message msg)
{
	m_messages.push(msg);
}

void MessageManager::ClearMessages(void)
{
	while (m_messages.size() > 0)
	{
		m_messages.pop();
	}
}

Vector2 MessageManager::GetMessageBGScale(void)
{
	return m__messageBG->GetTransform().Scale;
}

vector<GameObject2D*> MessageManager::GetMessageObjects(int viewWidth, int viewHeight)
{
	static vector<GameObject2D*> goList;			// The list of gameobjects to render regarding this message

	// Retrieve a message
	Message message;

	if (m_messages.size() > 0)
	{
		message = m_messages.front();

		// If message is meant as a timer, then don't render anything
		if (message.m_messageTitle == NO_MESSAGE_TIMER_TITLE)
		{
			return goList;
		}
		else if (!m_newMessage)
		{
			return goList;
		}
	}
	else
	{
		goList.clear();
		return goList;
	}

	// Calculate the position of this message
	const Vector2 S_POS_OF[NUM_MESSAGE_POSITION] =
	{
		Vector2(0.0f, viewHeight),
		Vector2(viewWidth, viewHeight),
		Vector2(0.0f, 0.0f),
		Vector2(viewWidth, 0.0f)
	};
	Vector2 messagePos = S_POS_OF[message.m_posType];
	Vector2 messageBGScale = m__messageBG->GetTransform().Scale;

	switch (message.m_posType)
	{
		case MP_TOP_LEFT:
		{
			messagePos.x += m_margin.x;
			messagePos.y -= m_margin.y + messageBGScale.y;
			break;
		}
		case MP_TOP_RIGHT:
		{
			messagePos.x -= m_margin.x + messageBGScale.x;
			messagePos.y -= m_margin.y + messageBGScale.y;
			break;
		}
		case MP_BOT_LEFT:
		{
			messagePos.x += m_margin.x;
			messagePos.y += m_margin.y;
			break;
		}
		case MP_BOT_RIGHT:
		{
			messagePos.x -= m_margin.x + messageBGScale.x;
			messagePos.y += m_margin.y;
			break;
		}
	}

	// Prepare the message
	// -- Calculate the position of the text
	Vector2 messageTextPos = messagePos;
	messageTextPos.x += TEXT_POS_OFFSET.x;
	messageTextPos.y += messageBGScale.y - TEXT_POS_OFFSET.y;
	messageTextPos = messageTextPos * 0.1;

	// -- Deactivate the previous text objects
	deactivateTextObjects();

	// -- Split a long message into multiple lines
	vector<TextObject*> textMessages;
	const size_t MAX_LETTERS_PER_LINE = (m__messageBG->GetTransform().Scale.x / m__messageTextTemplate->GetTransform().Scale.x) * 0.15 /* A constant value that returns the correct values. */;
	static const Vector2 SHIFT_PER_LINE(0.0f, -m__messageTextTemplate->GetTransform().Scale.x);

	int numSubStr = 0;		// Notes down number of substring created so as to shift the text down properly

	for (size_t letter = 0; letter < message.m_message.length(); ++numSubStr)
	{
		// Concatenate the string
		string str = message.m_message.substr(letter, MAX_LETTERS_PER_LINE);

		// Build the Text Object
		TextObject* text = fetchTextObject();
		text->SetPos(messageTextPos + (SHIFT_PER_LINE * numSubStr));
		text->SetText(str);
		textMessages.push_back(text);

		// Move to the next sub string
		letter += MAX_LETTERS_PER_LINE;
	}

	// Prepare the title
	// -- Calculate the position of the text
	Vector2 messageTitlePos = messagePos;
	messageTitlePos.x += TITLE_POS_OFFSET.x;
	messageTitlePos.y += messageBGScale.y - TITLE_POS_OFFSET.y;
	messageTitlePos = messageTitlePos * 0.1;

	m__messageTitle->SetPos(messageTitlePos);
	m__messageTitle->SetText(message.m_messageTitle);

	// Prepare the Background
	m__messageBG->SetPos(messagePos);

	// Ensure the list is clear
	goList.clear();
	// Render the background
	goList.push_back(m__messageBG);
	// Render the message on top of the background
	for (vector<TextObject*>::iterator textObjIter = textMessages.begin(); textObjIter != textMessages.end(); ++textObjIter)
	{
		if ((*textObjIter)->GetActive() == true)
		{
			goList.push_back(*textObjIter);
		}
	}	
	// Render the title on top of the background
	goList.push_back(m__messageTitle);

	m_newMessage = false;

	return goList;
}

TextObject * MessageManager::fetchTextObject(void)
{
	// Find an inactive TextObject
	for (vector<TextObject*>::iterator textObjIter = m_messageTextList.begin(); textObjIter != m_messageTextList.end(); ++textObjIter)
	{
		if ((*textObjIter)->GetActive() == false)
		{
			(*textObjIter)->SetActive(true);
			return *textObjIter;
		}
	}

	// Generate one if none are available
	TextObject* textObj = new TextObject(NULL);
	*textObj = *m__messageTextTemplate;
	textObj->SetActive(true);
	m_messageTextList.push_back(textObj);

	return m_messageTextList.back();
}

void MessageManager::deactivateTextObjects(void)
{
	for (vector<TextObject*>::iterator textObjIter = m_messageTextList.begin(); textObjIter != m_messageTextList.end(); ++textObjIter)
	{
		(*textObjIter)->SetActive(false);
	}
}
