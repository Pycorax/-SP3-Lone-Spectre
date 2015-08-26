#include "MessageManager.h"
#include "SONIO.h"


MessageManager::MessageManager()
{
}


MessageManager::~MessageManager()
{
}

void MessageManager::Init(Mesh * _messageBackground, Mesh* _textMesh, Mesh* _titleMesh, int messageWidth, int messageHeight)
{
	// Init the MessageBG
	m_messageBG = new GameObject2D;
	m_messageBG->SetMesh(_messageBackground);
	m_messageBG->SetScale(Vector2(messageWidth, messageHeight));

	// Init the Text
	m_messageText = new TextObject(_textMesh);

	// Init the Title
	m_messageTitle = new TextObject(_titleMesh);
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

			Message message;

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

vector<GameObject2D*> MessageManager::GetMessageObject(int viewWidth, int viewHeight)
{
	vector<GameObject2D*> goList;

	// Retrieve a message
	Message message = m_messages.front();
	m_messages.pop();

	// Calculate the position of this message
	static const Vector2 S_POS_OF[NUM_MESSAGE_POSITION] =
	{
		Vector2(0.0f, viewHeight),
		Vector2(viewWidth, viewHeight),
		Vector2(0.0f, 0.0f),
		Vector2(viewWidth, 0.0f)
	};
	Vector2 messagePos = S_POS_OF[message.m_posType];
	Vector2 messageScale = m_messageBG->GetTransform().Scale;

	switch (message.m_posType)
	{
		case MP_TOP_LEFT:
		{
			messagePos.x += m_margin.x + messageScale.x;
			messagePos.y -= m_margin.y + messageScale.y;
			break;
		}
		case MP_TOP_RIGHT:
		{
			messagePos.x -= m_margin.x + messageScale.x;
			messagePos.y -= m_margin.y + messageScale.y;
			break;
		}
		case MP_BOT_LEFT:
		{
			messagePos.x += m_margin.x + messageScale.x;
			messagePos.y += m_margin.y + messageScale.y;
			break;
		}
		case MP_BOT_RIGHT:
		{
			messagePos.x -= m_margin.x + messageScale.x;
			messagePos.y += m_margin.y + messageScale.y;
			break;
		}
	}

	// Prepare the message
	//m_messageText->SetPos()
	m_messageText->SetText(message.m_message);

	// Prepare the title
	m_messageTitle->SetText(message.m_messageTitle);

	// Prepare the Background
	m_messageBG->SetPos(messagePos);

	// Render the background
	goList.push_back(m_messageBG);
	// Render the message on top of the background
	goList.push_back(m_messageText);

	return goList;
}
