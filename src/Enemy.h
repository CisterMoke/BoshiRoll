#pragma once
#include <memory>
#include <string>
#include "AnimSprite.h"
#include "Colliders.h"
#include "Entity.h"

class Enemy : public Entity
{
public:
	std::shared_ptr<AnimSprite> sprite;
	std::unique_ptr<RectCollider> collider;

	Enemy(std::shared_ptr<AnimSprite> sprite, const Vec2 &pos = Vec2(0.0f, 0.0f));
	Enemy(std::shared_ptr<AnimSprite> sprite, int x, int y);
	~Enemy() = default;
};
