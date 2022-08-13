#pragma once
#include <array>
#include <memory>
#include "globals.h"
#include "Colliders.h"
#include "Player.h"
#include "Level.h"
#include "RenderCommands.h"
#include "Sprites.h"

using glob::GAMETICKS;

class Game
{
public:
	std::shared_ptr<Player> player;
	std::shared_ptr<Level> currLevel;
	Camera *camera;
	bool paused = false;
	bool over = false;

	// Physics
	float g = 12.0f / GAMETICKS;
	float air_fric_t = 0.2f / GAMETICKS;
	float air_fric_r = 0.5f / GAMETICKS;
	float roll_fric = 0.1f / GAMETICKS;

	float bounciness = 0.2f;


	Game() = default;
	Game(Player &player, Level &level);
	Game(std::shared_ptr<Player> player, std::shared_ptr<Level> level);
	~Game();

	void init();
	void tick();
	void push_render_commands();

private:
	bool check_collisions();
	void apply_air_friction(Entity &e);
	void apply_rolling_friction(Entity &e, float grip);
	float calculate_grip(float Fn, float sigma = -1.0f);
	float slip_ratio(Entity &e, Vec2 &dir);
	float traction_force(float slip, float grip);
};

