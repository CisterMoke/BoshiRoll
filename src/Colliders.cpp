#include "Colliders.h"

CircleCollider::CircleCollider() {}
CircleCollider::CircleCollider(Vec2 *p, float radius)
	: pos(p), r(radius) {}
CircleCollider::CircleCollider(float x, float y, float radius)
	: pos(new Vec2(x, y)), r(radius) {}
CircleCollider::~CircleCollider()
{
	delete pos;
}

bool CircleCollider::check_collision(CircleCollider &c, Vec2 *cptr)
{
	float dist = this->pos->dist(*c.pos);
	return dist < this->r + c.r;
}

Vec2 CircleCollider::collision_disp(CircleCollider &c, Vec2 *cptr)
{
	Vec2 dir = *c.pos - *this->pos;
	float d = dir.norm() - this->r + c.r ;

	return dir.normalize() * d;
}

Vec2 CircleCollider::anti_collision_disp(CircleCollider &c, Vec2 *cptr)
{
	Vec2 dir = *this->pos - *c.pos;
	float d = dir.norm() - this->r + c.r;

	return dir.normalize() * d;
}

BaseRenderCommand *CircleCollider::create_cmd(Camera *camera, SDL_Color color)
{
	return new RenderColliderCommand<CircleCollider>(this, camera, color);
}

LineCollider::LineCollider() {};
LineCollider::LineCollider(Vec2 *start, Vec2 *stop)
	: start(start), stop(stop) {};
LineCollider::~LineCollider()
{
	delete start;
	delete stop;
}

bool LineCollider::check_collision(CircleCollider &c, Vec2 *cptr)
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

Vec2 LineCollider::collision_disp(CircleCollider &c, Vec2 *cptr)
{
	bool local = (cptr == nullptr);
	if (local)
	{
		cptr = new Vec2(0.0f, 0.0f);
		check_collision(c, cptr);
	}
	Vec2 closest = *cptr;
	Vec2 dir = *c.pos - closest;
	float d = c.r - dir.norm();

	if (local) delete cptr;

	return dir.normalize() * d;
}

BaseRenderCommand *LineCollider::create_cmd(Camera *camera, SDL_Color color)
{
	return new RenderColliderCommand<LineCollider>(this, camera, color);
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

bool RectCollider::check_collision(CircleCollider &c, Vec2 *cptr)
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

bool RectCollider::check_collision(RectCollider &r)
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

Vec2 RectCollider::collision_disp(CircleCollider &c, Vec2 *cptr)
{
	bool local = (cptr == nullptr);
	if (local)
	{
		cptr = new Vec2(0.0f, 0.0f);
		check_collision(c, cptr);
	}

	if (*cptr == *c.pos)
	{
		float x_proj = c.pos->x < (this->pos->x + this->w) / 2 ?
			this->pos->x : this->pos->x = this->w;
		float y_proj = c.pos->y < (this->pos->y + this->h) / 2 ?
			this->pos->y : this->pos->y = this->h;
		float x_diff = fabs(c.pos->x - x_proj);
		float y_diff = fabs(c.pos->y - y_proj);
		if (x_diff < y_diff) { cptr->x = x_proj; }
		else { cptr->y = y_proj; }
	}
	Vec2 closest = *cptr;
	Vec2 dir = *c.pos - closest;
	float d = c.r - dir.norm();

	if (local) delete cptr;

	return dir.normalize() * d;
}

Vec2 RectCollider::collision_disp(RectCollider &r)
{
	Vec2 &tp = *this->pos;
	Vec2 &rp = *r.pos;
	float xdisp, ydisp;
	xdisp = fmaxf(0, fminf(tp.x + this->w - rp.x, tp.x - rp.x + r.w));
	ydisp = fmaxf(0, fminf(tp.y + this->h - rp.y, tp.y - rp.y + r.h));

	return xdisp < ydisp ? Vec2(xdisp, 0.0f) : Vec2(0.0f, ydisp);
}

BaseRenderCommand *RectCollider::create_cmd(Camera *camera, SDL_Color color)
{
	return new RenderColliderCommand<RectCollider>(this, camera, color);
}

RampCollider::RampCollider() : circ(new CircleCollider()), rect(new RectCollider()), line(new LineCollider()) {}

RampCollider::RampCollider(Vec2 *p, float size, Quadrant quadrant) :
	pos(p), sz(size), quad(quadrant), rect(new RectCollider(p, size, size))
{
	Vec2 *ctr = new Vec2(*pos);
	switch (quad)
	{
	case Quadrant::I:
		*ctr += Vec2(size, 0.0f);
		break;
	case Quadrant::II:
		*ctr += Vec2(size, size);
		break;
	case Quadrant::III:
		*ctr += Vec2(0.0f, 0.0f);
		break;
	case Quadrant::IV:
		break;
	}
	circ = new CircleCollider(ctr, size);
	Vec2 *prev_c = new Vec2(*circ->pos + ((to_vec(quad - 1) - to_vec(quad)) * sz / 2));
	Vec2 *next_c = new Vec2(*circ->pos + ((to_vec(quad + 1) - to_vec(quad)) * sz / 2));
	line = new LineCollider(prev_c, next_c);
}

RampCollider::RampCollider(float x, float y, float size, Quadrant quadrant)
	: RampCollider(new Vec2(x, y), size, quadrant) {}

RampCollider::~RampCollider()
{
	delete pos, circ, rect, line;
}

bool RampCollider::check_collision(CircleCollider &c, Vec2 *cptr)
{
	if (!rect->check_collision(c, cptr))
	{
		return false;
	}
	else
	{
		Vec2 diff = *c.pos - *circ->pos;
		float d = diff.norm();
		Quadrant q = to_quad(diff);
		if (q == quad + 2)
		{
			if (d >= circ->r - c.r)
			{
				return true;
			}
			return false;
		}

		Vec2 prev_c = *line->start;
		Vec2 next_c = *line->stop;
		if (c.pos->dist(prev_c) <= c.r || c.pos->dist(next_c) <= c.r)
		{
			return true;
		}
		return false;
	}
}

Vec2 RampCollider::collision_disp(CircleCollider &c, Vec2 *cptr)
{
	bool local = (cptr == nullptr);
	if (local)
	{
		cptr = new Vec2(0.0f, 0.0f);
	}

	Vec2 diff = *c.pos  - *circ->pos;
	float d = diff.norm();
	Quadrant q = to_quad(diff);

	Vec2 prev_c = *line->start;
	Vec2 next_c = *line->stop;
	if (q == quad + 2)
	{
		if (c.r < circ->r && d < circ->r)
		{
			circ->check_collision(c, cptr);
			return circ->anti_collision_disp(c, cptr);
		}
		else if (d < circ->r)
		{
			line->check_collision(c, cptr);
			return line->collision_disp(c, cptr);
		}
		else
		{
			return rect->collision_disp(c, cptr);
		}
	}

	float prev_d = c.pos->dist(prev_c);
	float next_d = c.pos->dist(next_c);
	if (prev_d <= c.r && next_d <= c.r)
	{
		line->check_collision(c, cptr);
		return line->collision_disp(c, cptr);
	}
	else if (prev_d <= c.r) { *cptr = prev_c; }
	else if (next_d <= c.r) { *cptr = next_c; }

	Vec2 closest = *cptr;
	Vec2 dir = *c.pos - closest;
	float d2 = c.r - dir.norm();

	if (local) delete cptr;

	return dir.normalize() * d2;
}

BaseRenderCommand *RampCollider::create_cmd(Camera *camera, SDL_Color color)
{
	return new RenderColliderCommand<RampCollider>(this, camera, color);
}