#include "Colliders.h"

BaseCollider::BaseCollider(std::shared_ptr<Vec2> p)
	: pos(p) {}

CircleCollider::CircleCollider(std::shared_ptr<Vec2> p, float radius)
	: BaseCollider(p), r(radius) {}

CircleCollider::CircleCollider(float x, float y, float radius)
	: CircleCollider(std::shared_ptr<Vec2>(new Vec2(x, y)), radius) {}

bool CircleCollider::check_collision(CircleCollider &c, Vec2 *cptr) const
{
	float dist = this->pos->dist(*c.pos);
	return dist < this->r + c.r;
}

Vec2 CircleCollider::collision_disp(CircleCollider &c, Vec2 *cptr) const
{
	Vec2 dir = *c.pos - *this->pos;
	float d = dir.norm() - this->r + c.r ;

	return dir.normalize() * d;
}

Vec2 CircleCollider::anti_collision_disp(CircleCollider &c, Vec2 *cptr) const
{
	Vec2 dir = *this->pos - *c.pos;
	float d = dir.norm() - this->r + c.r;

	return dir.normalize() * d;
}

BaseRenderCommand *CircleCollider::create_cmd(Camera *camera, SDL_Color color)
{
	return new RenderColliderCommand<CircleCollider>(this, camera, color);
}

LineCollider::LineCollider(std::shared_ptr<Vec2>  start, std::shared_ptr<Vec2>  stop)
	: BaseCollider(std::shared_ptr<Vec2> (new Vec2((*start + *stop) * 2))), start(start), stop(stop) {}

bool LineCollider::check_collision(CircleCollider &c, Vec2 *cptr) const
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

Vec2 LineCollider::collision_disp(CircleCollider &c, Vec2 *cptr) const
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

RectCollider::RectCollider(std::shared_ptr<Vec2> lu, float width, float height)
	: BaseCollider(std::shared_ptr<Vec2>(new Vec2(*lu + Vec2(width / 2, height / 2)))), lu(lu), w(width), h(height) {}

RectCollider::RectCollider(float x, float y, float width, float height)
	: RectCollider(std::shared_ptr<Vec2>(new Vec2(x, y)), width, height) {}

bool RectCollider::check_collision(CircleCollider &c, Vec2 *cptr) const
{
	Vec2 closest = Vec2(0.0f, 0.0f);
	Vec2 &cp = *c.pos;
	Vec2 &rp = *this->lu;

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

bool RectCollider::check_collision(RectCollider &r) const
{
	Vec2 &tp = *this->lu;
	Vec2 &rp = *r.lu;

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

Vec2 RectCollider::collision_disp(CircleCollider &c, Vec2 *cptr) const
{
	bool local = (cptr == nullptr);
	if (local)
	{
		cptr = new Vec2(0.0f, 0.0f);
		check_collision(c, cptr);
	}

	if (*cptr == *c.pos)
	{
		float x_proj = c.pos->x < (this->lu->x + this->w) / 2 ?
			this->lu->x : this->lu->x = this->w;
		float y_proj = c.pos->y < (this->lu->y + this->h) / 2 ?
			this->lu->y : this->lu->y = this->h;
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

Vec2 RectCollider::collision_disp(RectCollider &r) const
{
	Vec2 &tp = *this->lu;
	Vec2 &rp = *r.lu;
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

RampCollider::RampCollider(std::shared_ptr<Vec2>  lu, float size, Quadrant quadrant) :
	BaseCollider(std::shared_ptr<Vec2>(new Vec2(*lu + Vec2(size / 2, size / 2)))), lu(lu), sz(size), quad(quadrant), rect(new RectCollider(lu, size, size))
{
	Vec2 *ctr = new Vec2(*lu);
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
	circ = new CircleCollider(std::shared_ptr<Vec2> (ctr), size);
	Vec2 *prev_c = new Vec2(*circ->pos + ((to_vec(quad - 1) - to_vec(quad)) * sz / 2));
	Vec2 *next_c = new Vec2(*circ->pos + ((to_vec(quad + 1) - to_vec(quad)) * sz / 2));
	line = new LineCollider(std::shared_ptr<Vec2> (prev_c), std::shared_ptr<Vec2> (next_c));
}

RampCollider::RampCollider(float x, float y, float size, Quadrant quadrant)
	: RampCollider(std::shared_ptr<Vec2> (new Vec2(x, y)), size, quadrant) {}

RampCollider::~RampCollider()
{
	delete circ, rect, line;
}

bool RampCollider::check_collision(CircleCollider &c, Vec2 *cptr) const
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

Vec2 RampCollider::collision_disp(CircleCollider &c, Vec2 *cptr) const
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