#include "Colliders.h"

void CircleCollider::draw(SDL_Renderer *renderer, const SDL_Color  &color, const Vec2 &orig, const Vec2 &offset, const Mat22 &T)
{
	Vec2 center = T * (*pos - orig) + offset;
	float zoom_x = (T * Vec2(1.0f, 0.0f)).norm();
	float zoom_y = (T * Vec2(0.0f, 1.0f)).norm();
	ellipseRGBA(renderer, center.x, center.y, r * zoom_x, r * zoom_y, color.r, color.g, color.b, 255);
}

CircleCollider::CircleCollider() {}
CircleCollider::CircleCollider(Vec2 *p, float radius)
	: pos(p), r(radius) {}
CircleCollider::CircleCollider(float x, float y, float radius)
	: pos(new Vec2(x, y)), r(radius) {}
CircleCollider::~CircleCollider()
{
	delete pos;
}

bool CircleCollider::checkCollision(CircleCollider &c, Vec2 *cptr)
{
	float dist = this->pos->dist(*c.pos);
	return dist < this->r + c.r;
}

Vec2 CircleCollider::collisionDisp(CircleCollider &c, Vec2 *cptr)
{
	Vec2 dir = *c.pos - *this->pos;
	float d = c.r + this->r - dir.norm();

	return dir.normalize() * d;
}

LineCollider::LineCollider() {};
LineCollider::LineCollider(Vec2 *start, Vec2 *stop)
	: start(start), stop(stop) {};
LineCollider::~LineCollider()
{
	delete start;
	delete stop;
}

bool LineCollider::checkCollision(CircleCollider &c, Vec2 *cptr)
{
	Vec2 dir = *this->stop - *this->start;
	Vec2 ac = *c.pos - *this->start;
	// t lies between 0 and 1
	float t = dir.dot(ac) / dir.norm2();
	t = fmaxf(0.0f, fminf(t, 1.0f));

	// Closest point on line to center
	Vec2 closest = *this->start + dir * t;

	if (cptr != nullptr) *cptr = closest;

	if ((*c.pos - closest).norm() < c.r) return true;
	return false;
}

Vec2 LineCollider::collisionDisp(CircleCollider &c, Vec2 *cptr)
{
	bool local = (cptr == nullptr);
	if (local)
	{
		cptr = new Vec2(0.0f, 0.0f);
		checkCollision(c, cptr);
	}
	Vec2 closest = *cptr;
	Vec2 dir = *c.pos - closest;
	float d = c.r - dir.norm();

	if (local) delete cptr;

	return dir.normalize() * d;
}

void LineCollider::draw(SDL_Renderer *renderer, const SDL_Color &color, const Vec2 &orig, const Vec2 &offset, const Mat22 &T)
{
	Vec2 begin = T * (*start - orig) + offset;
	Vec2 end = T * (*stop - orig) + offset;
	lineRGBA(renderer, begin. x, begin.y, end.x, end.y, color.r, color.g, color.b, 255);
}

RectCollider::RectCollider() {}
RectCollider::RectCollider(Vec2 *p, float width, float height)
	: pos(p), w(width), h(height) {}
RectCollider::RectCollider(float x, float y, float width, float height)
	: pos(new Vec2(x, y)), w(width), h(height) {}
RectCollider::~RectCollider()
{
	delete pos;
}

bool RectCollider::checkCollision(CircleCollider &c, Vec2 *cptr)
{
	Vec2 closest = Vec2(0.0f, 0.0f);
	Vec2 &cp = *c.pos;
	Vec2 &rp = *this->pos;

	// Circle center left of rect
	if (cp.x < rp.x)
	{
		closest.x = rp.x;
	}
	// Circle center right of rect
	else if (cp.x > rp.x + this->w)
	{
		closest.x = rp.x + this->w;
	}
	// Circle center in horizontal rect space
	else closest.x = cp.x;

	// Circle center above rect
	if (cp.y < rp.y)
	{
		closest.y = rp.y;
	}
	// Circle center below rect
	else if (cp.y > rp.y + this->h)
	{
		closest.y = rp.y + this->h;
	}
	// Circle center in vertical rect space
	else closest.y = cp.y;

	if (cptr != nullptr) *cptr = closest;

	float dist = cp.dist(closest);
	return dist < c.r;
}

bool RectCollider::checkCollison(RectCollider &r)
{
	Vec2 &tp = *this->pos;
	Vec2 &rp = *r.pos;

	// Bottom 1 <= Top 2
	if (tp.y + this->h <= rp.y) return false;
	// Top 1 >= Bottom 2
	if (tp.y >= rp.y + r.h) return false;
	// Right 1 <= Left 2
	if (tp.x + this->h <= rp.x) return false;
	// Left 1 >= Right
	if (tp.x >= rp.x + r.h) return false;
	return true;
}

Vec2 RectCollider::collisionDisp(CircleCollider &c, Vec2 *cptr)
{
	bool local = (cptr == nullptr);
	if (local)
	{
		cptr = new Vec2(0.0f, 0.0f);
		checkCollision(c, cptr);
	}
	Vec2 closest = *cptr;
	Vec2 dir = *c.pos - closest;
	float d = c.r - dir.norm();

	if (local) delete cptr;

	return dir.normalize() * d;
}

Vec2 RectCollider::collisionDisp(RectCollider &r)
{
	Vec2 &tp = *this->pos;
	Vec2 &rp = *r.pos;
	float xdisp, ydisp;
	xdisp = fmaxf(0, fminf(tp.x + this->w - rp.x, tp.x - rp.x + r.w));
	ydisp = fmaxf(0, fminf(tp.y + this->h - rp.y, tp.y - rp.y + r.h));

	return xdisp < ydisp ? Vec2(xdisp, 0.0f) : Vec2(0.0f, ydisp);
}

void RectCollider::draw(SDL_Renderer *renderer, const SDL_Color &color, const Vec2 &orig, const Vec2 &offset, const Mat22 &T)
{
	Vec2 lu = T * (*pos - orig) + offset;
	float zoom_x = (T * Vec2(1.0f, 0.0f)).norm();
	float zoom_y = (T * Vec2(0.0f, 1.0f)).norm();

	rectangleRGBA(renderer, lu.x, lu.y, lu.x + w*zoom_x, lu.y + h*zoom_y, color.r, color.g, color.b, 255);
}