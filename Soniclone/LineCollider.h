#pragma once
#include <math.h>
#include "Vec2.h"

struct LineCollider
{
	Vec2 *start = new Vec2(0.0f, 0.0f);
	Vec2 *stop = new Vec2(0.0f, 0.0f);
	LineCollider();
	LineCollider(Vec2 *start, Vec2 *stop);
	~LineCollider();
};

