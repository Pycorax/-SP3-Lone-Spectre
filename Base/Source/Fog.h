#ifndef FOG_H
#define FOG_H

#include "Vertex.h"

struct Fog
{
	float start;
	float end;
	float density;
	int type;
	Color col;
	bool enabled;

	Fog(float _start = 0,
		float _end = 10000,
		float _density = 0.00000004f,
		int _type = 2,
		Color _col = Color(0.5f, 0.5f, 0.5f),
		bool _enabled = false)
	{
		start = _start;
		end = _end;
		density = _density;
		SetType(_type);
		col = _col;
		enabled = _enabled;
	}

	void SetType (int _type)
	{
		if (_type >= 0 && _type <= 2)
		{
			type = _type;
		}
	}
};

#endif