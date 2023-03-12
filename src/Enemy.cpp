#include "Enemy.h"

Enemy::Enemy(std::shared_ptr<AnimSprite> sprite, const Vec2 &pos)
	: Entity(sprite, pos), sprite(sprite),
	collider(new RectCollider(std::shared_ptr<Vec2>(&this->pos, [](Vec2 *) {}), this->sprite->get_width(), this->sprite->get_height()))
{}

Enemy::Enemy(std::shared_ptr<AnimSprite> sprite, int x, int y)
	: Enemy(sprite, Vec2(x, y)) {}