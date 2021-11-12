#pragma once
#include "Vec2.h"
struct CircleCollider
{
	Vec2 *pos = new Vec2(0.0f, 0.0f);
	float r = 1.0f;

	CircleCollider();
	CircleCollider(Vec2 *p, float radius);
	CircleCollider(float x, float y, float radius);
	~CircleCollider();
};

