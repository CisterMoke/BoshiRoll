#pragma once
#include <memory>
#include <string>
#include <vector>
#include <list>
#include "globals.h"
#include "BaseSprite.h"
#include "Colliders.h"
#include "Enemy.h"
#include "Vec2.h"


class Level
{

private:


	std::shared_ptr<BaseSprite> background{};
	std::shared_ptr<BaseSprite> foreground{};
	
	bool initialized{};
	Vec2 spawn{};

	std::vector< std::unique_ptr<BaseCollider>> colliders{};
	std::list<Enemy> enemies{}; // use linked list to prevent deleted copy constructor being called
	std::vector<Vec2> checkpoints{};

public:
	Level() = default;

	bool is_init();
	void init();

	const Vec2 get_spawn() const;
	const std::list<Enemy> &get_enemies();
	const std::vector<BaseCollider*> get_colliders() const;
	
	BaseSprite &get_background();
	BaseSprite &get_foreground();
	Level &set_background(std::shared_ptr<BaseSprite> background);
	Level &set_foreground(std::shared_ptr<BaseSprite> foreground);

};

