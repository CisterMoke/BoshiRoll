#pragma once
#include <array>
#include <memory>
#include "globals.h"
#include "Colliders.h"
#include "Entity.h"
#include "Level.h"

using glob::GAMETICKS;

class Game
{
public:
	std::shared_ptr<Entity> player;
	std::shared_ptr<Level> currLevel;
	bool paused = false;
	bool over = false;

	// Physics
	float g = 12.0f / GAMETICKS;
	float air_fric_t = 0.2f / GAMETICKS;
	float air_fric_r = 0.5f / GAMETICKS;
	float roll_fric = 0.1f / GAMETICKS;

	float bounciness = 0.2f;


	Game() = default;
	Game(Entity &player, Level &level);
	Game(std::shared_ptr<Entity> player, std::shared_ptr<Level> level);
	~Game();

	void tick();

private:
	bool checkCollisions();
	void applyAirFriction();
	void applyRollingFriction(float grip);
	float calculateGrip(float Fn, float sigma = -1.0f);
	float slipRatio(Vec2 &dir);
	float tractionForce(float slip, float grip);
};

