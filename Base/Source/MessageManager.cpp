#include "MessageManager.h"
#include "SONIO.h"


const Vector2 MessageManager::TITLE_POS_OFFSET(50.0f, 55.0f);
const Vector2 MessageManager::TEXT_POS_OFFSET(50.0f, 90.0f);

MessageManager::MessageManager()
{
}


MessageManager::~MessageManager()
{
}

void MessageManager::Init(Mesh * _messageBackground, Mesh* _textMesh, Mesh* _titleMesh, Vector2 messageScale, Vector2 margin)
{
	// Init the MessageBG
	m_messageBG = new GameObject2D;
	m_messageBG->SetMesh(_messageBackground);
	m_messageBG->SetScale(messageScale);

	// Init the Text
	m_messageText = new TextObject(_textMesh);
	m_messageText->SetScale(Vector2(3.0f));

	// Init the Title
	m_messageTitle = new TextObject(_titleMesh);
	m_messageTitle->SetScale(Vector2(4.5f));

	// Set the Margin
	m_margin = margin;
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

vector<GameObject2D*> MessageManager::GetMessageObjects(int viewWidth, int viewHeight)
{
	vector<GameObject2D*> goList;

	// Retrieve a message
	Message message;

	if (m_messages.size() > 0)
	{
		message = m_messages.front();
		m_messages.pop();
	}
	else
	{
		return goList;
	}

	// Calculate the position of this message
	static const Vector2 S_POS_OF[NUM_MESSAGE_POSITION] =
	{
		Vector2(0.0f, viewHeight),
		Vector2(viewWidth, viewHeight),
		Vector2(0.0f, 0.0f),
		Vector2(viewWidth, 0.0f)
	};
	Vector2 messagePos = S_POS_OF[message.m_posType];
	Vector2 messageBGScale = m_messageBG->GetTransform().Scale;

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

	m_messageText->SetPos(messageTextPos);
	m_messageText->SetText(message.m_message);

	// Prepare the title
	// -- Calculate the position of the text
	Vector2 messageTitlePos = messagePos;
	messageTitlePos.x += TITLE_POS_OFFSET.x;
	messageTitlePos.y += messageBGScale.y - TITLE_POS_OFFSET.y;
	messageTitlePos = messageTitlePos * 0.1;

	m_messageTitle->SetPos(messageTitlePos);
	m_messageTitle->SetText(message.m_messageTitle);

	// Prepare the Background
	m_messageBG->SetPos(messagePos);

	// Render the background
	goList.push_back(m_messageBG);
	// Render the message on top of the background
	goList.push_back(m_messageText);
	// Render the title on top of the background
	goList.push_back(m_messageTitle);

	return goList;
}
