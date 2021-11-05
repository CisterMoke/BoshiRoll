#include "CircleCollider.h"

CircleCollider::CircleCollider() {}
CircleCollider::CircleCollider(Vec2 *p, float radius)
	: pos(p), r(radius) {}
CircleCollider::CircleCollider(float x, float y, float radius)
	: pos(new Vec2(x, y)), r(radius) {}
CircleCollider::~CircleCollider()
{
	delete pos;
}
