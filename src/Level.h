#pragma once
#include <memory>
#include <string>
#include <vector>
#include "globals.h"
#include "BaseSprite.h"
#include "Colliders.h"
#include "Enemy.h"
#include "Vec2.h"

class Level
{

private:

	Level();

	std::shared_ptr<BaseSprite> background = nullptr;
	std::shared_ptr<BaseSprite> foreground = nullptr;

public:

	std::vector<std::shared_ptr<BaseCollider>> colliders = {};
	std::vector<std::shared_ptr<Enemy>> enemies = {};
	Vec2 spawn = Vec2(0.0f, 0.0f);
	std::vector<Vec2> checkpoints = {};

	static Level init_level();

	Level(const Level &other);
	
	BaseSprite &get_background();
	BaseSprite &get_foreground();
	Level *set_background(std::shared_ptr<BaseSprite> background);
	Level *set_foreground(std::shared_ptr<BaseSprite> foreground);

};

