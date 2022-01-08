#include "Tongue.h"

Tongue::Tongue(Vec2 *origin)
	: tip(new Vec2(*origin)), end(new Vec2(*origin)), origin(origin)
{
	parts[0] = new Entity(glob::TONGUE_TIP, *tip);
	parts[parts.size() - 1] = new Entity(glob::TONGUE_END, *end);
	for (int i = 1; i < parts.size() - 1; i++)
	{
		parts[i] = new Entity(glob::TONGUE_BODY, *new Vec2(origin->x, origin->y));
	}
}

Tongue::~Tongue()
{
	for (Entity *e : parts) { delete e; }
}

TongueState Tongue::getState() { return state; }

Vec2 Tongue::springForce(Vec2 &disp)
{
	Vec2 norm;
	if (disp == Vec2(0.0f, 0.0f)) { norm = Vec2(0.0f, 0.01f); }
	else { norm = disp.normalize(); }
	float d = rest_l - disp.norm();

	return norm * (k * d);
}

void Tongue::shoot(Vec2 const &dir)
{
	reel_i = parts.size() - 2;
	Vec2 shoot_dir = dir;
	shoot_dir = shoot_dir.normalize();
	Vec2 vel = shoot_dir * shoot_speed;
	*parts[0]->vel = vel;
	state = SHOT;
}

void Tongue::release()
{
	for (int i = parts.size() - 2; i > -1; i--)
	{
		if (parts[i]->pos->dist(*end) < shoot_speed) { parts[i]->stop(); }
		else
		{ 
			reel_i = i;
			break;
		}
	}
	Vec2 return_dir = *end - *parts[reel_i]->pos;
	return_dir = return_dir.normalize();
	Vec2 vel = return_dir * shoot_speed;
	parts[reel_i]->stop();
	*parts[reel_i]->vel = vel;
	state = RELEASED;
}

void Tongue::anchor(Vec2 &pos)
{
	tip = &pos;
	state = ANCHORED;
}

void Tongue::idle()
{
	for (Entity *e : parts)
	{
		e->stop();
		*e->pos = *origin;
	}
	state = IDLE;
}

void Tongue::teleport(Vec2 &v)
{
	for (Entity *e : parts)
	{
		e->teleport(v);
	}
}

void Tongue::push(Vec2 &f)
{
	for (Entity *e : parts)
	{
		e->push(f);
	}
}

void Tongue::correctPos()
{
	Vec2 off = *origin - *end;
	teleport(off);
}

void Tongue::correctAngles()
{
	for (int i = 1; i < reel_i + 1; i++)
	{
		Vec2 dir = *parts[i - 1]->pos - *parts[i]->pos;
		float angle = atan2f(dir.y, dir.x);
		*parts[i]->theta = angle * 180.0f / M_PI;
		if (i == 1) { *parts[i - 1]->theta = angle * 180.0f / M_PI; }
	}
}

void Tongue::update()
{
	float dst;
	switch (state)
	{
	case IDLE:
		for (Entity *e : parts)
		{
			*e->pos = *origin;
		}
		break;
	case SHOT:
		correctPos();
		dst = tip->dist(*end);
		if (dst > max_len)
		{
			release();
		}
		break;
	case ANCHORED:
		break;
	case RELEASED:
		correctPos();
		dst = parts[reel_i]->pos->dist(*end);
		if (dst < shoot_speed)
		{
			if (reel_i == 0) { idle(); }
			else
			{ 
				parts[reel_i]->stop();
				reel_i--;
				Vec2 return_dir = *end - *parts[reel_i]->pos;
				return_dir = return_dir.normalize();
				Vec2 vel = return_dir * shoot_speed;
				parts[reel_i]->stop();
				*parts[reel_i]->vel = vel;
			}
		}
		break;
	}
	if (state != IDLE)
	{
		if (state == RELEASED)
		{
			Vec2 d_down = *parts[0]->pos - *parts[1]->pos;
			parts[0]->push(springForce(d_down));
		}
		for (int i = 1; i < reel_i; i++)
		{
			Vec2 d_up = *parts[i]->pos - *parts[i - 1]->pos;
			Vec2 d_down = *parts[i]->pos - *parts[i + 1]->pos;
			Vec2 F = springForce(d_up) + springForce(d_down);
			parts[i]->push(F);
		}
		for (Entity *e : parts) { e->update(); }
		correctAngles();
	}
	*end = *origin;
}
