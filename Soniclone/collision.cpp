#include "collision.h"

bool checkCollision(CircleCollider &c1, CircleCollider &c2)
{
	float dist = c1.pos->dist(*c2.pos);
	return dist < c1.r + c2.r;
}

bool checkCollision(CircleCollider &c, RectCollider &r, Vec2 *cptr)
{
	Vec2 closest = Vec2(0.0f, 0.0f);
	Vec2 &cp = *c.pos;
	Vec2 &rp = *r.pos;

	// Circle center left of rect
	if (cp.x < rp.x)
	{
		closest.x = rp.x;
	}
	// Circle center right of rect
	else if (cp.x > rp.x + rp.x)
	{
		closest.x = rp.x + rp.x;
	}
	// Circle center in horizontal rect space
	else closest.x = cp.x;

	// Circle center above rect
	if (cp.y < rp.y)
	{
		closest.y = rp.y;
	}
	// Circle center below rect
	else if (cp.y > rp.y + rp.y)
	{
		closest.y = rp.y + rp.y;
	}
	// Circle center in vertical rect space
	else closest.y = cp.y;

	if (cptr != nullptr) *cptr = closest;

	float dist = cp.dist(closest);
	return dist < c.r;
}

bool checkCollison(RectCollider &r1, RectCollider &r2)
{
	Vec2 &rp1 = *r1.pos;
	Vec2 &rp2 = *r2.pos;

	// Bottom 1 <= Top 2
	if (rp1.y + r1.h <= rp2.y) return false;
	// Top 1 >= Bottom 2
	if (rp1.y >= rp2.y + r2.h) return false;
	// Right 1 <= Left 2
	if (rp1.x + r1.h <= rp2.x) return false;
	// Left 1 >= Right
	if (rp1.x >= rp2.x + r2.h) return false;
	return true;
}

bool checkCollision(CircleCollider &c, LineCollider &l, Vec2 *cptr)
{
	Vec2 dir = *l.stop - *l.start;
	Vec2 ac = *c.pos - *l.start;
	// t lies between 0 and 1
	float t = fmaxf(0.0f, fminf(dir.dot(ac), 1.0f));

	// Closest point on line to center
	Vec2 closest = *l.start + dir * t;

	if (cptr != nullptr) *cptr = closest;

	if ((*c.pos - closest).norm() < c.r) return true;
	return false;
}

Vec2 collisionDisp(CircleCollider &c1, CircleCollider &c2)
{
	Vec2 dir = *c1.pos - *c2.pos;
	float d = c1.r + c2.r - dir.norm();

	return dir.normalize() * d;
}

Vec2 collisionDisp(CircleCollider &c, RectCollider &r, Vec2 *cptr)
{
	bool local = (cptr == nullptr);
	if (local)
	{
		cptr = new Vec2(0.0f, 0.0f);
		checkCollision(c, r, cptr);
	}
	Vec2 closest = *cptr;
	Vec2 dir = *c.pos - closest;
	float d = c.r - dir.norm();

	if (local) delete cptr;
	
	return dir.normalize() * d;
}

Vec2 collisionDisp(RectCollider &r1, RectCollider &r2)
{
	Vec2 &rp1 = *r1.pos;
	Vec2 &rp2 = *r2.pos;
	float xdisp, ydisp;
	xdisp = fmaxf(0, fminf(rp1.x + r1.w - rp2.x, rp1.x - rp2.x + r2.w));
	ydisp = fmaxf(0, fminf(rp1.y + r1.h - rp2.y, rp1.y - rp2.y + r2.h));

	return xdisp < ydisp ? Vec2(xdisp, 0.0f) : Vec2(0.0f, ydisp);
}

Vec2 collisionDisp(CircleCollider &c, LineCollider &l, Vec2 *cptr)
{
	bool local = (cptr == nullptr);
	if (local)
	{
		cptr = new Vec2(0.0f, 0.0f);
		checkCollision(c, l, cptr);
	}
	Vec2 closest = *cptr;
	Vec2 dir = *c.pos - closest;
	float d = c.r - dir.norm();

	if (local) delete cptr;

	return dir.normalize() * d;
}