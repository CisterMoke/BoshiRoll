#pragma once
#include <math.h>
#include "Vec2.h"

struct CircleCollider;

struct BaseCollider
{
	BaseCollider() = default;
	~BaseCollider() = default;
	bool virtual checkCollision(CircleCollider &c, Vec2 *cptr = nullptr) = 0;
	Vec2 virtual collisionDisp(CircleCollider &c, Vec2 *cptr = nullptr) = 0;
};

struct CircleCollider : public BaseCollider
{
	Vec2 *pos = new Vec2(0.0f, 0.0f);
	float r = 1.0f;

	CircleCollider();
	CircleCollider(Vec2 *p, float radius);
	CircleCollider(float x, float y, float radius);
	~CircleCollider();
	bool checkCollision(CircleCollider &c, Vec2 *cptr = nullptr);
	Vec2 collisionDisp(CircleCollider &c, Vec2 *cptr = nullptr);
};

struct LineCollider : public BaseCollider
{
	Vec2 *start = new Vec2(0.0f, 0.0f);
	Vec2 *stop = new Vec2(0.0f, 0.0f);
	LineCollider();
	LineCollider(Vec2 *start, Vec2 *stop);
	~LineCollider();
	bool checkCollision(CircleCollider &c, Vec2 *cptr = nullptr);
	Vec2 collisionDisp(CircleCollider &c, Vec2 *cptr = nullptr);
};

struct RectCollider : public BaseCollider
{
	Vec2 *pos = new Vec2(0.0f, 0.0f);
	float w, h;

	RectCollider();
	RectCollider(Vec2 *p, float width, float height);
	RectCollider(float x, float y, float width, float height);
	~RectCollider();
	bool checkCollision(CircleCollider &c, Vec2 *cptr = nullptr);
	bool checkCollison(RectCollider &r);
	Vec2 collisionDisp(CircleCollider &c, Vec2 *cptr = nullptr);
	Vec2 collisionDisp(RectCollider &r);
};
