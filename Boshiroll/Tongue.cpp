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

void Tongue::shoot(Vec2 const &dir)
{
	shootDir = dir;
	shootDir = shootDir.normalize();
	float angle = atan2f(shootDir.y, shootDir.x);
	for (int i = 0; i < parts.size(); i++)
	{
		Vec2 vel = shootDir * shootSpeed * (parts.size() -1 - i) / parts.size();
		*parts[i]->vel = vel;
		*parts[i]->theta = angle * 180.0f / M_PI;
	}
	state = SHOT;
}

void Tongue::release()
{
	shootDir = *end - *tip;
	shootDir = shootDir.normalize();
	for (int i = 0; i < parts.size(); i++)
	{
		Vec2 vel = shootDir * shootSpeed * (parts.size() - 1 - i) / parts.size();
		parts[i]->stop();
		*parts[i]->vel = vel;
	}
	state = RELEASED;
}

void Tongue::anchor(Vec2 *pos)
{
	tip = pos;
	state = ANCHORED;
}

void Tongue::idle()
{
	for (int i = 0; i < parts.size(); i++)
	{
		parts[i]->stop();
		*parts[i]->pos = *origin;
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

void Tongue::update()
{
	float dst;
	bool update = true;
	switch (state)
	{
	case IDLE:
		for (int i = 0; i < parts.size(); i++)
		{
			*parts[i]->pos = *origin;
		}
		update = false;
		break;
	case SHOT:
		correctPos();
		dst = tip->dist(*end);
		if (dst > maxLen)
		{
			release();
		}
		break;
	case ANCHORED:
		break;
	case RELEASED:
		correctPos();
		dst = tip->dist(*end);
		if (dst < shootSpeed)
		{
			idle();
		}
		break;
	}
	if (update) { for (Entity *e : parts) { e->applyVelocities(); } }
	*end = *origin;
}
