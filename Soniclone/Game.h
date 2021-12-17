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
	float g = 9.0f / GAMETICKS;
	float air_fric_t = 1.5f / GAMETICKS;
	float air_fric_r = 0.5f / GAMETICKS;

	float bounciness = 0.5f;


	Game() = default;
	Game(Entity &player, Level &level);
	Game(std::shared_ptr<Entity> player, std::shared_ptr<Level> level);
	~Game();

	void tick();

private:
	void checkCollisions();
	void applyAirFriction();
	float calculateGrip(float Fn, float sigma = -1.0f);
	float slipRatio(Vec2 &dir);
	float tractionForce(float slip, float grip);
	std::array<float, 2> calculateGripForce(float grip, Vec2 &dir);

};

