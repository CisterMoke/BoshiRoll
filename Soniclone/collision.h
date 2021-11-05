#pragma once
#include <array>
#include <math.h>
#include "Vec2.h"
#include "CircleCollider.h"
#include "RectCollider.h"
#include "LineCollider.h"

bool checkCollision(CircleCollider &c1, CircleCollider &c2);
bool checkCollision(CircleCollider &c, RectCollider &r, Vec2 *cptr = nullptr);
bool checkCollison(RectCollider &r1, RectCollider &r2);
bool checkCollision(CircleCollider &c, LineCollider &l, Vec2 *cptr = nullptr);

Vec2 collisionDisp(CircleCollider &c1, CircleCollider &c2);
Vec2 collisionDisp(CircleCollider &c, RectCollider &r, Vec2 *cptr = nullptr);
Vec2 collisionDisp(RectCollider &r1, RectCollider &r2);
Vec2 collisionDisp(CircleCollider &c, LineCollider &l, Vec2 *cptr = nullptr);

