#include "Entity.h"

Entity::Entity(std::shared_ptr<BaseSprite> sprite, const Vec2 &pos)
	: sprite(sprite), pos(pos) {}

Entity::Entity(std::shared_ptr<BaseSprite> sprite, float x, float y)
	: Entity(sprite, Vec2(x, y)) {}

Entity::~Entity() {} // Necessary for pure virtual destructors appartently

void Entity::set_theta(float angle)
{
	theta = angle;
	sprite->set_theta(angle);
}

void Entity::teleport(Vec2 const &v) { pos += v; }
void Entity::push(Vec2 const &f) { t_force += f; }
void Entity::rotate(float angle)
{ 
	sprite->rotate(angle);
	theta = fmodf(fmodf(theta + angle, 360) + 360,  360);
}
void Entity::spin(float alpha) { r_force += alpha; }
void Entity::stop()
{
	vel = Vec2(0.0f, 0.0f);
	t_force = Vec2(0.0f, 0.0f);
	omega = 0.0f;
	r_force = 0.0f;
}
void Entity::apply_velocities()
{
	teleport(vel);
	rotate(omega);
}

void Entity::apply_forces()
{
	vel += t_force/mass;
	t_force = Vec2(0.0f, 0.0f);

	omega += r_force/mass;
	r_force = 0.0f;
}

void Entity::update()
{
	apply_forces();
	apply_velocities();
}