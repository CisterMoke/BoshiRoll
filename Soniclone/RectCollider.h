#pragma once
#include "Vec2.h"
struct RectCollider
{
	Vec2 *pos;
	float w, h;

	RectCollider();
	RectCollider(Vec2 *p, float width, float height);
	RectCollider(float x, float y, float width, float height);
	~RectCollider();
};

