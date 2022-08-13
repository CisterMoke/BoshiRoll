#include "Quadrant.h"

Quadrant operator+ (Quadrant q, int i) { return static_cast<Quadrant>(((int)q + i) % 4); }
Quadrant operator+ (int i, Quadrant q) { return q + i; }
Quadrant operator- (Quadrant q, int i) { return q + (-i); }
Quadrant operator- (int i, Quadrant q) { return q + (-i); }

Quadrant to_quad(Vec2 v)
{
	if (v.x >= 0 && v.y <= 0) { return Quadrant::I; }
	else if (v.x >= 0 && v.y > 0) { return Quadrant::II; }
	else if (v.x < 0 && v.y > 0) { return Quadrant::III; }
	else { return Quadrant::IV; }
}

Vec2 to_vec(Quadrant q)
{
	switch (q)
	{
	case Quadrant::I:
		return Vec2(1.0f, -1.0f);
	case Quadrant::II:
		return Vec2(1.0f, 1.0f);
	case Quadrant::III:
		return Vec2(-1.0f, 1.0f);
	case Quadrant::IV:
		return Vec2(-1.0f, -1.0f);
	}
}