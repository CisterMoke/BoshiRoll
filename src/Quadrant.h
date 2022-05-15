#pragma once
#include "Vec2.h"

enum class Quadrant
{
	I = 0, II = 1, III = 2, IV = 3
};

Quadrant operator+ (Quadrant q, int i);
Quadrant operator+ (int i, Quadrant q);
Quadrant operator- (Quadrant q, int i);
Quadrant operator- (int i, Quadrant q);

Quadrant toQuad(Vec2 v);
Vec2 toVec(Quadrant q);
