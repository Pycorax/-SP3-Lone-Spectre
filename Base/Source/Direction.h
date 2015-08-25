#ifndef DIRECTION_H
#define DIRECTION_H

#include "Vector2.h"

namespace Direction
{
	enum E_DIR
	{
		DIR_UP,
		DIR_DOWN,
		DIR_LEFT,
		DIR_RIGHT,
		NUM_DIR,
	};
	const Vector2 DIRECTIONS[NUM_DIR] = {
											Vector2(0.f, 1.f),
											Vector2(0.f, -1.f),
											Vector2(-1.f, 0.f),
											Vector2(1.f, 0.f)
										};
}

#endif