#include "Tongue.h"

Tongue::Tongue(Vec2 *origin)
	: tip(new Vec2(*origin)), end(new Vec2(*origin)), origin(origin)
{
	parts[0] = new Entity(glob::TONGUE_TIP, *tip);
	parts[0]->mass = s_mass;
	tongueEnd = new BaseSprite();
	tongueEnd->loadFromFile(glob::TONGUE_END);
	for (int i = 1; i < parts.size(); i++)
	{
		parts[i] = new Entity(glob::TONGUE_BODY, *new Vec2(origin->x, origin->y));
		parts[i]->mass = s_mass;
	}
}

Tongue::~Tongue()
{
	for (Entity *e : parts) { delete e; }
}

TongueState Tongue::getState() { return state; }
Entity* Tongue::getTip() { return parts[0];  }
int Tongue::getReel() { return reel;  }

Vec2 Tongue::springForce(Vec2 &disp, Vec2 &vel, float m, bool half)
{
	Vec2 norm;
	if (disp == Vec2(0.0f, 0.0f)) { norm = Vec2(0.0f, 1.0f); }
	else { norm = disp.normalize(); }
	float r = rest_l - disp.norm();
	float d_hat = fminf(d, m/(1+half));
	float k_hat = fminf(k, 2*m);
	float f = k_hat * r - d_hat * norm.dot(vel);
	return norm * (f);
}

void Tongue::shoot(Vec2 const &dir, Vec2 const &r_vel)
{
	reel = 0;
	Vec2 shoot_dir = dir.normalize();
	Vec2 vel = shoot_dir * shoot_speed;
	*parts[0]->vel = vel + shoot_dir * r_vel.dot(shoot_dir);
	state = TongueState::SHOT;
}

void Tongue::release()
{
	for (int i = 0; i < reel + 1; i++)
	{
		parts[i]->stop();
	}
	Vec2 return_dir = *end - *parts[reel]->pos;
	return_dir = return_dir.normalize();
	Vec2 vel = return_dir * shoot_speed;
	*parts[reel]->vel = vel;
	state = TongueState::RELEASED;
}

void Tongue::anchor()
{
	anchor(*tip);
}

void Tongue::anchor(Vec2 &pos)
{
	tip = &pos;
	parts[0]->stop();
	state = TongueState::ANCHORED;
}

void Tongue::idle()
{
	for (Entity *e : parts)
	{
		e->stop();
		*e->pos = *origin;
	}
	state = TongueState::IDLE;
}

void Tongue::reelOut()
{
	if (parts[reel]->pos->dist(*end) > rest_l && reel < parts.size()-1)
	{
		*parts[++reel]->pos = *end;
		*parts[reel]->vel = *parts[0]->vel;
	}
	else if (tip->dist(*end) > rest_l * parts.size())
	{
		release();
	}
}

void Tongue::reelIn()
{
	if (parts[reel]->pos->dist(*end) < shoot_speed)
	{
		if (reel == 0)
		{
			idle();
			return;
		}
		else
		{
			parts[reel--]->stop();
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

void Tongue::applyGravity(float g)
{
	if (state == TongueState::ANCHORED)
	{
		Vec2 f = Vec2(0.0f, g * s_mass);
		for (int i = 1; i < reel + 1; i++)
		{
			parts[i]->push(f);
		}
	}
}

void Tongue::correctPos()
{
	Vec2 off = *origin - *end;
	teleport(off);
	*end = *origin;
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
	case TongueState::IDLE:
		for (Entity *e : parts)
		{
			*e->pos = *end;
		}
		*end = *origin;
		break;
	case TongueState::ANCHORED:
		*end = *origin;
		break;
	case TongueState::SHOT:
		*end = *origin;
		max_i = reel;
		reelOut();
		if (state != TongueState::RELEASED) { break; } // Fall through if released released in this step.
	case TongueState::RELEASED:
		correctPos();
		reelIn();
		min_i = 0;
		max_i = reel;
		break;
	}
	if (state != TongueState::IDLE)
	{
		for (int i = min_i; i < max_i; i++)
		{
			if (i == 0)
			{
				Vec2 d_down = *parts[i]->pos - *parts[i + 1]->pos;
				parts[i]->push(springForce(d_down, *parts[i]->vel, s_mass, 0));
			}
			else if (i == reel)
			{
				Vec2 d_up = *parts[i]->pos - *parts[i - 1]->pos;
				Vec2 d_down = *parts[i]->pos - *end;
				parts[i]->push(springForce(d_up, *parts[i]->vel, s_mass, 1) + springForce(d_down, *parts[i]->vel, s_mass, 1));
			}
			else
			{
				Vec2 d_up = *parts[i]->pos - *parts[i - 1]->pos;
				Vec2 d_down = *parts[i]->pos - *parts[i + 1]->pos;
				parts[i]->push(springForce(d_up, *parts[i]->vel, s_mass, 1) + springForce(d_down, *parts[i]->vel, s_mass, 1));
			}
		}
		for (Entity *e : parts) { e->update(); }
		correctAngles();
	}
}

void Tongue::render(SDL_Renderer *renderer, Vec2 const &orig, Vec2 const &offset, float phi, float zx, float zy)
{
	if (state != TongueState::IDLE)
	{
		for (int i = 0; i < reel + 1; i++)
		{
			parts[i]->render(renderer, orig, offset, phi, zx, zy);
		}
		tongueEnd->renderAt(*end - orig, offset, phi, zx, zy);
	}
}
