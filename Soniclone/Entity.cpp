#include "Entity.h"

Entity::Entity(std::string spritePath, float zoom, int mode)
{
	sprite = new BaseSprite();
	sprite->loadFromFile(spritePath, mode);
	sprite->zoom(zoom, zoom);
	theta = &sprite->theta;
	collider = new CircleCollider(pos, sprite->getWidth()/2);
}
Entity::Entity(std::string spritePath, Vec2 *pos, float zoom, int mode)
	:Entity(spritePath, zoom, mode) { pos = pos; }
Entity::Entity(std::string spritePath, float x, float y, float zoom, int mode)
	:Entity(spritePath, zoom, mode)  {pos = new Vec2(x, y); }
Entity::~Entity()
{
	delete pos, vel, t_force, sprite, collider;
}

void Entity::teleport(Vec2 const &v) { *pos += v; }
void Entity::push(Vec2 const &f) { *t_force += f; }
void Entity::rotate(float angle)
{ *theta = fmodf(fmodf(*theta + angle, 360) + 360,  360); }
void Entity::spin(float alpha) { r_force += alpha; }
void Entity::update()
{
	applyTFriction();
	applyTForce();
	applyRFriction();
	applyRForce();
	*pos += *vel;
	*theta += omega;
}
void Entity::render(SDL_Renderer *renderer, Vec2 const &offset)
{
	sprite->renderAt(*pos + offset);
}

void Entity::doAction(SDL_Event &event)
{
	if (event.type == SDL_KEYDOWN)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_SPACE:
			break;

		case SDLK_UP:
			*t_force += Vec2(0.0f, -5.0f);
			break;

		case SDLK_DOWN:
			*t_force += Vec2(0.0f, 5.0f);
			break;

		case SDLK_LEFT:
			*t_force += Vec2(-5.0f, 0.0f);
			r_force -= 2;
			break;

		case SDLK_RIGHT:
			*t_force += Vec2(5.0f, 0.0f);
			r_force += 2;
			break;
		}
	}
}

void Entity::applyTForce()
{
	*vel += *t_force;
	*t_force = Vec2(0.0f, 0.0f);
}

void Entity::applyTFriction()
{
	float vel_norm = vel->norm();
	if (fabsf(vel_norm) < 0.01)
	{
		*vel = Vec2(0.0f, 0.0f);
		return;
	}
	*vel -= *vel * 0.05;
	return;
}

void Entity::applyRForce()
{
	omega += r_force;
	r_force = 0.0f;
}

void Entity::applyRFriction()
{
	if (fabsf(omega) < 0.01f)
	{
		omega = 0.0f;
		return;
	}
	omega -= omega * 0.005;
}