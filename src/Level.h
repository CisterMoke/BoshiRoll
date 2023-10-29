#pragma once
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <box2d.h>
#include "BaseSprite.h"
#include "Enemy.h"


class Level
{

private:

	std::shared_ptr<BaseSprite> background{};
	std::shared_ptr<BaseSprite> foreground{};
	
	Vec2 spawn{};

	std::list<Enemy> enemies{}; // use linked list to prevent deleted copy constructor being called
	std::vector<Vec2> checkpoints{};

	std::vector<std::vector<Vec2>> vertex_groups{};
	std::vector<bool> loop_flags{};

	static b2ChainShape get_chain(const std::vector<Vec2> &vertices, bool loop);

public:
	Level() = default;

	b2Body *add_to(b2World &world);

	const Vec2 get_spawn() const;
	void set_spawn(Vec2 spawn);
	const std::vector<Vec2> get_checkpoints() const;
	void add_checkpoint(Vec2 checkpoint);
	const std::list<Enemy> &get_enemies();
	void add_enemy(Enemy enemy);
	void add_vgroup(std::vector<Vec2> vgroup, bool loop);
	
	BaseSprite &get_background();
	BaseSprite &get_foreground();
	Level &set_background(std::shared_ptr<BaseSprite> background);
	Level &set_foreground(std::shared_ptr<BaseSprite> foreground);



};

