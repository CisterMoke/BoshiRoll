#include "Tongue.h"

Tongue::Tongue(Vec2 *origin)
	: tip(new Vec2(*origin)), end(new Vec2(*origin)), origin(origin)
{
	parts[0] = new Entity(glob::TONGUE_TIP, *tip);
	tongueEnd = new BaseSprite();
	tongueEnd->loadFromFile(glob::TONGUE_END);
	for (int i = 1; i < parts.size(); i++)
	{
		parts[i] = new Entity(glob::TONGUE_BODY, *new Vec2(origin->x, origin->y));
	}
}

Tongue::~Tongue()
{
	for (Entity *e : parts) { delete e; }
}

TongueState Tongue::getState() { return state; }

Vec2 Tongue::springForce(Vec2 &disp, Vec2 &vel)
{
	Vec2 norm;
	if (disp == Vec2(0.0f, 0.0f)) { norm = Vec2(0.0f, 1.0f); }
	else { norm = disp.normalize(); }
	float r = rest_l - disp.norm();

	return norm * (k * r - d * norm.dot(vel));
}

void Tongue::shoot(Vec2 const &dir)
{
	reel = 0;
	Vec2 shoot_dir = dir;
	shoot_dir = shoot_dir.normalize();
	Vec2 vel = shoot_dir * shoot_speed;
	*parts[0]->vel = vel;
	state = SHOT;
}

void Tongue::release()
{
	Vec2 return_dir = *end - *parts[reel]->pos;
	return_dir = return_dir.normalize();
	Vec2 vel = return_dir * shoot_speed;
	parts[reel]->stop();
	*parts[reel]->vel = vel;
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

void Tongue::reel_out()
{
	if (parts[reel]->pos->dist(*end) > reel_len)
	{
		if (reel == parts.size() - 1) { release(); }
		else
		{
			reel++;
			*parts[reel]->vel = *parts[0]->vel;
		}
	}
}

void Tongue::reel_in()
{
	if (parts[reel]->pos->dist(*end) < shoot_speed)
	{
		if (reel == 0) { idle(); }
		else
		{
			parts[reel]->stop();
			reel--;
			Vec2 return_dir = *end - *parts[reel]->pos;
			return_dir = return_dir.normalize();
			Vec2 vel = return_dir * shoot_speed;
			parts[reel]->stop();
			*parts[reel]->vel = vel;
		}
	}
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
	Vec2 dir;
	float angle;
	for (int i = 1; i < reel + 1; i++)
	{
		dir = *parts[i - 1]->pos - *parts[i]->pos;
		angle = atan2f(dir.y, dir.x);
		*parts[i]->theta = angle * 180.0f / M_PI;
		if (i == 1) { *parts[i - 1]->theta = angle * 180.0f / M_PI; }
	}
	dir = *parts[reel]->pos - *end;
	angle = atan2f(dir.y, dir.x);
	tongueEnd->setTheta(angle * 180.0f / M_PI);
}

void Tongue::update()
{
	int min_i = 1;
	int max_i = reel+1;
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
		reel_out();
		break;
	case ANCHORED:
		break;
	case RELEASED:
		correctPos();
		reel_in();
		min_i = 0;
		max_i = reel;
		break;
	}
	if (state != IDLE)
	{
		for (int i = min_i; i < max_i; i++)
		{
			if (i == 0)
			{
				Vec2 d_down = *parts[i]->pos - *parts[i + 1]->pos;
				parts[i]->push(springForce(d_down, *parts[i]->vel));
			}
			else if (i == parts.size() - 1)
			{
				Vec2 d_up = *parts[i]->pos - *parts[i - 1]->pos;
				parts[i]->push(springForce(d_up, *parts[i]->vel));
			}
			else
			{
				Vec2 d_up = *parts[i]->pos - *parts[i - 1]->pos;
				Vec2 d_down = *parts[i]->pos - *parts[i + 1]->pos;
				parts[i]->push(springForce(d_up, *parts[i]->vel) + springForce(d_down, *parts[i]->vel));
			}
		}
		for (Entity *e : parts) { e->update(); }
		correctAngles();
	}
	*end = *origin;
}

void Tongue::render(SDL_Renderer *renderer, Vec2 const &orig, Vec2 const &offset, float phi, float zx, float zy)
{
	if (state != IDLE)
	{
		for (int i = 0; i < reel + 1; i++)
		{
			parts[i]->render(renderer, orig, offset, phi, zx, zy);
		}
		tongueEnd->renderAt(*end - orig, offset, phi, zx, zy);
	}
}
