#include "Entity.h"

EntityData::EntityData(b2Body &body)
	: pos((const Vec2&)body.GetPosition()), vel((const Vec2 &)body.GetLinearVelocity()),
	theta(body.GetAngle()), omega(body.GetAngularVelocity()), mass(body.GetMass()),
	inertia(body.GetInertia()){}

Entity::Entity(std::shared_ptr<BaseSprite> sprite, b2World &world, const Vec2 &pos)
	: sprite(sprite), world(&world), body() {}


Entity::~Entity() {} // Necessary for pure virtual destructors appartently

EntityData Entity::get_data() const
{
	return EntityData(*body);
}

const Vec2 &Entity::get_pos() const
{
	return (const Vec2 &)body->GetPosition();
}

float Entity::get_theta() const
{
	return body->GetAngle();
}

void Entity::set_theta(float angle)
{
	body->SetTransform(get_pos(), angle);
	sprite->set_theta(angle);
}

float Entity::get_size() const
{
	return body->GetFixtureList()[0].GetShape()->m_radius;
}

void Entity::grow(float scale)
{
	Vec2 new_zoom = sprite->get_zoom() * scale;
	sprite->zoom(new_zoom.x, new_zoom.y);
	body->GetFixtureList()[0].GetShape()->m_radius *= scale;
}

void Entity::teleport(Vec2 const &v)
{
	body->SetTransform(v, get_theta());
}

void Entity::push(Vec2 const &f)
{
	body->ApplyLinearImpulseToCenter(f, true);
}

void Entity::rotate(float angle)
{ 
	sprite->rotate(angle);
	float new_theta = fmodf(fmodf(get_theta() + angle, 360) + 360, 360);
	set_theta(new_theta);
}

void Entity::spin(float alpha)
{
	body->ApplyAngularImpulse(alpha, true);
}

void Entity::stop()
{
	body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	body->SetAngularVelocity(0.0f);
}

//void Entity::apply_velocities()
//{
//	teleport(vel);
//	rotate(omega);
//}
//
//void Entity::apply_forces()
//{
//	vel += t_force/mass;
//	t_force = Vec2(0.0f, 0.0f);
//
//	omega += r_force/mass;
//	r_force = 0.0f;
//}
//
void Entity::update()
{
	sprite->set_theta(get_theta());
}