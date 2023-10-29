#pragma once
#include <memory>
#include <string>
#include "AnimSprite.h"
#include "Colliders.h"
#include "Entity.h"

class Enemy : public Entity
{
protected:
	virtual b2Body *add_to(b2World &world, const Vec2 &pos);

public:
	std::shared_ptr<AnimSprite> sprite;

	Enemy(std::shared_ptr<AnimSprite> sprite, b2World &world, const Vec2 &pos = Vec2(0.0f, 0.0f));
	virtual ~Enemy() = default;
};
