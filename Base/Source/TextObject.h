#ifndef TEXT_OBJECT_H
#define TEXT_OBJECT_H

#include "GameObject2D.h"

#include <string>

#include "Vector3.h"
#include "Vertex.h"

using std::string;

class TextObject : public GameObject2D
{
	string m_text;
	Color m_col;

	public:
		TextObject(Mesh * font, string text = "", Color col = Color());
		~TextObject(void);

		void SetText(string text);
		void SetColor(Color col);

		string GetText(void);
		Color GetColor(void);
};

#endif