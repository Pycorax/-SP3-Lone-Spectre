#include "TextObject.h"

TextObject::TextObject(Mesh * font, string text, Color col/* = Color()*/)
{
	SetMesh(font);
	m_text = text;
	m_col = col;
}


TextObject::~TextObject()
{
	// Mesh is not deleted here as a collection of meshes will be stored in a Mesh Library
}

void TextObject::SetText(string text)
{
	m_text = text;
}

void TextObject::SetColor(Color col)
{
	m_col = col;
}

string TextObject::GetText(void)
{
	return m_text;
}

Color TextObject::GetColor(void)
{
	return m_col;
}
