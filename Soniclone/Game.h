#pragma once
#include <memory>
#include "Colliders.h"
#include "Entity.h"
#include "Level.h"
class Game
{
public:
	std::shared_ptr<Entity> player;
	std::shared_ptr<Level> currLevel;
	bool paused = false;
	bool over = false;

	Game() = default;
	Game(Entity &player, Level &level);
	Game(std::shared_ptr<Entity> player, std::shared_ptr<Level> level);
	~Game();

	void tick();

private:
	void checkCollisions();

};

