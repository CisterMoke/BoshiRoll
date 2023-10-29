#include "Enemy.h"

b2Body *Enemy::add_to(b2World &world, const Vec2 &pos)
{
	b2BodyDef bodydef;
	bodydef.type = b2_kinematicBody;
	bodydef.position.Set(pos.x, pos.y);


	b2PolygonShape collider;
	collider.SetAsBox(sprite->get_width() / 2.0f, sprite->get_height() / 2.0f);

	body = world.CreateBody(&bodydef);
	body->CreateFixture(&collider, 1.0f);
	return body;
}

Enemy::Enemy(std::shared_ptr<AnimSprite> sprite, b2World &world, const Vec2 &pos)
	: Entity(sprite, world, pos), sprite(sprite)
{
	add_to(world, pos);
}