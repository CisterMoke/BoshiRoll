#include "Entity.h"

Entity::Entity(std::string spritePath, float zoom, int mode)
{
	sprite = new BaseSprite();
	sprite->loadFromFile(spritePath, mode);
	sprite->zoom(zoom, zoom);
	theta = &sprite->theta;
	collider = new CircleCollider(pos, sprite->getWidth()/2);
}
Entity::Entity(std::string spritePath, Vec2 &pos, float zoom, int mode)
	:Entity(spritePath, zoom, mode)
{ 
	this->pos = &pos;
	delete this->collider->pos;
	this->collider->pos = &pos;
}
Entity::Entity(std::string spritePath, float x, float y, float zoom, int mode)
	:Entity(spritePath, zoom, mode)
{
	pos = new Vec2(x, y);
	delete this->collider->pos;
	this->collider->pos = pos;
}
Entity::~Entity()
{
	delete pos, vel, t_force, sprite, collider;
}

void Entity::teleport(Vec2 const &v) { *pos += v; }
void Entity::push(Vec2 const &f) { *t_force += f; }
void Entity::rotate(float angle)
{ *theta = fmodf(fmodf(*theta + angle, 360) + 360,  360); }
void Entity::spin(float alpha) { r_force += alpha; }
void Entity::stop()
{
	*vel = Vec2(0.0f, 0.0f);
	*t_force = Vec2(0.0f, 0.0f);
	omega = 0.0f;
	r_force = 0.0f;
}
void Entity::applyVelocities()
{
	teleport(*vel);
	rotate(omega);
}

void Entity::applyForces()
{
	*vel += *t_force;
	*t_force = Vec2(0.0f, 0.0f);

	omega += r_force;
	r_force = 0.0f;
}

void Entity::update()
{
	applyForces();
	applyVelocities();
}

void Entity::render(SDL_Renderer *renderer, Vec2 const &orig, Vec2 const &offset, float phi, float zx, float zy)
{
	sprite->renderAt(*pos - orig, offset, phi, zx, zy);
}