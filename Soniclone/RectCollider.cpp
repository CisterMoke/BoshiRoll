#include "RectCollider.h"

RectCollider::RectCollider() {}
RectCollider::RectCollider(Vec2 *p, float width, float height)
	: pos(p), w(width), h(height) {}
RectCollider::RectCollider(float x, float y, float width, float height)
	: pos(new Vec2(x, y)), w(width), h(height) {}
RectCollider::~RectCollider()
{
	delete pos;
}